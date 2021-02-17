/* Classe encarregada de fazer a geração adaptativa das malhas de acordo com as
curvaturas.
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "../../Headers/Generator/GeradorAdaptativoPorCurvatura.h"

GeradorAdaptativoPorCurvatura::GeradorAdaptativoPorCurvatura()
{

}

#if USE_OPENMP
SubMalha *GeradorAdaptativoPorCurvatura::malhaInicialOmp(CoonsPatch *patch, Performer::IdManager *idManager)
{
    Curva* c1 = patch->getCurva ( 0 );
    Curva* c2 = patch->getCurva ( 1 );
    Curva* c3 = patch->getCurva ( 2 );
    Curva* c4 = patch->getCurva ( 3 );

    // 1. verifica quais curvas ainda não foram discretizadas
    if ( c1->getNumDePontos ( ) ) c1 = NULL; // c1 já foi trabalhada no patch vizinho
    if ( c2->getNumDePontos ( ) ) c2 = NULL; // c2 já foi trabalhada no patch vizinho
    if ( c3->getNumDePontos ( ) ) c3 = NULL; // c3 já foi trabalhada no patch vizinho
    if ( c4->getNumDePontos ( ) ) c4 = NULL; // c4 já foi trabalhada no patch vizinho

    SubMalha* sub = new SubMalha;

    //========================= Malha Grosseira ====================================
    // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região

    for ( double v = 0.0; v <= 1.0; v += 1 )
    {
        for ( double u = 0.0; u <= 1.0; u += 1 )
        {
            Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
            p->id = idManager->next(0);

            if ( v == 0 and c1 ) // p está na curva 1 (c1 = NULL)
                c1-> inserePonto ( p );
            else if ( v == 1 and c3 ) // p está na curva 3
                c3-> inserePonto ( p );

            if ( u == 0 and c4 ) // p está na curva 4
                c4-> inserePonto ( p );
            else if ( u == 1 and c2 ) // p está na curva 2
                c2-> inserePonto ( p );

            sub->insereNoh ( static_cast < Noh* > ( p ) );
        }
    }

    Ponto* p = new Noh ( patch->parametrizar ( 0.5, 0.5 ) );
    p->id = idManager->next(0);
    sub->insereNoh ( static_cast < Noh* > ( p ) );


    Elemento* e1 = new Triangulo (	sub->getNoh ( 0 ),
                                    sub->getNoh ( 1 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e1)->p1 = make_tuple ( 0, 0 );
    ((Triangulo*)e1)->p2 = make_tuple ( 1, 0 );
    ((Triangulo*)e1)->p3 = make_tuple ( 0.5, 0.5 );
    e1->setId (/*idManager->next(1)
*/ idManager->next(1));
    sub->insereElemento ( e1);

    Elemento* e2 = new Triangulo (	sub->getNoh ( 1 ),
                                    sub->getNoh ( 3 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e2)->p1 = make_tuple ( 1, 0 );
    ((Triangulo*)e2)->p2 = make_tuple ( 1, 1 );
    ((Triangulo*)e2)->p3 = make_tuple ( 0.5, 0.5 );
    e2->setId ( /*idManager->next(1)
*/ idManager->next(1));
    sub->insereElemento ( e2);

    Elemento* e3 = new Triangulo (	sub->getNoh ( 3 ),
                                    sub->getNoh ( 2 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e3)->p1 = make_tuple ( 1, 1 );
    ((Triangulo*)e3)->p2 = make_tuple ( 0, 1 );
    ((Triangulo*)e3)->p3 = make_tuple ( 0.5, 0.5 );
    e3->setId ( /*idManager->next(1)
*/ idManager->next(1));
    sub->insereElemento ( e3);

    Elemento* e4 = new Triangulo (	sub->getNoh ( 2 ),
                                    sub->getNoh ( 0 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e4)->p1 = make_tuple ( 0, 1 );
    ((Triangulo*)e4)->p2 = make_tuple ( 0, 0 );
    ((Triangulo*)e4)->p3 = make_tuple ( 0.5, 0.5 );
    e4->setId ( /*idManager->next(1)
*/ idManager->next(1));
    sub->insereElemento ( e4);
    //==============================================================================*/

    // 5. define a submalha do patch
    patch->setMalha ( sub );
    sub->setPatch ( patch );

    return sub;
}

double GeradorAdaptativoPorCurvatura::erroGlobalOmp(Malha *malha)
{
    unsigned int Ns = 0; // número de submalhas
    double Nj = 0.0; // erro global da malha

    Ns = malha->getNumDeSubMalhas ( );

    // Calcula o erro global de cada submalha (OMP)
    Int nThreads = static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads();;

#pragma omp parallel for num_threads(nThreads) firstprivate(Ns) reduction(+ :Nj)
    for ( unsigned int i = 0; i < Ns; ++i )
    {
        SubMalha* sub = malha->getSubMalha ( i );
        unsigned int Nv = sub->getNumDeNos ( );
        double curvPower = 0.0;
        double Njs = 0.0;

        // Calcula o erro relativo para cada nó e soma a Nj
        //#pragma omp parallel for num_threads(nThreads) firstprivate(Ns) reduction(+ :Nj)
        for ( unsigned int j = 0; j < Nv; ++j )
        {
            Ponto *n = sub->getNoh ( j );
            Patch *p = sub->getPatch (  );
            CurvaturaAnalitica ka (	*( static_cast < Noh* > ( n ) ), *( static_cast < CoonsPatch* > ( p ) ) );
            CurvaturaDiscreta kd ( *( static_cast < Noh* > ( n ) ) );
            double Ga = ka.gauss();
            double Gd = kd.gauss();
            double Ha = ka.media();
            double Hd = kd.media();

            //tratamento para erro -nan
            if (std::isnan(Gd)) {
                cout<<"-nan Gd"<<endl;
                Ga = 0.0;
                Gd = 0.0;
            }

            // atualiza as curvaturas do nó ( para que não sejam recalculadas na
            // adaptação das curvas e do domínio )
            ((Noh*)n)->Ga = Ga;
            ((Noh*)n)->Gd = Gd;
            ((Noh*)n)->Ha = Ha;
            ((Noh*)n)->Hd = Hd;

            double power = 0.0;
            double diff = 0.0;

            if ( fabs ( Ga ) >= TOLERANCIA)
            {
                diff = Gd - Ga;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ga, 2);
            }
            else if ( fabs ( Ha ) >= TOLERANCIA )
            {
                diff = Hd - Ha;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ha, 2);
            }
        }

        if (Njs > 0.0 && curvPower > 0.0) {
            Njs =(double) sqrt( Njs / curvPower) / Nv;
        }

        if(!std::isnan(Njs))
            Nj += Njs;
        else
            cout<<"Njs -nan"<<endl;

    } //Parallel for


    Nj /= Ns; // o erro global é a média do erro das submalhas

    return Nj;
}

GeradorAdaptativoPorCurvatura::GeradorAdaptativoPorCurvatura(Modelo &modelo, Timer *timer, int idrange)
{
    this->idManager = NULL;
    this->idoffset = 0;
    this->idrange = idrange;

    Int nThreads = 1;

#if USE_MPI
    Int nProcesses = 1;
    Int rank = 0;
    nProcesses = this->comm->numProcesses();
    rank = this->comm->rank();
#endif //#if USE_MPI

    Geometria* geo = modelo.getGeometria ( );
    int sizePatch = geo->getNumDePatches();

    Malha* malha = new Malha;
    malha->resizeSubmalha(sizePatch);

    this->passo = 0;

    this->comm = new Parallel::TMCommunicator(true);
    nThreads = static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads();

    ptr_aux.resize(nThreads,NULL);

    if (this->idManagers.empty())
    {
        this->idManagers.resize(comm->getMaxThreads(), NULL);
    }

   // nThreads = 1;
#pragma omp parallel num_threads(nThreads) shared(malha, geo, sizePatch)
    {
        Int id = comm->threadId();
        if (!this->idManagers[id])
        {
            this->idManagers[id] = this->makeIdManagerOmp(comm, id);
        }

        // 1. Gera a malha inicial
#pragma omp for
        for (int i = 0; i < sizePatch; ++i )
        {
            CoonsPatch *patch = static_cast < CoonsPatch* > ( geo->getPatch ( i ) );
            SubMalha *sub = this->malhaInicialOmp ( static_cast < CoonsPatch* > ( patch ), this->idManagers[id]);
            malha->insereSubMalha (sub,i);
        }
    }

    // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
    modelo.insereMalha ( malha );

    // 3. Calcula o erro global para a malha inicial
    timer->initTime(7); // Calculo do erro
    this->erro = this->erroGlobalOmp( malha );
    timer->endTime(7); // Calculo do erro


#if USE_PRINT_ERRO
    cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_ERRO

#if USE_SAVE_MESH
    escreveMalha(malha, passo);
#endif //#USE_SAVE_MESH

    this->erro = 1.0;

    // 4. enquanto o erro global de uma malha gerada não for menor que o desejado
    while ( this->erro > EPSYLON )
    {
        if (passo >= 2)
        {
            break;
        }

        this->passo++;

        // 4.1. Aloca uma nova malha
        malha = new Malha;
        malha->resizeSubmalha(sizePatch);

        list<Ponto *> novosPontos[geo->getNumDeCurvas()];

        // map<Ponto *, Ponto *> mapaPontos;

        int sizeCurvas = geo->getNumDeCurvas ( );

       //
        nThreads = 1;
#pragma omp parallel num_threads(nThreads) shared(geo, sizeCurvas, sizePatch, malha, novosPontos)
        {
            Int id = comm->threadId();
            //           this->idManagers[id] = this->makeIdManagerOmp(comm, id);

            // 4.2. Adapta as curvas pela curvatura da curva
#pragma omp for firstprivate(ptr_aux)
            for (int i = 0; i < sizeCurvas; ++i )
            {
                novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaByCurvaOmp( geo->getCurva( i ), this->idManagers[id], 1);
                geo->getCurva(i)->setPontos(novosPontos[i]);
                novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaBySuperficieOmp( geo->getCurva( i ), this->idManagers[id], 1);
                geo->getCurva(i)->setPontos(novosPontos[i]);
                ((CurvaParametrica*)geo->getCurva(i))->ordenaLista ( );
            }

        }

       nThreads = 4;
#pragma omp parallel num_threads(nThreads) shared(geo, sizePatch, malha)
        {
            Int id = comm->threadId();
            //((Performer::RangedIdManager *)this->idManagers[id])->setMin(1,0) ;

            // 4.3. Adapta as patches
#pragma omp for
            for (int i = 0; i < sizePatch; ++i )
            {
                CoonsPatch *p = static_cast < CoonsPatch* > ( geo->getPatch( i ) );
                SubMalha* sub = AdaptadorPorCurvatura::adaptaDominioOmp ( p, this->idManagers[id], this->passo);
                sub->setPatch(p);
                malha->insereSubMalha(sub, i);
            }
        }

        // 4.5. Atualiza os patches
        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
        {
            geo->getPatch( i )->setMalha(malha->getSubMalha( i ));
        }

        // 4.6. Insere a malha gerada no modelo ( que guarda todas as malhas geradas )
        modelo.insereMalha ( malha );

        // 4.7. Escreve um artigo "neutral file" da malha gerada

#if USE_SAVE_MESH
        escreveMalha(malha, passo);
#endif //#USE_SAVE_MESH

        // 4.7. Calcula o erro global para a malha
        timer->initTime(7); // Calculo do erro
        this->erro = this->erroGlobalOmp( malha );
        timer->endTime(7); // Calculo do erro

#if USE_PRINT_ERRO
        cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_COMENT
    }
}

#else

GeradorAdaptativoPorCurvatura::GeradorAdaptativoPorCurvatura (Modelo& modelo , Timer *timer, int idrange)
{
    this->comm = new Parallel::TMCommunicator(false);
    this->idoffset = 0;
    this->idrange = idrange;
    this->idManager = this->makeIdManager(comm, 0);


    CoonsPatch* patch = NULL;
    Geometria* geo = modelo.getGeometria ( );
    Malha* malha = new Malha;
    malha->resizeSubmalha(geo->getNumDePatches());
    ptr_aux.resize(1,NULL);

    this->passo = 0;

    timer->initTimerParallel(0,0, 2); //malha inicial

    // 1. Gera a malha inicial
    for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
    {
        patch = static_cast < CoonsPatch* > ( geo->getPatch ( i ) );
        SubMalha *sub = this->malhaInicial ( static_cast < CoonsPatch* > ( patch ), this->idManager);
        malha->insereSubMalha ( sub, i);
    }

    timer->endTimerParallel(0,0, 2); //malha inicial

    // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
    modelo.insereMalha ( malha );

    timer->endTimerParallel(0, 0, 8); //Overhead
    timer->initTimerParallel(0, 0, 7); // Calculo do erro

    // 3. Calcula o erro global para a malha inicial    
    this->erro = this->erroGlobal ( malha );

    timer->endTimerParallel(0, 0, 7); // Calculo do erro


#if USE_PRINT_ERRO
    cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_ERRO

#if USE_SAVE_MESH
    escreveMalha(malha, passo);
#endif //#USE_SAVE_MESH

    this->erro = 1.0;

    // 4. enquanto o erro global de uma malha gerada não for menor que o desejado
    while ( this->erro > EPSYLON )
    {
        if (passo >= 2)
        {
            break;
        }

        this->passo++;

        // 4.1. Aloca uma nova malha
        malha = new Malha;
        malha->resizeSubmalha(geo->getNumDePatches());

        list<Ponto *> novosPontos[geo->getNumDeCurvas ( )];
        map<Ponto *, Ponto *> mapaPontos;
        int sizeCurvas = geo->getNumDeCurvas( );

        this->idManager = this->makeIdManager(comm, 0);

        timer->initTimerParallel(0, 0, 3); // adaptação das curvas
        //4.2. Adapta as curvas pela curvatura da curva / 4.3. Atualiza a discretização das curvas
        for ( unsigned int i = 0; i < sizeCurvas; ++i )
        {
            novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaByCurva( geo->getCurva( i ), mapaPontos, this->idManager);
            geo->getCurva( i )->setPontos(novosPontos[i]);
            novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaBySuperficie( geo->getCurva( i ), mapaPontos, this->idManager);
            geo->getCurva( i )->setPontos(novosPontos[i]);
        }

        ((Performer::RangedIdManager *)this->idManager)->setMin(1,0);


        timer->endTimerParallel(0, 0, 3); // adaptação das curvas

        //((Performer::RangedIdManager *)this->idManager)->setMin(1,0);

        timer->initTimerParallel(0, 0, 4); // adaptação do domínio
        // 4.4. Adapta as patches
        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
        {
            CoonsPatch *p = static_cast < CoonsPatch* > ( geo->getPatch( i ) );
            SubMalha* sub = AdaptadorPorCurvatura::adaptaDominio ( p, this->idManager, 1);
            sub->setPatch(p);
            malha->insereSubMalha(sub, i);
        }
        timer->endTimerParallel(0, 0, 4); // adaptação do domínio


#if USE_PRINT_COMENT
        cout << "atualizando os patches" << endl;
#endif //#if USE_PRINT_COMENT

        // 4.5. Atualiza os patches
        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
        {
            geo->getPatch( i )->setMalha(malha->getSubMalha( i ));
        }

        // 4.6. Insere a malha gerada no modelo ( que guarda todas as malhas geradas )
        modelo.insereMalha ( malha );

        // 4.7. Escreve um artigo "neutral file" da malha gerada

#if USE_SAVE_MESH
        escreveMalha(malha, passo);
#endif //#USE_SAVE_MESH

        // 4.7. Calcula o erro global para a malha
        timer->initTimerParallel(0, 0, 7); // Calculo do erro
        this->erro = this->erroGlobal ( malha );
        timer->endTimerParallel(0, 0, 7); // Calculo do erro

#if USE_PRINT_ERRO
        cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_COMENT
    }
}

SubMalha* GeradorAdaptativoPorCurvatura::malhaInicial (CoonsPatch* patch, Performer::IdManager *idManager)
{

    Curva* c1 = patch->getCurva ( 0 );
    Curva* c2 = patch->getCurva ( 1 );
    Curva* c3 = patch->getCurva ( 2 );
    Curva* c4 = patch->getCurva ( 3 );


    // 1. verifica quais curvas ainda não foram discretizadas
    if ( c1->getNumDePontos ( ) ) c1 = NULL; // c1 já foi trabalhada no patch vizinho
    if ( c2->getNumDePontos ( ) ) c2 = NULL; // c2 já foi trabalhada no patch vizinho
    if ( c3->getNumDePontos ( ) ) c3 = NULL; // c3 já foi trabalhada no patch vizinho
    if ( c4->getNumDePontos ( ) ) c4 = NULL; // c4 já foi trabalhada no patch vizinho

    SubMalha* sub = new SubMalha;

    //========================= Malha Grosseira ====================================
    // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região
    for ( double v = 0.0; v <= 1.0; v += 1 )
    {
        for ( double u = 0.0; u <= 1.0; u += 1 )
        {
            //			cout << "u = " << u << " v = " << v << endl;
            Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
            p->id = idManager->next(0);

            //			cout << "ponto " << p->id << " " <<  p->x << " " << p->y << " " << p->z << endl;
            //			cout << "====" << endl;

            if ( v == 0 and c1 ) // p está na curva 1 (c1 = NULL)
                c1-> inserePonto ( p );
            else if ( v == 1 and c3 ) // p está na curva 3
                c3-> inserePonto ( p );

            if ( u == 0 and c4 ) // p está na curva 4
                c4-> inserePonto ( p );
            else if ( u == 1 and c2 ) // p está na curva 2
                c2-> inserePonto ( p );

            sub->insereNoh ( static_cast < Noh* > ( p ) );
        }
    }

    Ponto* p = new Noh ( patch->parametrizar ( 0.5, 0.5 ) );
    sub->insereNoh ( static_cast < Noh* > ( p ) );
    p->id = idManager->next(0);

        Elemento* e1 = new Triangulo (	sub->getNoh ( 0 ),
                                            sub->getNoh ( 1 ),
                                            sub->getNoh ( 4 ) );
        ((Triangulo*)e1)->p1 = make_tuple ( 0, 0 );
        ((Triangulo*)e1)->p2 = make_tuple ( 1, 0 );
        ((Triangulo*)e1)->p3 = make_tuple ( 0.5, 0.5 );
        e1->setId ( idManager->next(1));
        sub->insereElemento ( e1 );

        Elemento* e2 = new Triangulo (	sub->getNoh ( 1 ),
                                            sub->getNoh ( 3 ),
                                            sub->getNoh ( 4 ) );
        ((Triangulo*)e2)->p1 = make_tuple ( 1, 0 );
        ((Triangulo*)e2)->p2 = make_tuple ( 1, 1 );
        ((Triangulo*)e2)->p3 = make_tuple ( 0.5, 0.5 );
        e2->setId ( idManager->next(1));
        sub->insereElemento ( e2 );

        Elemento* e3 = new Triangulo (	sub->getNoh ( 3 ),
                                            sub->getNoh ( 2 ),
                                            sub->getNoh ( 4 ) );
        ((Triangulo*)e3)->p1 = make_tuple ( 1, 1 );
        ((Triangulo*)e3)->p2 = make_tuple ( 0, 1 );
        ((Triangulo*)e3)->p3 = make_tuple ( 0.5, 0.5 );
        e3->setId ( idManager->next(1));
        sub->insereElemento ( e3 );

        Elemento* e4 = new Triangulo (	sub->getNoh ( 2 ),
                                            sub->getNoh ( 0 ),
                                            sub->getNoh ( 4 ) );
        ((Triangulo*)e4)->p1 = make_tuple ( 0, 1 );
        ((Triangulo*)e4)->p2 = make_tuple ( 0, 0 );
        ((Triangulo*)e4)->p3 = make_tuple ( 0.5, 0.5 );
        e4->setId ( idManager->next(1));
            sub->insereElemento ( e4 );
    //==============================================================================*/

    // 5. define a submalha do patch
    patch->setMalha ( sub );
    sub->setPatch ( patch );

    return sub;
}

// gera a malha inicial e insere na lista de malhas do modelo
// a lista de pontos da curva é preenchida durante a geração da malha inicial

// calcula o erro global da malha
double GeradorAdaptativoPorCurvatura::erroGlobal (Malha* malha)
{

    unsigned int Ns = 0; // número de submalhas
    unsigned int Nv = 0; // número de vértices
    double Njs = 0; // erro global da submalha
    double curvPower = 0.0;
    double Nj = 0; // erro global da malha
    SubMalha* sub = 0;

    Ns = malha->getNumDeSubMalhas ( );

    // Calcula o erro global de cada submalha
    for ( unsigned int i = 0; i < Ns; ++i )
    {
        sub = malha->getSubMalha ( i );
        Nv = sub->getNumDeNos ( );
        Njs = 0.0;
        curvPower = 0.0;

        // Calcula o erro relativo para cada nó e soma a Nj
        for ( unsigned int j = 0; j < Nv; ++j )
        {
            Ponto *n = sub->getNoh ( j );
            Patch *p = sub->getPatch (  );
            CurvaturaAnalitica ka (	*( static_cast < Noh* > ( n ) ), *( static_cast < CoonsPatch* > ( p ) ) );
            CurvaturaDiscreta kd ( *( static_cast < Noh* > ( n ) ) );
            double Ga = ka.gauss();
            double Gd = kd.gauss();
            double Ha = ka.media();
            double Hd = kd.media();
            // atualiza as curvaturas do nó ( para que não sejam recalculadas na
            // adaptação das curvas e do domínio )
            ((Noh*)n)->Ga = Ga;
            ((Noh*)n)->Gd = Gd;
            ((Noh*)n)->Ha = Ha;
            ((Noh*)n)->Hd = Hd;

            double power = 0.0;
            double diff = 0.0;

            if ( fabs ( Ga ) >= TOLERANCIA )
            {
                diff = Gd - Ga;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ga, 2);
            }
            else if ( fabs ( Ha ) >= TOLERANCIA )
            {
                diff = Hd - Ha;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ha, 2);
            }
        }

        if (Njs > 0.0 && curvPower > 0.0) {
            Njs =(double) sqrt( Njs / curvPower) / Nv;
        }

        Nj += Njs;
    }


    Nj /= Ns; // o erro global é a média do erro das submalhas

    return Nj;
}
#endif //USE_OPENMP

Performer::IdManager *GeradorAdaptativoPorCurvatura::makeIdManager(const Parallel::TMCommunicator *comm, Int id) const
{
    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 2);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);
    manager->setMin(0, /*this->idManager->getId(0)*/ 0 + procOffset + threadOffset);
    manager->setMin(1, /*this->idManager->getId(1)*/ 0 + procOffset + threadOffset);

    return manager;
}

Performer::IdManager *GeradorAdaptativoPorCurvatura::makeIdManagerOmp(const Parallel::TMCommunicator *comm, Int id) const
{
    Int iNoh, iElemet;
    if (this->idManagers[id]) {
        iNoh = this->idManagers[id]->getId(0);
        iElemet = this->idManagers[id]->getId(1);
    } else {
        iNoh = 0;
        iElemet = 0;
    }

    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 2);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);
    manager->setMin(0, iNoh + procOffset + threadOffset);
    manager->setMin(1, iElemet + procOffset + threadOffset);

    return manager;
}

Performer::IdManager *GeradorAdaptativoPorCurvatura::makeIdManagerElementOmp(const Parallel::TMCommunicator *comm, Int id) const
{
    Int iNoh;
    if (this->idManagers[id]) {
        iNoh = this->idManagers[id]->getId(0);
    } else {
        iNoh = 0;
    }

    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 2);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);
    manager->setMin(0, iNoh + procOffset + threadOffset);
    manager->setMin(1, 0 + procOffset + threadOffset);

    return manager;
}

void GeradorAdaptativoPorCurvatura::saveErroMesh(Malha *malha)
{
    cout << "Salvando a Malha com "<< malha->getNumDeSubMalhas ( )<<" subMalhas"<< endl;

    unsigned int Ns = 0; // número de submalhas
    unsigned int Nv = 0; // número de vértices
    double Njs = 0; // erro global da submalha
    double curvPower = 0.0;
    //double Nj = 0; // erro global da malha
    SubMalha* sub = 0;


    // Escreve arquivo com as curvaturas
    stringstream nome;
    nome << passo;
    nome << "erro";
    nome << passo;
    nome << ".txt";

    ofstream arquivo(nome.str().c_str());

    Ns = malha->getNumDeSubMalhas ( );

    // Calcula o erro global de cada submalha
    for ( unsigned int i = 0; i < Ns; ++i )
    {
        sub = malha->getSubMalha ( i );
        Nv = sub->getNumDeNos ( );
        Njs = 0.0;
        curvPower = 0.0;

        // Calcula o erro relativo para cada nó e soma a Nj
        for ( unsigned int j = 0; j < Nv; ++j )
        {
            Ponto *n = sub->getNoh ( j );
            Patch *p = sub->getPatch (  );
            CurvaturaAnalitica ka (	*( static_cast < Noh* > ( n ) ), *( static_cast < CoonsPatch* > ( p ) ) );
            CurvaturaDiscreta kd ( *( static_cast < Noh* > ( n ) ) );
            double Ga = ka.gauss();
            double Gd = kd.gauss();
            double Ha = ka.media();
            double Hd = kd.media();
            // atualiza as curvaturas do nó ( para que não sejam recalculadas na
            // adaptação das curvas e do domínio )
            ((Noh*)n)->Ga = Ga;
            ((Noh*)n)->Gd = Gd;
            ((Noh*)n)->Ha = Ha;
            ((Noh*)n)->Hd = Hd;

            arquivo << "P " << n->id << ": ( " << n->x << ", " << n->y << ", " << n->z << ")" << endl;
            tuple <double, double> t_n = ((HermitePatch*)p)->encontrar_u_v ( *n );
            arquivo << "\tu = " << get<0>(t_n) << " v = " << get<1>(t_n) << endl;
            unsigned int num = ((Noh*)n)->getNumDeElementos();
            arquivo << "\t" << num << " elementos incidentes:";
            for (unsigned int i = 0; i < num; ++i)
            {
                Elemento* elem = ((Noh*)n)->getElemento(i);
                arquivo << " T-" << elem->getId();
            }
            arquivo << endl;
            arquivo << "\tGd = " << ((Noh*)n)->Gd << " Ga = " << ((Noh*)n)->Ga << endl;
            arquivo << "\tHd = " << ((Noh*)n)->Hd << " Ha = " << ((Noh*)n)->Ha << endl;

            double power = 0.0;
            double diff = 0.0;

            if ( fabs ( Ga ) >= TOLERANCIA )
            {
                diff = Gd - Ga;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ga, 2);
                arquivo << "\tCd = " << ((Noh*)n)->Gd << " Ca = " << ((Noh*)n)->Ga << endl;
                arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
                if ( fabs(diff) <= TOLERANCIA )
                    arquivo << "\tdiferença menor que tolerância!!" << endl;
            }
            else if ( fabs ( Ha ) >= TOLERANCIA )
            {
                diff = Hd - Ha;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ha, 2);
                arquivo << "\tCd = " << ((Noh*)n)->Hd << " Ca = " << ((Noh*)n)->Ha << endl;
                arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
                if ( fabs(diff) <= TOLERANCIA )
                    arquivo << "\tdiferença menor que tolerância!!" << endl;
            }

            arquivo << endl;


        }

        if (Njs > 0.0 && curvPower > 0.0) {
            Njs =(double) sqrt( Njs / curvPower) / Nv;
        }
    }

    arquivo.close();

    cout << "Malha salva com sucesso!!!" << endl;
}

void GeradorAdaptativoPorCurvatura::escreveMalha(Malha *malha, int passo)
{
    stringstream nome;
    nome << passo;
    nome << "malha";
    nome << passo;
    nome << ".pos";

    ofstream arq(nome.str().c_str());

    arq << "%HEADER" << endl
        << "Arquivo gerado pelo gerador de malhas de superficie do Daniel Siqueira" << endl << endl;

    arq << "%HEADER.VERSION" << endl
        << "0-005 - Oct/93" << endl << endl
        << "%HEADER.ANALYSIS" << endl
        << "\'shell\'" << endl << endl;


    unsigned long int Nv, Nt;
    Nv = Nt = 0;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++)
    {
        SubMalha *sub = malha->getSubMalha(i);

        Nv += sub->getNumDeNos();
        Nt += sub->getNumDeElementos();
    }

    arq << "%NODE" << endl
        << Nv << endl << endl;

    arq << "%NODE.COORD" << endl
        << Nv << endl;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++)
    {
        SubMalha *sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeNos(); j++)
        {
            Noh *n = sub->getNoh(j);

            arq << n->id << " "
                << n->x << " "
                << n->y << " "
                << n->z << endl;
        }
    }

    arq << endl;

    arq << "%MATERIAL" << endl
        << "1" << endl << endl
        << "%MATERIAL.LABEL" << endl
        << "1" << endl
        << "1\t\'m1\'" << endl << endl
        << "%MATERIAL.ISOTROPIC" << endl
        << "1" << endl
        << "1\t1000.0\t0.0" << endl << endl
        << "%THICKNESS" << endl
        << "1" << endl
        << "1\t1.0" << endl << endl
        << "%INTEGRATION.ORDER" << endl
        << "1" << endl
        << "1\t3\t1\t1\t3\t1\t1" << endl << endl;

    arq << "%ELEMENT" << endl
        << Nt << endl << endl;

    arq << "%ELEMENT.T3" << endl
        << Nt << endl;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++)
    {
        SubMalha *sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeElementos(); j++)
        {
            Triangulo *t = (Triangulo *)sub->getElemento(j);

            arq << t->getId() << " "
                << "1 1 1 "
                << t->getN(1).id << " "
                << t->getN(2).id << " "
                << t->getN(3).id << endl;
        }
    }

    arq << endl;
    arq << "%END";

    arq.flush();

    arq.close();

    //  cout << "escreveu o arquivo para o passo " << passo << endl;
}

void escreveElementos( int passo, SubMalha *sub, int i )
{
    stringstream nome;
    nome << passo;
    nome << "submalha-";
    nome << i;
    nome << ".txt";

    ofstream arq(nome.str().c_str());

    for ( unsigned int k = 0; k < sub->getNumDeElementos(); ++k)
    {
        Elemento *elem = sub->getElemento( k );

        Noh n1 ( elem->getN ( 1 ) );
        Noh n2 ( elem->getN ( 2 ) );
        Noh n3 ( elem->getN ( 3 ) );

        tuple <double, double> t1 = ((Triangulo*)elem)->p1;
        tuple <double, double> t2 = ((Triangulo*)elem)->p2;
        tuple <double, double> t3 = ((Triangulo*)elem)->p3;

        arq	<< "T-" << elem->getId() << ":\n\t"
            << "área = " << elem->getArea() << ";\n\t"
            << "normal = " << elem->getNormal().x << ", " << elem->getNormal().y << ", " << elem->getNormal().z << "\n\t"
            << "n-" << n1.id << "( " << get<0>(t1) << " , " << get<1>(t1) <<  ") ângulo = " << elem->getAngulo( n1 ) << ";\n\t"
            << "n-" << n2.id << "( " << get<0>(t2) << " , " << get<1>(t2) <<  ") ângulo = " << elem->getAngulo( n2 ) << ";\n\t"
            << "n-" << n3.id << "( " << get<0>(t3) << " , " << get<1>(t3) <<  ") ângulo = " << elem->getAngulo( n3 ) << endl;
    }

    arq.flush();

    arq.close();

    cout << "escreveu o arquivo com os elementos da submalha " << i << " para o passo " << passo << endl;
}


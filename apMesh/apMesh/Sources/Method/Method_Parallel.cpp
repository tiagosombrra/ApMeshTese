#include "../../Headers/Method/Method_Parallel.h"

//observação: o uso de variáveis globais pode dar problema
//  em paralelismo de memória compartilhada
static unsigned long int idv1 = 1;
static unsigned long int ide1 = 1;

Method_Parallel::Method_Parallel()
{
}
/*
Method_Parallel::Method_Parallel(int argc, char* argv[])
{
#if USE_OPENMPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &SIZE_MPI);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK_MPI);
    MPI_Status status;

    starttime = 0;
    endtime = 0;

    // limitador de passos em cada processador
    this->passo = 0;

    // divisao do trabalho entre os processadores
    if (RANK_MPI == 0) {

        // estimativa de caga para a distribuiçao nos patches
        geo = new Geometria();
        estimateChargeofPatches();

        malha = new Malha;
        Modelo modelo;

        // 1. Gera a malha inicial
        for (unsigned int i = 0; i < geo->getNumDePatches(); ++i) {
            patch = static_cast<CoonsPatch*>(geo->getPatch(i));
            SubMalha* sub = this->malhaInicial(static_cast<CoonsPatch*>(patch));
            malha->insereSubMalha(sub);
        }

        // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
        modelo.insereMalha(malha);

        // 3. Calcula o erro global para a malha inicial
        this->erroMeshProcess = this->erroGlobal(malha);

        // criação do vetor de curvas para a adapatção
        // createVectorOfCurves();

        // vetor de pontos
        vector<list<Ponto*> > novosPontos;

        map<Ponto*, Ponto*> mapaPontos;

        AdaptadorPorCurvatura::id_noh = 1;
        AdaptadorPorCurvatura::id_ele = 1;

        // controla o erro global da malha de todos os processadores
        // 4. enquanto o erro global de uma malha gerada não for menor que o desejado
        while (erroMeshProcess > EPSYLON) {

            // 4.1. Aloca uma nova malha
            malha = new Malha();

            // 4.2. Adapta as curvas
            startTime();
            //  #pragma omp parallel for private (mapaPontos)
            for (unsigned int i = 0; i < geo->getNumDeCurvas(); i++) {
                list<Ponto*> listPoints =
                    AdaptadorPorCurvatura::adaptaCurva(geo->getCurva(i), mapaPontos);
                //   #pragma omp critical
                novosPontos.push_back(listPoints);
            }

            // 4.3. Atualiza a discretização das curvas
            for (unsigned int i = 0; i < geo->getNumDeCurvas(); ++i) {
                geo->getCurva(i)->setPontos(novosPontos[i]);
            }
            endTime("Adaptaçao das curvas");

            long cont = 0;
            for (unsigned int i = 0; i < novosPontos.size(); i++) {
                list<Ponto*> listPontis = novosPontos.at(i);
                for (std::list<Ponto*>::iterator it = listPontis.begin(); it != listPontis.end(); it++) {
                    (*it)->mostraPonto();
                }
                cont += listPontis.size();
            }
            cout << "lista de pontos >>>>>" << cont << endl;

            erroMeshProcess = 0.0;
        }
    }

    MPI_Finalize();
#endif //#if USE_OPENMPI
}
*/

std::list<BezierPatch*>::iterator Method_Parallel::getIteratorListPatches(int numberPatches)
{
    std::list<BezierPatch*>::iterator it = listBezierPt.begin();
    advance(it, numberPatches);
    return it;
}

int Method_Parallel::execute(int argc, char* argv[], Timer *timer)
{
#if USE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &SIZE_MPI);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK_MPI);
    MPI_Status status;

    malha = new Malha;
    geo = new Geometria();

    if (RANK_MPI == 0) {

        timer->endTime(0); // inicialização process 0

        // estimativa de caga para a distribuiçao nos patches
        estimateChargeofPatches(timer);

        timer->endTime(1); // Estimativa de carga process 0

        timer->initTime(8); // Overhead process 0

        // criação do vetor de curvas para a adapatção
        createVectorOfCurves();

        // distribuição dos patches entre os patches de acordo com suas cargas
        listBezierPt = orderPatchesDistribProcess(listBezierPt);

  
        //estimativa para cada processo
        
        double est0=0, est1=0, est2=0, est3=0, est4=0, est5=0, est6=0, est7=0;
        
        for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++)
        {
            switch ((*it)->getIdProcess()) {
                case 0:{
                    est0 = est0 + (*it)->getNumberTriangle();
                    break;
                }
                case 1:{
                    est1 = est1 + (*it)->getNumberTriangle();
                    break;
                }
                case 2:{
                    est2 = est2 + (*it)->getNumberTriangle();
                    break;
                }
                case 3:{
                    est3 = est3 + (*it)->getNumberTriangle();
                    break;
                }
                case 4:{
                    est4 = est4 + (*it)->getNumberTriangle();
                    break;
                }
                case 5:{
                    est5 = est5 + (*it)->getNumberTriangle();
                    break;
                }
                case 6:{
                    est6 = est6 + (*it)->getNumberTriangle();
                    break;
                }
                case 7:{
                    est7 = est7 + (*it)->getNumberTriangle();
                    break;
                }
                default:
                    cout<<"ERRO>> Número de Processos"<<endl;
                    break;
            }
        }
        
        cout<<"EST0 = "<<est0<<endl;
        cout<<"EST1 = "<<est1<<endl;
        cout<<"EST2 = "<<est2<<endl;
        cout<<"EST3 = "<<est3<<endl;
        cout<<"EST4 = "<<est4<<endl;
        cout<<"EST5 = "<<est5<<endl;
        cout<<"EST6 = "<<est6<<endl;
        cout<<"EST7 = "<<est7<<endl;
        
        double vecBezierPatchesOfProcess0[listBezierPt.size() * 48];
        double vecBezierPatches[listBezierPt.size() * 48];

        timer->endTime(8); // Overhead process 0

        int numberPatches = 0, i = 0, izero = 0;
        
        for (int j = 0; j < SIZE_MPI; j++) {

            timer->initTime(8); // Overhead process 0

            i = 0;

            for (std::list<BezierPatch*>::iterator it = getIteratorListPatches(numberPatches); it != listBezierPt.end();
                 it++) {

                if (j != (*it)->getIdProcess()) {
                    break;
                } else {
                    numberPatches++;
                }

                vecBezierPatches[i] = (*it)->getPt00().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt00().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt00().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt10().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt10().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt10().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt20().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt20().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt20().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt30().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt30().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt30().z;
                i++;

                vecBezierPatches[i] = (*it)->getPt01().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt01().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt01().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt11().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt11().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt11().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt21().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt21().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt21().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt31().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt31().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt31().z;
                i++;

                vecBezierPatches[i] = (*it)->getPt02().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt02().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt02().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt12().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt12().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt12().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt22().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt22().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt22().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt32().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt32().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt32().z;
                i++;

                vecBezierPatches[i] = (*it)->getPt03().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt03().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt03().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt13().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt13().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt13().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt23().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt23().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt23().z;
                i++;
                vecBezierPatches[i] = (*it)->getPt33().x;
                i++;
                vecBezierPatches[i] = (*it)->getPt33().y;
                i++;
                vecBezierPatches[i] = (*it)->getPt33().z;
                i++;
            }

            if (j == 0) {
                izero = i;
                for (int m = 0; m < i; ++m) {
                    vecBezierPatchesOfProcess0[m] = vecBezierPatches[m];
                }
                
            }else {

                timer->endTime(8); // Overhead process 0

                timer->initTime(9); // Timer send and recv process 0
                MPI_Send(&i, 1, MPI_INT, j, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD);
                MPI_Send(&vecBezierPatches, i, MPI_DOUBLE, j, TAG_DOUBLE, MPI_COMM_WORLD);

    timer->endTime(9); // Timer send and recv process 0
            }
        }
        
         timer->endTime(8); // Overhead process 0

        GeradorAdaptativoPorCurvatura adaptiveMesh(vecBezierPatchesOfProcess0, izero, timer);

    } else {

        timer->endTime(0); // Inicialização process n

        timer->initTime(9); // Timer send and recv process n

        int sizePtBezierPatches = 0;
        MPI_Recv(&sizePtBezierPatches, 1, MPI_INT, 0, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD, &status);

        double ptBezierPatches[sizePtBezierPatches];
        MPI_Recv(&ptBezierPatches, sizePtBezierPatches, MPI_DOUBLE, 0, TAG_DOUBLE, MPI_COMM_WORLD, &status);

        timer->endTime(9); // Timer send and recv process n

        GeradorAdaptativoPorCurvatura adaptiveMesh(ptBezierPatches, sizePtBezierPatches, timer);

    }

    //delete BezierPatch::B;
    //delete HermitePatch::H;
    delete timer;

    return MPI_Finalize();
#else
    return 0;
#endif //#if USE_MPI
}

std::list<BezierPatch*> Method_Parallel::orderPatchesDistribProcess(std::list<BezierPatch*> listPatches)
{
#if USE_MPI
    std::vector<std::pair<double, int> > vectorEstimativeProcessPair;
    std::list<BezierPatch*> listPatchesOrder;

    if (SIZE_MPI > 1) {
        for (int i = 0; i < SIZE_MPI; i++) {
            vectorEstimativeProcessPair.push_back(std::make_pair(0.0, i));
        }

        for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {

            std::sort(vectorEstimativeProcessPair.begin(), vectorEstimativeProcessPair.end());

            //			for (std::vector<std::pair<double, int> >::iterator it =
            // vectorEstimativeProcessPair.begin();
            //         it != vectorEstimativeProcessPair.end();
            //         it++) {
            //			 std::cout<<"first: "<<(*it).first<<" second: "<<(*it).second<<std::endl;
            //    }
            (*it)->setIdProcess(vectorEstimativeProcessPair.front().second);
            // std::cout << "idProcess: " << vectorEstimativeProcessPair.front().second << std::endl;

            vectorEstimativeProcessPair.front().first += (*it)->getNumberTriangle();
            // std::cout << "front value: " << vectorEstimativeProcessPair.front().first << std::endl;
        }
 
        for (int i = 0; i < SIZE_MPI; i++) {
            for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {
                if ((*it)->getIdProcess() == i) {
                    listPatchesOrder.push_back((*it));
                }
            }
        }
        return listPatchesOrder;
    } else {
        for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {
            (*it)->setIdProcess(0);
        }
        return listPatches;
    }
#else
    return listPatches;
#endif //#if USE_MPI
}

Method_Parallel::~Method_Parallel()
{
}
void Method_Parallel::createVectorOfCurves()
{
    Curva* curva;

    for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++) {

        if (curves.size() > 0) {
            if (verifyCurve((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30())) {
                curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30());
                curves.push_back(curva);
            }
            if (verifyCurve((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33())) {
                curva = new CurvParamBezier((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33());
                curves.push_back(curva);
            }
            if (verifyCurve((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33())) {
                curva = new CurvParamBezier((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33());
                curves.push_back(curva);
            }
            if (verifyCurve((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03())) {
                curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03());
                curves.push_back(curva);
            }
        } else if (curves.size() == 0) {
            curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30());
            curves.push_back(curva);
            curva = new CurvParamBezier((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33());
            curves.push_back(curva);
            curva = new CurvParamBezier((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33());
            curves.push_back(curva);
            curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03());
            curves.push_back(curva);
        }
    }
}
void Method_Parallel::estimateChargeofPatches(Timer* timer)
{
        cout<<"INIT >> estimative"<< endl;
    ChargeEstimateProcess* cep = new ChargeEstimateProcess();
    listBezierPt = cep->chargeEstimateProcess(geo, timer);
    delete cep;
}
bool Method_Parallel::verifyCurve(Ponto p0, Ponto p1, Ponto p2, Ponto p3)
{
    for (vector<Curva*>::iterator it = this->curves.begin(); it != this->curves.end(); it++) {
        if (static_cast<CurvParamBezier*>(*it)
                ->getP0()
                .operator==(p0) and static_cast<CurvParamBezier*>(*it)
                ->getP1()
                .operator==(p1) and static_cast<CurvParamBezier*>(*it)
                ->getP2()
                .operator==(p2) and static_cast<CurvParamBezier*>(*it)
                ->getP3()
                .operator==(p3)) {
            return false;
        }
    }

    return true;
}
void Method_Parallel::adapterCurves()
{
}

SubMalha* Method_Parallel::malhaInicial(CoonsPatch* patch)
{
    Curva* c1 = patch->getCurva(0);
    Curva* c2 = patch->getCurva(1);
    Curva* c3 = patch->getCurva(2);
    Curva* c4 = patch->getCurva(3);

    // 1. verifica quais curvas ainda nÃ£o foram discretizadas
    if (c1->getNumDePontos())
        c1 = NULL; // c1 jÃ¡ foi trabalhada no patch vizinho
    if (c2->getNumDePontos())
        c2 = NULL; // c2 jÃ¡ foi trabalhada no patch vizinho
    if (c3->getNumDePontos())
        c3 = NULL; // c3 jÃ¡ foi trabalhada no patch vizinho
    if (c4->getNumDePontos())
        c4 = NULL; // c4 jÃ¡ foi trabalhada no patch vizinho

    SubMalha* sub = new SubMalha;

    //============== Define um grid regular no espaÃ§o paramÃ©trico =================

    //    short div = 2;
    //    short div = 4;
    //    short div = 8;
    //    short div = 16;
    //    short div = 32;
    /*float inc = (float) 1.0 / div;

    cout << "insere os pontos nas curvas e na submalha" << endl;
    // Insere os pontos nas curvas e na submalha
    for (double v = 0.0; v <= 1.0; v += inc)
    {
        for (double u = 0.0; u <= 1.0; u += inc)
        {
            Ponto *p = new Noh(patch->parametrizar(u,v));
            p->id = idv1++;

            if ( v == 0 and c1 ) // p estÃ¡ na curva 1
                              c1-> inserePonto ( p );
            else if ( v == 1 and c3 ) // p estÃ¡ na curva 3
                              c3-> inserePonto ( p );

            if ( u == 0 and c4 ) // p estÃ¡ na curva 4
                              c4-> inserePonto ( p );
            else if ( u == 1 and c2 ) // p estÃ¡ na curva 2
                              c2-> inserePonto ( p );

            sub->insereNoh ( static_cast < Noh* > ( p ) );
        }
    }

    cout << "Insere os elementos na submalha" << endl;

    // Insere os elementos na submalha
   short j = 1;
   int k = sub->getNumDeNos() - (div + 1);
   for ( short i = 0; i < k; ++i)
   {
       if ( i + 1 == j*(div + 1) )
       {
           ++j;
           continue;
       }

       Elemento* e1 = new Triangulo (  sub->getNoh ( i ),
                                       sub->getNoh ( i + 1  ),
                                       sub->getNoh ( i + div + 2 ) );
       ((Triangulo*)e1)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i ) ) );
       ((Triangulo*)e1)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + 1 ) ) );
       ((Triangulo*)e1)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + div + 2 ) ) );
       e1->setId ( ide1++ );
       sub->insereElemento ( e1 );

       Elemento* e2 = new Triangulo (  sub->getNoh ( i ),
                                       sub->getNoh ( i + div + 2 ),
                                       sub->getNoh ( i + div + 1 ) );
       ((Triangulo*)e2)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i ) ) );
       ((Triangulo*)e2)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + div + 2 ) ) );
       ((Triangulo*)e2)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + div + 1 ) ) );
       e2->setId ( ide1++ );
       sub->insereElemento ( e2 );
   }
//==============================================================================*/

    /*//=========================== Malha Mais Refinada ==============================

        for ( double v = 0.0; v <= 1.0; v += 0.25 )
        {
            for ( double u = 0.0; u <= 1.0; u += 0.25 )
            {
                //cout << "u = " << u << " v = " << v << endl;
                Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
                p->id = idv1++;

                if ( v == 0 and c1 ) // p estÃ¡ na curva 1
                    c1-> inserePonto ( p );
                else if ( v == 1 and c3 ) // p estÃ¡ na curva 3
                    c3-> inserePonto ( p );

                if ( u == 0 and c4 ) // p estÃ¡ na curva 4
                    c4-> inserePonto ( p );
                else if ( u == 1 and c2 ) // p estÃ¡ na curva 2
                    c2-> inserePonto ( p );

                sub->insereNoh ( static_cast < Noh* > ( p ) );
            }
        }


        for ( double v = 1.0/8.0; v <= 7.0/8.0; v += 1.0/4.0 )
        {
            for ( double u = 1.0/8.0; u <= 7.0/8.0; u += 1.0/4.0 )
            {
                //cout << "u = " << u << " v = " << v << endl;
                Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
                sub->insereNoh ( static_cast < Noh* > ( p ) );
                p->id = idv1++;
            }
        }

        short j = 0;

        for ( short i = 0; i < 19 ; ++i )
        {
            if ( i == 4 or i == 9 or i == 14 )
            {
                ++j;
                ++i;
            }

            Elemento* e1 = new Triangulo (  sub->getNoh ( i ),
                                            sub->getNoh ( i + 1  ),
                                            sub->getNoh ( i + 25 - j ) );
            ((Triangulo*)e1)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i ) ) );
            ((Triangulo*)e1)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + 1 ) ) );
            ((Triangulo*)e1)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
            e1->setId ( ide1++ );
            sub->insereElemento ( e1 );

            Elemento* e2 = new Triangulo (  sub->getNoh ( i + 1 ),
                                            sub->getNoh ( i + 6 ),
                                            sub->getNoh ( i + 25 - j ) );
            ((Triangulo*)e2)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i + 1 ) ) );
            ((Triangulo*)e2)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + 6 ) ) );
            ((Triangulo*)e2)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
            e2->setId ( ide1++ );
            sub->insereElemento ( e2 );

            Elemento* e3 = new Triangulo (  sub->getNoh ( i + 6 ),
                                            sub->getNoh ( i + 5 ),
                                            sub->getNoh ( i + 25 - j ) );
            ((Triangulo*)e3)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i + 6 ) ) );
            ((Triangulo*)e3)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + 5 ) ) );
            ((Triangulo*)e3)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
            e3->setId ( ide1++ );
            sub->insereElemento ( e3 );

            Elemento* e4 = new Triangulo (  sub->getNoh ( i + 5 ),
                                            sub->getNoh ( i ),
                                            sub->getNoh ( i + 25 - j ) );
            ((Triangulo*)e4)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i + 5 ) ) );
            ((Triangulo*)e4)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i ) ) );
            ((Triangulo*)e4)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
            e4->setId ( ide1++ );
            sub->insereElemento ( e4 );
        }
        //==============================================================================*/

    //========================= Malha Grosseira ====================================
    // 2. divide cada patch em 9 regiÃµes e gera os nÃ³s dos extremos de cada regiÃ£o
    for (double v = 0.0; v <= 1.0; v += 1) {
        for (double u = 0.0; u <= 1.0; u += 1) {
            //			cout << "u = " << u << " v = " << v << endl;
            Ponto* p = new Noh(patch->parametrizar(u, v));
            p->id = idv1++;

            //			cout << "ponto " << p->id << " " <<  p->x << " " << p->y << " " << p->z << endl;
            //			cout << "====" << endl;

            if (v == 0 and c1) // p estÃ¡ na curva 1 (c1 = NULL)
                c1->inserePonto(p);
            else if (v == 1 and c3) // p estÃ¡ na curva 3
                c3->inserePonto(p);

            if (u == 0 and c4) // p estÃ¡ na curva 4
                c4->inserePonto(p);
            else if (u == 1 and c2) // p estÃ¡ na curva 2
                c2->inserePonto(p);

            sub->insereNoh(static_cast<Noh*>(p));
        }
    }

    Ponto* p = new Noh(patch->parametrizar(0.5, 0.5));
    sub->insereNoh(static_cast<Noh*>(p));
    p->id = idv1++;
    // cout << p->id << " " <<  p->x << " " << p->y << " " << p->z <<endl;

    Elemento* e1 = new Triangulo(sub->getNoh(0), sub->getNoh(1), sub->getNoh(4));
    ((Triangulo*)e1)->p1 = make_tuple(0, 0);
    ((Triangulo*)e1)->p2 = make_tuple(1, 0);
    ((Triangulo*)e1)->p3 = make_tuple(0.5, 0.5);
    e1->setId(ide1++);
    sub->insereElemento(e1);

    Elemento* e2 = new Triangulo(sub->getNoh(1), sub->getNoh(3), sub->getNoh(4));
    ((Triangulo*)e2)->p1 = make_tuple(1, 0);
    ((Triangulo*)e2)->p2 = make_tuple(1, 1);
    ((Triangulo*)e2)->p3 = make_tuple(0.5, 0.5);
    e2->setId(ide1++);
    sub->insereElemento(e2);

    Elemento* e3 = new Triangulo(sub->getNoh(3), sub->getNoh(2), sub->getNoh(4));
    ((Triangulo*)e3)->p1 = make_tuple(1, 1);
    ((Triangulo*)e3)->p2 = make_tuple(0, 1);
    ((Triangulo*)e3)->p3 = make_tuple(0.5, 0.5);
    e3->setId(ide1++);
    sub->insereElemento(e3);

    Elemento* e4 = new Triangulo(sub->getNoh(2), sub->getNoh(0), sub->getNoh(4));
    ((Triangulo*)e4)->p1 = make_tuple(0, 1);
    ((Triangulo*)e4)->p2 = make_tuple(0, 0);
    ((Triangulo*)e4)->p3 = make_tuple(0.5, 0.5);
    e4->setId(ide1++);
    sub->insereElemento(e4);
    //==============================================================================

    // 5. define a submalha do patch
    patch->setMalha(sub);
    sub->setPatch(patch);

    return sub;
}

double Method_Parallel::erroGlobal(Malha* malha)
{
    cout << "Calculando o erro global=================================" << endl;

    //    para o calculo do tempo real
    //    struct timeval t;
    //    gettimeofday(&t, NULL);
    //    double methodTimeStart = static_cast<double>(t.tv_sec) + static_cast<double>(t.tv_usec)*0.000001;

    unsigned int Ns = 0; // número de submalhas
    unsigned int Nv = 0; // número de vértices
    double Njs = 0;      // erro global da submalha
    double curvPower = 0.0;
    double Nj = 0; // erro global da malha
    SubMalha* sub = 0;

    // Escreve arquivo com as curvaturas
    stringstream nome;
    nome << passo;
    nome << "erro";
    nome << passo;
    nome << ".txt";

    ofstream arquivo(nome.str().c_str());

    Ns = malha->getNumDeSubMalhas();

    //	cout << "Ns = " << Ns << endl;

    // Calcula o erro global de cada submalha

    for (unsigned int i = 0; i < Ns; ++i) {
        //		cout << "for " << i << " to " << Ns-1 << " do\n{" << endl;
        sub = malha->getSubMalha(i);
        Nv = sub->getNumDeNos();
        Njs = 0.0;
        curvPower = 0.0;

        //		cout << "\tNv = " << Nv << endl;
        // Calcula o erro relativo para cada nó e soma a Nj

        for (unsigned int j = 0; j < Nv; ++j) {
            //			cout << "\tfor j = " << j << " to " << Nv-1 << " do\n\t{" << endl;

            Ponto* n = sub->getNoh(j);
            Patch* p = sub->getPatch();

            // APAGAR
            //			cout << "\t\t";
            //			n->mostraPonto();
            /////////////////

            CurvaturaAnalitica ka(*(static_cast<Noh*>(n)), *(static_cast<CoonsPatch*>(p)));

            CurvaturaDiscreta kd(*(static_cast<Noh*>(n)));

            double Ga = ka.gauss();
            double Gd = kd.gauss();
            double Ha = ka.media();
            double Hd = kd.media();

            // APAGAR
            //			cout << "\t\tCurvaturas:\n\t\t\tGa = " << Ga << ", Gd = " << Gd
            //				 << "\n\t\t\tHa = " << Ha << ", Hd = " << Hd << endl;

            // atualiza as curvaturas do nó ( para que não sejam recalculadas na
            // adaptação das curvas e do domínio )
            ((Noh*)n)->Ga = Ga;
            ((Noh*)n)->Gd = Gd;
            ((Noh*)n)->Ha = Ha;
            ((Noh*)n)->Hd = Hd;

            arquivo << "P " << n->id << ": ( " << n->x << ", " << n->y << ", " << n->z << ")" << endl;
            tuple<double, double> t_n = ((HermitePatch*)p)->encontrar_u_v(*n);
            arquivo << "\tu = " << get<0>(t_n) << " v = " << get<1>(t_n) << endl;
            unsigned int num = ((Noh*)n)->getNumDeElementos();
            arquivo << "\t" << num << " elementos incidentes:";

            for (unsigned int i = 0; i < num; ++i) {
                Elemento* elem = ((Noh*)n)->getElemento(i);
                arquivo << " T-" << elem->getId();
            }
            arquivo << endl;
            arquivo << "\tGd = " << ((Noh*)n)->Gd << " Ga = " << ((Noh*)n)->Ga << endl;
            arquivo << "\tHd = " << ((Noh*)n)->Hd << " Ha = " << ((Noh*)n)->Ha << endl;

            double power = 0.0;
            double diff = 0.0;

            if (fabs(Ga) >= TOLERANCIA) {
                diff = Gd - Ga;
                power = pow(diff, 2);
                Njs += power;
                curvPower += pow(Ga, 2);
                arquivo << "\tCd = " << ((Noh*)n)->Gd << " Ca = " << ((Noh*)n)->Ga << endl;
                arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
                if (fabs(diff) <= TOLERANCIA)
                    arquivo << "\tdiferença menor que tolerância!!" << endl;
            } else if (fabs(Ha) >= TOLERANCIA) {
                diff = Hd - Ha;
                power = pow(diff, 2);
                Njs += power;
                curvPower += pow(Ha, 2);
                arquivo << "\tCd = " << ((Noh*)n)->Hd << " Ca = " << ((Noh*)n)->Ha << endl;
                arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
                if (fabs(diff) <= TOLERANCIA)
                    arquivo << "\tdiferença menor que tolerância!!" << endl;
            }

            arquivo << endl;

            //                        cout << "\t\tNjs = " << Njs << endl;
        }

        Njs = (double)sqrt(Njs / curvPower) / Nv;

        //                cout << "\tNjs/Nv = " << Njs << endl;

        Nj += Njs;
    }

    arquivo.close();

    //        cout << "Nj" << " = " << Nj << " / " << Ns << endl;

    Nj /= Ns; // o erro global é a média do erro das submalhas

    // cout << "Nj = " << Nj << endl;

    //    //pra o calculo do tempo real
    //    gettimeofday(&t, NULL);
    //    double methodTimeEnd = static_cast<double>(t.tv_sec) + static_cast<double>(t.tv_usec)*0.000001;
    //    double totalMethodTime = 0;
    //    totalMethodTime += methodTimeEnd - methodTimeStart;

    //    cout <<"time: "<< totalMethodTime <<"==================================================" << endl;

    return Nj;
}

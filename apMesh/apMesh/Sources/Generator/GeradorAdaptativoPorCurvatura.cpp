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


void escreveMalha(Malha *malha, int passo)
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


// gera a malha inicial e insere na lista de malhas do modelo
// a lista de pontos da curva é preenchida durante a geração da malha inicial
SubMalha* GeradorAdaptativoPorCurvatura::malhaInicial ( CoonsPatch* patch )
{

#if USE_PRINT_COMENT
    cout << "Pegando as curvas" << endl;
#endif //#if USE_PRINT_COMENT


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

    //============== Define um grid regular no espaço paramétrico =================

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
            p->id = idv++;

            if ( v == 0 and c1 ) // p está na curva 1
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
       e1->setId ( ide++ );
       sub->insereElemento ( e1 );

       Elemento* e2 = new Triangulo (  sub->getNoh ( i ),
                                       sub->getNoh ( i + div + 2 ),
                                       sub->getNoh ( i + div + 1 ) );
       ((Triangulo*)e2)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i ) ) );
       ((Triangulo*)e2)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + div + 2 ) ) );
       ((Triangulo*)e2)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + div + 1 ) ) );
       e2->setId ( ide++ );
       sub->insereElemento ( e2 );
   }
//==============================================================================*/


    //=========================== Malha Mais Refinada ==============================

    /*   for ( double v = 0.0; v <= 1.0; v += 0.25 )
        {
                for ( double u = 0.0; u <= 1.0; u += 0.25 )
                {
                        //cout << "u = " << u << " v = " << v << endl;
                        Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
                        p->id = idv++;

                        if ( v == 0 and c1 ) // p está na curva 1
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


        for ( double v = 1.0/8.0; v <= 7.0/8.0; v += 1.0/4.0 )
   {
                for ( double u = 1.0/8.0; u <= 7.0/8.0; u += 1.0/4.0 )
                {
                                //cout << "u = " << u << " v = " << v << endl;
                        Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
                        sub->insereNoh ( static_cast < Noh* > ( p ) );
                        p->id = idv++;
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
                ((Triangulo*)e1)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 16 - j ) ) );
                e1->setId ( ide++ );
                sub->insereElemento ( e1 );

                Elemento* e2 = new Triangulo (  sub->getNoh ( i + 1 ),
                                                sub->getNoh ( i + 6 ),
                                                sub->getNoh ( i + 25 - j ) );
                ((Triangulo*)e2)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i + 1 ) ) );
                ((Triangulo*)e2)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + 6 ) ) );
                ((Triangulo*)e2)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
                e2->setId ( ide++ );
                sub->insereElemento ( e2 );

                Elemento* e3 = new Triangulo (  sub->getNoh ( i + 6 ),
                                                sub->getNoh ( i + 5 ),
                                                sub->getNoh ( i + 25 - j ) );
                ((Triangulo*)e3)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i + 6 ) ) );
                ((Triangulo*)e3)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i + 5 ) ) );
                ((Triangulo*)e3)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
                e3->setId ( ide++ );
                sub->insereElemento ( e3 );

                Elemento* e4 = new Triangulo (  sub->getNoh ( i + 5 ),
                                                sub->getNoh ( i ),
                                                sub->getNoh ( i + 25 - j ) );
                ((Triangulo*)e4)->p1 = patch->encontrar_u_v ( *( sub->getNoh ( i + 5 ) ) );
                ((Triangulo*)e4)->p2 = patch->encontrar_u_v ( *( sub->getNoh ( i ) ) );
                ((Triangulo*)e4)->p3 = patch->encontrar_u_v ( *( sub->getNoh ( i + 25 - j ) ) );
                e4->setId ( ide++ );
                sub->insereElemento ( e4 );
        }*/
    //==============================================================================*/

    //========================= Malha Grosseira ====================================
    // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região
    for ( double v = 0.0; v <= 1.0; v += 1 )
    {
        for ( double u = 0.0; u <= 1.0; u += 1 )
        {
            //			cout << "u = " << u << " v = " << v << endl;
            Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
            p->id = idv++;

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
    p->id = idv++;
    //cout << p->id << " " <<  p->x << " " << p->y << " " << p->z <<endl;

    Elemento* e1 = new Triangulo (	sub->getNoh ( 0 ),
                                    sub->getNoh ( 1 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e1)->p1 = make_tuple ( 0, 0 );
    ((Triangulo*)e1)->p2 = make_tuple ( 1, 0 );
    ((Triangulo*)e1)->p3 = make_tuple ( 0.5, 0.5 );
    e1->setId ( ide++ );
    sub->insereElemento ( e1 );

    Elemento* e2 = new Triangulo (	sub->getNoh ( 1 ),
                                    sub->getNoh ( 3 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e2)->p1 = make_tuple ( 1, 0 );
    ((Triangulo*)e2)->p2 = make_tuple ( 1, 1 );
    ((Triangulo*)e2)->p3 = make_tuple ( 0.5, 0.5 );
    e2->setId ( ide++ );
    sub->insereElemento ( e2 );

    Elemento* e3 = new Triangulo (	sub->getNoh ( 3 ),
                                    sub->getNoh ( 2 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e3)->p1 = make_tuple ( 1, 1 );
    ((Triangulo*)e3)->p2 = make_tuple ( 0, 1 );
    ((Triangulo*)e3)->p3 = make_tuple ( 0.5, 0.5 );
    e3->setId ( ide++ );
    sub->insereElemento ( e3 );

    Elemento* e4 = new Triangulo (	sub->getNoh ( 2 ),
                                    sub->getNoh ( 0 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e4)->p1 = make_tuple ( 0, 1 );
    ((Triangulo*)e4)->p2 = make_tuple ( 0, 0 );
    ((Triangulo*)e4)->p3 = make_tuple ( 0.5, 0.5 );
    e4->setId ( ide++ );
    sub->insereElemento ( e4 );
    //==============================================================================*/

    // 5. define a submalha do patch
    patch->setMalha ( sub );
    sub->setPatch ( patch );

    return sub;
}




#if USE_OPENMP
double GeradorAdaptativoPorCurvatura::erroGlobalOmp(Malha *malha)
{
#if USE_PRINT_COMENT
    cout << "Calculando o erro global..." << endl;
#endif //#if USE_PRINT_COMENT

    unsigned int Ns = 0; // número de submalhas
    double Nj = 0.0; // erro global da malha

    Ns = malha->getNumDeSubMalhas ( );

    // Calcula o erro global de cada submalha (OMP)
#pragma omp parallel for num_threads(NUM_THREADS) firstprivate(Ns) reduction(+ :Nj)
    for ( unsigned int i = 0; i < Ns; ++i )
    {
        SubMalha* sub = malha->getSubMalha ( i );
        unsigned int Nv = sub->getNumDeNos ( );
        double curvPower = 0.0;
        double Njs = 0.0;

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

    } //Parallel for


    Nj /= Ns; // o erro global é a média do erro das submalhas

    return Nj;
}
#else
// calcula o erro global da malha
double GeradorAdaptativoPorCurvatura::erroGlobal ( Malha* malha )
{

#if USE_PRINT_COMENT
    cout << "Calculando o erro global..." << endl;
#endif //#if USE_PRINT_COMENT

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
#endif //#if USE_OPENMP

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


GeradorAdaptativoPorCurvatura::GeradorAdaptativoPorCurvatura ( Modelo& modelo )
{
    CoonsPatch* patch = NULL;
    Geometria* geo = modelo.getGeometria ( );
    Malha* malha = new Malha;

    this->passo = 0;

    // 1. Gera a malha inicial
//#pragma omp parallel for num_threads(NUM_THREADS) firstprivate(patch, geo)
    for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
    {

#if USE_PRINT_COMENT
        cout << "gera a malha inicial para patch " << i << endl;
#endif //#if USE_PRINT_COMENT

        patch = static_cast < CoonsPatch* > ( geo->getPatch ( i ) );
        SubMalha *sub = this->malhaInicial ( static_cast < CoonsPatch* > ( patch ) );
//#pragma omp critical
        malha->insereSubMalha ( sub );
    }


    // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
    modelo.insereMalha ( malha );

    // 3. Calcula o erro global para a malha inicial

#if USE_OPENMP
    this->erro = this->erroGlobalOmp( malha );
#else
    this->erro = this->erroGlobal ( malha );
#endif //#USE_OPENMP


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
        if (passo >= 4)
        {
            break;
        }

        this->passo++;

        // 4.1. Aloca uma nova malha
        malha = new Malha;

        list<Ponto *> novosPontos[geo->getNumDeCurvas ( )];
        map<Ponto *, Ponto *> mapaPontos;


        AdaptadorPorCurvatura::id_noh = 1;
        AdaptadorPorCurvatura::id_ele = 1;

        // 4.2. Adapta as curvas
        //for ( unsigned int i = 0; i < geo->getNumDeCurvas ( ); ++i )
        for ( unsigned int i = 0; i < geo->getNumDeCurvas ( ); ++i )
        {

#if USE_PRINT_COMENT
            cout << "adaptando a curva pela curvatura da curva " << i + 1 << endl;
#endif //#if USE_PRINT_COMENT

            novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaByCurva ( geo->getCurva( i ), mapaPontos );
            geo->getCurva( i )->setPontos(novosPontos[i]);
            novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaBySuperficie ( geo->getCurva( i ), mapaPontos );
        }

#if USE_PRINT_COMENT
        cout << "atualizando as curvas" << endl;
#endif //#if USE_PRINT_COMENT

        // 4.3. Atualiza a discretização das curvas
        for ( unsigned int i = 0; i < geo->getNumDeCurvas ( ); ++i )
        {
            geo->getCurva( i )->setPontos(novosPontos[i]);
        }

#if USE_PRINT_COMENT
        cout << "adaptando os patches" << endl;
#endif //#if USE_PRINT_COMENT

        // 4.4. Adapta as patches
        //#pragma omp parallel for
        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
        {
            CoonsPatch *p = static_cast < CoonsPatch* > ( geo->getPatch( i ) );

            SubMalha* sub = AdaptadorPorCurvatura::adaptaDominio ( p, this->passo );

#if USE_PRINT_COMENT
            cout << "submalha tem " << sub->getNumDeElementos () << " elementos" << endl;
#endif //#if USE_PRINT_COMENT

            sub->setPatch(p);

            malha->insereSubMalha(sub);
        }

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
#if USE_OPENMP
        this->erro = this->erroGlobalOmp( malha );
#else
        this->erro = this->erroGlobal ( malha );
#endif //#USE_OPENMP

#if USE_PRINT_ERRO
        cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_COMENT

    }
}

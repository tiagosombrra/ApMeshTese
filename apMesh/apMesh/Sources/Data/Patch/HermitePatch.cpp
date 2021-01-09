/* Classe que define um patch de Hermite
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "CurvParamHermite.h"
#include "HermitePatch.h"

extern double TOLERANCIA; // distância máxima entre dois pontos

Matrix4d iniciaMatrizHermite ( )
{
    Matrix4d m;

    m<<2, -2, 1, 1,
            -3, 3, -2, -1,
            0, 0, 1, 0,
            3, 0, 0, 0;

    return m;
}


void HermitePatch::mostraMatrizes ( )
{

    cout << "U:\n" <<endl;
    cout<<U;

    cout << "V:\n" <<endl;
    cout<<V;

    cout << "H:\n" <<endl;
    cout<<H;

    cout << "Gx:\n" <<endl;
    cout<<Gx;

    cout << "Gy:\n" <<endl;
    cout<<Gy;

    cout << "Gz:\n" <<endl;
    cout<<Gz;

}

// encontra o parâmetro t de um dado ponto p na curva
tuple < double, double > HermitePatch::encontrar_u_v ( const Ponto& p )
{
    unsigned int iMax = 0;

    // chute inicial
    double u_i = 0.5;
    double v_i = 0.5;
    ////////////////

    double delta_u = 0.0; // o quanto o parâmetro terá de percorrer
    double delta_v = 0.0; // o quanto o parâmetro terá de percorrer

    // Método de Jacobi para resolução de sistema
    Ponto p_i;
    //cout << "encontrar_u_v (" << p.id << "), usando Jacobi!";

    do
    {
        Vetor Tu = -(this->Qu(u_i, v_i));
        Vetor Tv = -(this->Qv(u_i, v_i));

        p_i = this->parametrizar ( u_i, v_i ); // palpite inicial

        Matrix3d A;
        A<< Tu.x, Tv.x, p_i.x - p.x,
                Tu.y, Tv.y, p_i.y - p.y,
                Tu.z, Tv.z, p_i.z - p.z;

        int k = 0;
        double pivo = A( 0, 0 );

        while ( fabs(pivo) < TOLERANCIA and k < 3 )
        {
            ++k;
            pivo = A( k, 0 );
        }

        A.row(k).swap(A.row(0));

        if ( fabs(pivo) < TOLERANCIA )
        {
            cout << "Erro! Não é possível encontrar as coordenadas paramétricas no ponto p"
                 << p.id << " (" << p.x << ", " << p.y << ", " << p.z << ")" << endl;

            return make_tuple ( -1.0 , -1.0 );
        }

        double A_10 = A(1,0);
        double A_20 = A(2,0);

        for ( short j = 0; j < 3 ; ++j )
        {
            A(0,j) = static_cast <double > ( A(0,j))/pivo;
            A(1,j) = A(1,j) - A_10*(A(0,j));
            A(2,j) = A(2,j) - A_20*(A(0,j));
        }

        pivo = A( 1,1 );

        if ( fabs(pivo) < TOLERANCIA )
        {
            pivo = A( 2,1 );
            A.row(2).swap(A.row(1));
        }

        double A_01 = A( 0, 1 );
        double A_21 = A( 2, 1 );

        for ( short j = 0; j < 3 ; ++j )
        {
            A(1,j) = static_cast <double > ( A(1,j))/pivo;
            A(0,j) = A(0,j) - A_01*(A(1,j));
            A(2,j) = A(2,j) - A_21*(A(1,j));
        }


        delta_u =  A( 0, 2 );
        delta_v =  A( 1, 2 );

        u_i += delta_u;
        v_i += delta_v;


        if ( ++iMax > 50000 )
        {
            cout << "iMax alcançado!" << endl;
            break;
        }
        //cout << "u = " << u_i << " " << "v = " << v_i << endl;
        //cout << "delta_u = " << delta_u << " " << "delta_v = " << delta_v << endl;
    }
    while ( fabs ( delta_u ) >= TOLERANCIA or fabs ( delta_v ) >= TOLERANCIA );
    //while ( p.distanciaPara(p_i) >= TOLERANCIA );

    if ( u_i <= TOLERANCIA ) u_i = 0.0;
    else if ( u_i >= 1.0 - TOLERANCIA ) u_i = 1.0;

    if ( v_i <= TOLERANCIA ) v_i = 0.0;
    else if ( v_i >= 1.0 - TOLERANCIA ) v_i = 1.0;

    return make_tuple ( u_i, v_i );
}


// faz as multiplicações necessárias para 'parametrizar ( u, v )' e para as
// derivadas parciais
Ponto HermitePatch::calculaPonto_u_v ( )
{
    //  ALOCA um Ponto e o retorna
    //
    Ponto C;

    // C = ( U * ( H * ( G * ( Ht * V ) ) ) )
    C.x =	(	this->getU  ( ) *
                (	//this->getH  ( ) *
                    (	this->getGx ( ) *
                        (//(	this->getH ( ) ).transposta ( ) *
                         this->getV  ( ) ) ) ) )( 0, 0 );

    C.y =	(	this->getU  ( ) *
                (//	this->getH  ( ) *
                 (	this->getGy ( ) *
                    (//(	this->getH ( ) ).transposta ( ) *
                     this->getV  ( ) ) ) ) )( 0, 0 );

    C.z =	(	this->getU  ( ) *
                (//	this->getH  ( ) *
                 (	this->getGz ( ) *
                    (//(	this->getH ( ) ).transposta ( ) *
                     this->getV  ( ) ) ) ) )( 0, 0 );


    //cout << "calculaPonto_u_v () = " << C.x << " " << C.y << " " << C.z << endl;
    return C;
}


// encontra as coordenadas 3D de um ponto p de parâmetros u, v
Ponto HermitePatch::parametrizar ( double u, double v )
{
    //cout << "parametrizar ( " << u << ", " << v << ")" << endl;
    // parametrizar:
    //
    //  -> ALTERA a matriz U
    //  -> ALTERA a matriz V
    //  -> usa 'calculaPonto_u_v ( )'
    //

    this->U( 0, 0) = u*u*u;
    this->U( 0, 1) = u*u;
    this->U( 0, 2) = u;
    this->U( 0, 3) = 1;

    this->V( 0, 0) = v*v*v;
    this->V( 1, 0) = v*v;
    this->V( 2, 0) = v;
    this->V( 3, 0) = 1;


    return calculaPonto_u_v ( );
}


// calcula o vetor tangente na direção u nas coordenadas paramétricas u, v
Vetor HermitePatch::Qu ( double u, double v )
{
    // Qt:
    //
    //  -> ALOCA um Ponto (mas destroi aqui mesmo)
    //	 -> ALOCA um Vetor (retorna ele)
    //  -> ALTERA a matriz U !!!
    //  -> ALTERA a matriz V !!!
    //

    Ponto P;

    this->U( 0, 0) = 3*u*u;
    this->U( 0, 1) = 2*u;
    this->U( 0, 2) = 1;
    this->U( 0, 3) = 0;

    this->V( 0, 0) = v*v*v;
    this->V( 1, 0) = v*v;
    this->V( 2, 0) = v;
    this->V( 3, 0) = 1;


    P = calculaPonto_u_v ( );
    Vetor V ( P );

    return V;
}


// calcula o vetor tangente na direção v nas coordenadas paramétricas u, v
Vetor HermitePatch::Qv ( double u, double v )
{
    // Qt:
    //
    //  -> ALOCA um Ponto (mas destroi aqui mesmo)
    //	 -> ALOCA um Vetor (retorna ele)
    //  -> ALTERA a matriz U !!!
    //  -> ALTERA a matriz V !!!
    //

    Ponto P;

    this->U( 0, 0) = u*u*u;
    this->U( 0, 1) = u*u;
    this->U( 0, 2) = u;
    this->U( 0, 3) = 1;

    this->V( 0, 0) = 3*v*v;
    this->V( 1, 0) = 2*v;
    this->V( 2, 0) = 1;
    this->V( 3, 0) = 0;

    P = calculaPonto_u_v ( );
    Vetor V ( P );

    return V;
}


// calcula a derivada parcial Quu nas coordenadas paramétricas u, v
Vetor HermitePatch::Quu ( double u, double v )
{
    // Qt:
    //
    //  -> ALOCA um Ponto (mas destroi aqui mesmo)
    //	 -> ALOCA um Vetor (retorna ele)
    //  -> ALTERA a matriz U !!!
    //  -> ALTERA a matriz V !!!
    //

    Ponto P;

    this->U( 0, 0) = 6*u;
    this->U( 0, 1) = 2;
    this->U( 0, 2) = 0;
    this->U( 0, 3) = 0;

    this->V( 0, 0) = v*v*v;
    this->V( 1, 0) = v*v;
    this->V( 2, 0) = v;
    this->V( 3, 0) = 1;

    P = calculaPonto_u_v ( );
    Vetor V ( P );

    return V;
}


// calcula a derivada parcial Quv nas coordenadas paramétricas u, v
Vetor HermitePatch::Quv ( double u, double v )
{
    // Qt:
    //
    //  -> ALOCA um Ponto (mas destroi aqui mesmo)
    //	 -> ALOCA um Vetor (retorna ele)
    //  -> ALTERA a matriz U !!!
    //  -> ALTERA a matriz V !!!
    //

    Ponto P;

    this->U( 0, 0) = 3*u*u;
    this->U( 0, 1) = 2*u;
    this->U( 0, 2) = 1;
    this->U( 0, 3) = 0;

    this->V( 0, 0) = 3*v*v;
    this->V( 1, 0) = 2*v;
    this->V( 2, 0) = 1;
    this->V( 3, 0) = 0;

    P = calculaPonto_u_v ( );
    Vetor V ( P );

    return V;
}


// calcula a derivada parcial Qvu nas coordenadas paramétricas u, v
Vetor HermitePatch::Qvu ( double u, double v )
{
    return Quv ( u, v );
}


// calcula a derivada parcial Qvv nas coordenadas paramétricas u, v
Vetor HermitePatch::Qvv ( double u, double v )
{
    // Qt:
    //
    //  -> ALOCA um Ponto (mas destroi aqui mesmo)
    //	 -> ALOCA um Vetor (retorna ele)
    //  -> ALTERA a matriz U !!!
    //  -> ALTERA a matriz V !!!
    //

    Ponto P;

    this->U( 0, 0) = u*u*u;
    this->U( 0, 1) = u*u;
    this->U( 0, 2) = u;
    this->U( 0, 3) = 1;

    this->V( 0, 0) = 6*v;
    this->V( 1, 0) = 2;
    this->V( 2, 0) = 0;
    this->V( 3, 0) = 0;

    P = calculaPonto_u_v ( );
    Vetor V ( P );

    return V;
}


// calcula o vetor tangente na direção u para o ponto p
Vetor HermitePatch::Qu ( const Ponto& p )
{
    tuple < double, double > t = this->encontrar_u_v ( p );
    return this->Qu ( get < 0 > ( t ), get < 1 > ( t ) );
}


// calcula o vetor tangente na direção v para o ponto p
Vetor HermitePatch::Qv ( const Ponto& p )
{
    tuple < double, double > t = this->encontrar_u_v ( p );
    return this->Qv ( get < 0 > ( t ), get < 1 > ( t ) );
}


// calcula a derivada parcial Quu para o ponto p
Vetor HermitePatch::Quu ( const Ponto& p )
{
    tuple < double, double > t = this->encontrar_u_v ( p );
    return this->Quu ( get < 0 > ( t ), get < 1 > ( t ) );
}


// calcula a derivada parcial Quv para o ponto p
Vetor HermitePatch::Quv ( const Ponto& p )
{
    tuple < double, double > t = this->encontrar_u_v ( p );
    return this->Quv ( get < 0 > ( t ), get < 1 > ( t ) );
}


// calcula a derivada parcial Qvu para o ponto p
Vetor HermitePatch::Qvu ( const Ponto& p )
{
    tuple < double, double > t = this->encontrar_u_v ( p );
    return this->Qvu ( get < 0 > ( t ), get < 1 > ( t ) );
}


// calcula a derivada parcial Qvv para o ponto p
Vetor HermitePatch::Qvv ( const Ponto& p )
{
    tuple < double, double > t = this->encontrar_u_v ( p );
    return this->Qvv ( get < 0 > ( t ), get < 1 > ( t ) );
}


// Ordem das curvas:
//		C3
//	C4		C2
//		C1
HermitePatch::HermitePatch ( Curva* C1, Curva* C2, Curva* C3, Curva* C4,
                             Vetor TW_00, Vetor TW_10, Vetor TW_01, Vetor TW_11,
                             bool sinal_c1, bool sinal_c2, bool sinal_c3, bool sinal_c4 ) : CoonsPatch ( )
{
    // As curvas devem ser definidas da esquerda para a direita, de baixo para
    // cima em relação ao Patch !!!

    // 1. Inclui as curvas na lista de curvas de CoonsPatch
    //
    this->curvas.push_back ( C1 );
    this->curvas.push_back ( C2 );
    this->curvas.push_back ( C3 );
    this->curvas.push_back ( C4 );
    //
    //////////////////////////////

    // 2. Coloca o Patch na lista das curvas
    //
    static_cast < CurvParamHermite* > ( C1 )->inserePatch ( this );
    static_cast < CurvParamHermite* > ( C2 )->inserePatch ( this );
    static_cast < CurvParamHermite* > ( C3 )->inserePatch ( this );
    static_cast < CurvParamHermite* > ( C4 )->inserePatch ( this );

    // 3. Seta os atributos de acordo com as curvas
    //
    this->Pt00 = static_cast < CurvParamHermite* > ( C1 )->P0;
    this->Pt01 = static_cast < CurvParamHermite* > ( C3 )->P0;
    this->Pt10 = static_cast < CurvParamHermite* > ( C1 )->P1;
    this->Pt11 = static_cast < CurvParamHermite* > ( C3 )->P1;

    this->Qv00 = static_cast < CurvParamHermite* > ( C4 )->DP0;
    this->Qv01 = static_cast < CurvParamHermite* > ( C4 )->DP1;
    this->Qv10 = static_cast < CurvParamHermite* > ( C2 )->DP0;
    this->Qv11 = static_cast < CurvParamHermite* > ( C2 )->DP1;

    this->Qu00 = static_cast < CurvParamHermite* > ( C1 )->DP0;
    this->Qu01 = static_cast < CurvParamHermite* > ( C3 )->DP0;
    this->Qu10 = static_cast < CurvParamHermite* > ( C1 )->DP1;
    this->Qu11 = static_cast < CurvParamHermite* > ( C3 )->DP1;

    this->Tw00 = TW_00;
    this->Tw01 = TW_01;
    this->Tw10 = TW_10;
    this->Tw11 = TW_11;

    //
    ///////////////////////////////////////

    // 4. Aloca espaço para as matrizes
    //
//    this->U  = new Matriz ( 1, 4, "U" );
//    this->V  = new Matriz ( 4, 1, "V" );
//    this->Gx = new Matriz ( 4, 4, "Gx" );
//    this->Gy = new Matriz ( 4, 4, "Gy" );
//    this->Gz = new Matriz ( 4, 4, "Gz" );
    //
    ////////////////////////////////////

    //
    ////////////////////////////////////////////////////////////////////////////

    // 5. Preenche as matrizes geométricas com G de Hermited
    //
    // Gx:
    // 2x2 superior esquerdo
    Gx( 0, 0) = this->Pt00.x; Gx( 0, 1) = this->Pt01.x;
    Gx( 1, 0) = this->Pt10.x; Gx( 1, 1) = this->Pt11.x;
    // 2x2 inferior esquerdo
    Gx( 2, 0) = this->Qu00.x; Gx( 2, 1) = this->Qu01.x;
    Gx( 3, 0) = this->Qu10.x; Gx( 3, 1) = this->Qu11.x;
    // 2x2 superior direito
    Gx( 0, 2) = this->Qv00.x; Gx( 0, 3) = this->Qv01.x;
    Gx( 1, 2) = this->Qv10.x; Gx( 1, 3) = this->Qv11.x;
    // 2x2 inferior direito
    Gx( 2, 2) = this->Tw00.x; Gx( 2, 3) = this->Tw01.x;
    Gx( 3, 2) = this->Tw10.x; Gx( 3, 3) = this->Tw11.x;
    //
    // Gz:
    // 2x2 superior esquerdo
    Gy( 0, 0) = this->Pt00.y; Gy( 0, 1) = this->Pt01.y;
    Gy( 1, 0) = this->Pt10.y; Gy( 1, 1) = this->Pt11.y;
    // 2x2 inferior esquerdo
    Gy( 2, 0) = this->Qu00.y; Gy( 2, 1) = this->Qu01.y;
    Gy( 3, 0) = this->Qu10.y; Gy( 3, 1) = this->Qu11.y;
    // 2x2 superior direito
    Gy( 0, 2) = this->Qv00.y; Gy( 0, 3) = this->Qv01.y;
    Gy( 1, 2) = this->Qv10.y; Gy( 1, 3) = this->Qv11.y;
    // 2x2 inferior direito
    Gy( 2, 2) = this->Tw00.y; Gy( 2, 3) = this->Tw01.y;
    Gy( 3, 2) = this->Tw10.y; Gy( 3, 3) = this->Tw11.y;
    //
    // Gz:
    // 2x2 superior esquerdo
    Gz( 0, 0) = this->Pt00.z; Gz( 0, 1) = this->Pt01.z;
    Gz( 1, 0) = this->Pt10.z; Gz( 1, 1) = this->Pt11.z;
    // 2x2 inferior esquerdo
    Gz( 2, 0) = this->Qu00.z; Gz( 2, 1) = this->Qu01.z;
    Gz( 3, 0) = this->Qu10.z; Gz( 3, 1) = this->Qu11.z;
    // 2x2 superior direito
    Gz( 0, 2) = this->Qv00.z; Gz( 0, 3) = this->Qv01.z;
    Gz( 1, 2) = this->Qv10.z; Gz( 1, 3) = this->Qv11.z;
    // 2x2 inferior direito
    Gz( 2, 2) = this->Tw00.z; Gz( 2, 3) = this->Tw01.z;
    Gz( 3, 2) = this->Tw10.z; Gz( 3, 3) = this->Tw11.z;
    //
    //////////////

    Matrix4d m = this->getH() * this->getGx() * this->getH().transpose();
    this->Gx = m;

    m = this->getH() * this->getGy() * this->getH().transpose();
    this->Gy = m;

    m = this->getH() * this->getGz() * this->getH().transpose();
    this->Gz = m;
}


// Esse patch acha que é isolado! As curvas que apontam para ele devem ser definidas
// externamente.
/*HermitePatch :: HermitePatch (	Ponto Pt_00, Ponto Pt_01, Vetor Qv_00, Vetor Qv_01,
                                Ponto Pt_10, Ponto Pt_11, Vetor Qv_10, Vetor Qv_11,
                                Vetor Qu_00, Vetor Qu_01, Vetor Tw_00, Vetor Tw_01,
                                Vetor Qu_10, Vetor Qu_11, Vetor Tw_10, Vetor Tw_11,
                                bool sinal_c1, bool sinal_c2, bool sinal_c3, bool sinal_c4 )
: CoonsPatch ( ),	Pt00 (Pt_00), Pt01 (Pt_01), Qv02 (Qv_00), Qv03 (Qv_01),
                    Pt10 (Pt_10), Pt11 (Pt_11), Qv12 (Qv_10), Qv13 (Qv_11),
                    Qu20 (Qu_00), Qu21 (Pt_01), Tw22 (Pt_00), Tw23 (Tw_01),
                    Qu30 (Qu_10), Qu31 (Pt_11), Tw32 (Pt_10), Tw33 (Tw_11)
{
    // 1. Inclui as curvas na lista de curvas de CoonsPatch
    //
    this->curvas.push_back ( new CurvParamHermite ( Pt00, Pt10, Qu00, Qu10 ) );
    this->curvas.push_back ( new CurvParamHermite ( Pt10, Pt11, Qv10, Qv11 ) );
    this->curvas.push_back ( new CurvParamHermite ( Pt01, Pt11, Qu01, Qu11 ) );
    this->curvas.push_back ( new CurvParamHermite ( Pt00, Pt01, Qv00, Qv01 ) );
    //
    //////////////////////////////


    // 2. Aloca espaço para as matrizes
    //
    this->U  = new Matriz ( 1, 4, "U" );
    this->V  = new Matriz ( 4, 1, "V" );
    this->Gx = new Matriz ( 4, 4, "Gx" );
    this->Gy = new Matriz ( 4, 4, "Gy" );
    this->Gz = new Matriz ( 4, 4, "Gz" );
    //
    ////////////////////////////////////

    // 3. Preenche as matrizes geométricas com G de Hermite
    //
    // Gx:
    // 2x2 superior esquerdo
    Gx->setElem( 0, 0, this->Pt00.x ); Gx->setElem( 0, 1, this->Pt01.x );
    Gx->setElem( 1, 0, this->Pt10.x ); Gx->setElem( 1, 1, this->Pt11.x );
    // 2x2 inferior esquerdo
    Gx->setElem( 2, 0, this->Qu00.x ); Gx->setElem( 2, 1, this->Qu01.x );
    Gx->setElem( 3, 0, this->Qu10.x ); Gx->setElem( 3, 1, this->Qu11.x );
    // 2x2 superior direito
    Gx->setElem( 0, 2, this->Qv00.x ); Gx->setElem( 0, 3, this->Qv01.x );
    Gx->setElem( 1, 2, this->Qv10.x ); Gx->setElem( 1, 3, this->Qv11.x );
    // 2x2 inferior direito
    Gx->setElem( 2, 2, this->Tw00.x ); Gx->setElem( 2, 3, this->Tw01.x );
    Gx->setElem( 3, 2, this->Tw10.x ); Gx->setElem( 3, 3, this->Tw11.x );
    //
    // Gy:
    // 2x2 superior esquerdo
    Gy->setElem( 0, 0, this->Pt00.y ); Gy->setElem( 0, 1, this->Pt01.y );
    Gy->setElem( 1, 0, this->Pt10.y ); Gy->setElem( 1, 1, this->Pt11.y );
    // 2x2 inferior esquerdo
    Gy->setElem( 2, 0, this->Qu00.y ); Gy->setElem( 2, 1, this->Qu01.y );
    Gy->setElem( 3, 0, this->Qu10.y ); Gy->setElem( 3, 1, this->Qu11.y );
    // 2x2 superior direito
    Gy->setElem( 0, 2, this->Qv00.y ); Gy->setElem( 0, 3, this->Qv01.y );
    Gy->setElem( 1, 2, this->Qv10.y ); Gy->setElem( 1, 3, this->Qv11.y );
    // 2x2 inferior direito
    Gy->setElem( 2, 2, this->Tw00.y ); Gy->setElem( 2, 3, this->Tw01.y );
    Gy->setElem( 3, 2, this->Tw10.y ); Gy->setElem( 3, 3, this->Tw11.y );
    //
    // Gz:
    // 2x2 superior esquerdo
    Gz->setElem( 0, 0, this->Pt00.z ); Gz->setElem( 0, 1, this->Pt01.z );
    Gz->setElem( 1, 0, this->Pt10.z ); Gz->setElem( 1, 1, this->Pt11.z );
    // 2x2 inferior esquerdo
    Gz->setElem( 2, 0, this->Qu00.z ); Gz->setElem( 2, 1, this->Qu01.z );
    Gz->setElem( 3, 0, this->Qu10.z ); Gz->setElem( 3, 1, this->Qu11.z );
    // 2x2 superior direito
    Gz->setElem( 0, 2, this->Qv00.z ); Gz->setElem( 0, 3, this->Qv01.z );
    Gz->setElem( 1, 2, this->Qv10.z ); Gz->setElem( 1, 3, this->Qv11.z );
    // 2x2 inferior direito
    Gz->setElem( 2, 2, this->Tw00.z ); Gz->setElem( 2, 3, this->Tw01.z );
    Gz->setElem( 3, 2, this->Tw10.z ); Gz->setElem( 3, 3, this->Tw11.z );
    //
    //////////////


    Matriz m = this->getH() * this->getGx() * this->getH();
    delete this->Gx;
    this->Gx = new Matriz(m);

    m = this->getH() * this->getGy() * this->getH();
    delete this->Gy;
    this->Gy = new Matriz(m);

    m = this->getH() * this->getGz() * this->getH();
    delete this->Gz;
    this->Gz = new Matriz(m);
}*/


HermitePatch::HermitePatch ( HermitePatch *antigo ) : CoonsPatch ( antigo )
{
    this->H = iniciaMatrizHermite();
    this->Pt00 = antigo->Pt00;   this->Pt01 = antigo->Pt01;   this->Qv00 = antigo->Qv00;   this->Qv01 = antigo->Qv01;
    this->Pt10 = antigo->Pt10;   this->Pt11 = antigo->Pt11;   this->Qv10 = antigo->Qv10;   this->Qv11 = antigo->Qv11;
    this->Qu00 = antigo->Qu00;   this->Qu01 = antigo->Qu01;   this->Tw00 = antigo->Tw00;   this->Tw01 = antigo->Tw01;
    this->Qu10 = antigo->Qu10;   this->Qu11 = antigo->Qu11;   this->Tw10 = antigo->Tw10;   this->Tw11 = antigo->Tw11;

    this->Gx = antigo->Gx;
    this->Gy = antigo->Gy;
    this->Gz = antigo->Gz;
    this->U = antigo->U;
    this->V = antigo->V;
}


HermitePatch::~HermitePatch (  )
{

    delete &V;
    delete &U;
}

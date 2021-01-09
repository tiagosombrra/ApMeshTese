/* Classe que define uma curva paramétrica de hermite
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

CurvParamHermite::CurvParamHermite (  ) : CurvaParametrica (  )
{
	// 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Hermite
	//
    M<<2, -2, 1, 1,
      -3, 3, -2, -1,
       0, 0, 1, 0,
       1, 0, 0, 0;
	//
	////////////////////////////////////////////////////////////////////////////

	// 2. Preenche as matrizes geométricas com G de Hermite
	//
    Gx<<P0.x,
        P1.x,
        DP0.x,
        DP1.x;

    Gy<<P0.y,
        P1.y,
        DP0.y,
        DP1.y;

    Gz<<P0.z,
        P1.z,
        DP0.z,
        DP1.z;
	//
	///////////////////////////

    Matrix41d m = this->getM() * this->getGx();
    this->Gx = m;

    m = this->getM() * this->getGy();
    this->Gy = m;

	m = this->getM() * this->getGz();
    this->Gz = m;

    delete &M;

	// IMPORTANTE: Ao usar esse construtor, não esquecer de setar os pontos
	// inicial e final e os vetores tangentes inicial e final !!!
}


CurvParamHermite::CurvParamHermite ( Ponto p0, Ponto p1, Vetor dp0, Vetor dp1 )
: CurvaParametrica ( p0, p1 ), DP0( dp0 ), DP1( dp1 )
{
    // 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Hermite
    //
    M<<2, -2, 1, 1,
      -3, 3, -2, -1,
       0, 0, 1, 0,
       1, 0, 0, 0;
    //
    ////////////////////////////////////////////////////////////////////////////

    // 2. Preenche as matrizes geométricas com G de Hermite
    //
    Gx<<P0.x,
        P1.x,
        DP0.x,
        DP1.x;

    Gy<<P0.y,
        P1.y,
        DP0.y,
        DP1.y;

    Gz<<P0.z,
        P1.z,
        DP0.z,
        DP1.z;
    //
    ///////////////////////////

    Matrix41d m = this->getM() * this->getGx();
    this->Gx = m;

    m = this->getM() * this->getGy();
    this->Gy = m;

    m = this->getM() * this->getGz();
    this->Gz = m;


	// 3. Calcula o comprimento da curva
	//
	calcular_L ( );
	//
	///////////////
}


CurvParamHermite::CurvParamHermite ( CurvParamHermite* antiga ) :
CurvaParametrica ( antiga )
{
	this->DP0 = antiga->DP0;
    this->DP1 = antiga->DP1;
}

double CurvParamHermite::calcularCurvatura(double t)
{
    //B'(t)= 3(1-t)²(P1-P0)+6t(1-t)(P2-P1)+3t²(P3-P2)
    double v0 = 3*pow((1-t),2)*(DP0.x-P0.x)+6*t*(1-t)*(DP1.x-DP0.x)+3*pow(t,2)*(P1.x-DP1.x);
    double v1 = 3*pow((1-t),2)*(DP0.y-P0.y)+6*t*(1-t)*(DP1.y-DP0.y)+3*pow(t,2)*(P1.y-DP1.y);
    double v2 = 3*pow((1-t),2)*(P1.z-P0.z)+6*t*(1-t)*(DP1.z-DP0.z)+3*pow(t,2)*(P1.z-DP1.z);

    //B''(t)= 6(1-t)(P2-2P1+P0)+6t(P3-2P2+P1)
    double v00 = 6*(1-t)*(DP1.x-2*DP0.x+P0.x)+6*t*(P1.x-2*DP1.x+DP0.x);
    double v10 = 6*(1-t)*(DP1.y-2*DP0.y+P0.y)+6*t*(P1.y-2*DP1.y+DP0.y);
    double v20 = 6*(1-t)*(DP1.z-2*DP0.z+P0.z)+6*t*(P1.z-2*DP1.z+DP0.z);

    Vetor d1Vetor(v0,v1,v2);
    Vetor d2Vetor(v00,v10,v20);

    double k = ((d1Vetor.operator *(d2Vetor)).modulo())/(pow(d1Vetor.modulo(),3));

    return k;
}


CurvParamHermite::~CurvParamHermite (  )
{
}

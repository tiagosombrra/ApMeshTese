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
	M->setElem( 0, 0, 2 ); M->setElem( 0, 1,-2 ); M->setElem( 0, 2, 1 ); M->setElem( 0, 3, 1 );
	M->setElem( 1, 0,-3 ); M->setElem( 1, 1, 3 ); M->setElem( 1, 2,-2 ); M->setElem( 1, 3,-1 );
	M->setElem( 2, 0, 0 ); M->setElem( 2, 1, 0 ); M->setElem( 2, 2, 1 ); M->setElem( 2, 3, 0 );
	M->setElem( 3, 0, 1 ); M->setElem( 3, 1, 0 ); M->setElem( 3, 2, 0 ); M->setElem( 3, 3, 0 );
	//
	////////////////////////////////////////////////////////////////////////////

	// 2. Preenche as matrizes geométricas com G de Hermite
	//
	Gx->setElem( 0, 0, P0.x );
	Gx->setElem( 1, 0, P1.x );
	Gx->setElem( 2, 0,DP0.x );
	Gx->setElem( 3, 0,DP1.x );

	Gy->setElem( 0, 0, P0.y );
	Gy->setElem( 1, 0, P1.y );
	Gy->setElem( 2, 0,DP0.y );
	Gy->setElem( 3, 0,DP1.y );

	Gz->setElem( 0, 0, P0.z );
	Gz->setElem( 1, 0, P1.z );
	Gz->setElem( 2, 0,DP0.z );
	Gz->setElem( 3, 0,DP1.z );
	//
	///////////////////////////

	Matriz m = this->getM() * this->getGx();
	delete this->Gx;
	this->Gx = new Matriz(m);

	m = this->getM() * this->getGy();
	delete this->Gy;
	this->Gy = new Matriz(m);

	m = this->getM() * this->getGz();
	delete this->Gz;
	this->Gz = new Matriz(m);

	delete this->M;

	this->M = NULL;

	// IMPORTANTE: Ao usar esse construtor, não esquecer de setar os pontos
	// inicial e final e os vetores tangentes inicial e final !!!
}


CurvParamHermite::CurvParamHermite ( Ponto p0, Ponto p1, Vetor dp0, Vetor dp1 )
: CurvaParametrica ( p0, p1 ), DP0( dp0 ), DP1( dp1 )
{
	// 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Hermite
	//
	M->setElem( 0, 0, 2 ); M->setElem( 0, 1,-2 ); M->setElem( 0, 2, 1 ); M->setElem( 0, 3, 1 );
	M->setElem( 1, 0,-3 ); M->setElem( 1, 1, 3 ); M->setElem( 1, 2,-2 ); M->setElem( 1, 3,-1 );
	M->setElem( 2, 0, 0 ); M->setElem( 2, 1, 0 ); M->setElem( 2, 2, 1 ); M->setElem( 2, 3, 0 );
	M->setElem( 3, 0, 1 ); M->setElem( 3, 1, 0 ); M->setElem( 3, 2, 0 ); M->setElem( 3, 3, 0 );
	//
	////////////////////////////////////////////////////////////////////////////

	// 2. Preenche as matrizes geométricas com G de Hermite
	//
	Gx->setElem( 0, 0, p0.x );
	Gx->setElem( 1, 0, p1.x );
	Gx->setElem( 2, 0,dp0.x );
	Gx->setElem( 3, 0,dp1.x );

	Gy->setElem( 0, 0, p0.y );
	Gy->setElem( 1, 0, p1.y );
	Gy->setElem( 2, 0,dp0.y );
	Gy->setElem( 3, 0,dp1.y );

	Gz->setElem( 0, 0, p0.z );
	Gz->setElem( 1, 0, p1.z );
	Gz->setElem( 2, 0,dp0.z );
	Gz->setElem( 3, 0,dp1.z );
	//
	///////////////////////////

	Matriz m = this->getM() * this->getGx();
	delete this->Gx;
	this->Gx = new Matriz(m);

	m = this->getM() * this->getGy();
	delete this->Gy;
	this->Gy = new Matriz(m);

	m = this->getM() * this->getGz();
	delete this->Gz;
	this->Gz = new Matriz(m);

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


CurvParamHermite::~CurvParamHermite (  )
{
}

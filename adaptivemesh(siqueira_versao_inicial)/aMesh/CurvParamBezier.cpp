/* Classe que define uma curva paramétrica de Bezier
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da dissertação de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "CurvParamBezier.h"

CurvParamBezier::CurvParamBezier (  ) : CurvaParametrica (  )
{
	// 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Bezier
	//
	M->setElem( 0, 0,-1 ); M->setElem( 0, 1, 3 ); M->setElem( 0, 2,-3 ); M->setElem( 0, 3, 1 );
	M->setElem( 1, 0, 3 ); M->setElem( 1, 1,-6 ); M->setElem( 1, 2, 3 ); M->setElem( 1, 3, 0 );
	M->setElem( 2, 0,-3 ); M->setElem( 2, 1, 3 ); M->setElem( 2, 2, 0 ); M->setElem( 2, 3, 0 );
	M->setElem( 3, 0, 1 ); M->setElem( 3, 1, 0 ); M->setElem( 3, 2, 0 ); M->setElem( 3, 3, 0 );
	//
	////////////////////////////////////////////////////////////////////////////

	// 2. Preenche as matrizes geométricas com G de Bezier
	//
	Gx->setElem( 0, 0, P0.x );
	Gx->setElem( 1, 0, P1.x );
	Gx->setElem( 2, 0, P2.x );
	Gx->setElem( 3, 0, P3.x );

	Gy->setElem( 0, 0, P0.y );
	Gy->setElem( 1, 0, P1.y );
	Gy->setElem( 2, 0, P2.y );
	Gy->setElem( 3, 0, P3.y );

	Gz->setElem( 0, 0, P0.z );
	Gz->setElem( 1, 0, P1.z );
	Gz->setElem( 2, 0, P2.z );
	Gz->setElem( 3, 0, P3.z );
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


CurvParamBezier::CurvParamBezier ( Ponto p0, Ponto p1, Ponto p2, Ponto p3 )
: CurvaParametrica ( p0, p1 ), P2( p2 ), P3( p3 )
{
	// 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Bezier
	//
	M->setElem( 0, 0,-1 ); M->setElem( 0, 1, 3 ); M->setElem( 0, 2,-3 ); M->setElem( 0, 3, 1 );
	M->setElem( 1, 0, 3 ); M->setElem( 1, 1,-6 ); M->setElem( 1, 2, 3 ); M->setElem( 1, 3, 0 );
	M->setElem( 2, 0,-3 ); M->setElem( 2, 1, 3 ); M->setElem( 2, 2, 0 ); M->setElem( 2, 3, 0 );
	M->setElem( 3, 0, 1 ); M->setElem( 3, 1, 0 ); M->setElem( 3, 2, 0 ); M->setElem( 3, 3, 0 );
	//
	////////////////////////////////////////////////////////////////////////////

	// 2. Preenche as matrizes geométricas com G de Bezier
	//
	Gx->setElem( 0, 0, P0.x );
	Gx->setElem( 1, 0, P1.x );
	Gx->setElem( 2, 0, P2.x );
	Gx->setElem( 3, 0, P3.x );

	Gy->setElem( 0, 0, P0.y );
	Gy->setElem( 1, 0, P1.y );
	Gy->setElem( 2, 0, P2.y );
	Gy->setElem( 3, 0, P3.y );

	Gz->setElem( 0, 0, P0.z );
	Gz->setElem( 1, 0, P1.z );
	Gz->setElem( 2, 0, P2.z );
	Gz->setElem( 3, 0, P3.z );
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


CurvParamBezier::CurvParamBezier ( CurvParamBezier* antiga ) :
CurvaParametrica ( antiga )
{
	this->P2 = antiga->P2;
	this->P3 = antiga->P3;
}


CurvParamBezier::~CurvParamBezier (  )
{
}

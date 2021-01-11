/* Classe encarregada de fazer a adaptação das malhas baseando-se nas curvaturas
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef AdaptadorPorCurvatura_h
#define AdaptadorPorCurvatura_h

#include <cmath>
#include <map>

#include "Adaptador.h"
#include "CoonsPatch.h"
#include "SubMalha.h"
#include "CurvaParametrica.h"
#include "CurvaturaAnalitica.h"
#include "CurvaturaDiscreta.h"
#include "BinTree.h"
#include "AdvancingFront.h"
#include "Boundary.h"
#include "Face.h"
#include "Vertex.h"
#include "Definitions.h"

class AdaptadorPorCurvatura : public Adaptador
{
	static unsigned long int id_noh;
	static unsigned long int id_ele;
    static list < Ponto* > adaptaCurvaByCurva (Curva*, map < Ponto*, Ponto* > &, double fator_dis = 1.0 );
    static list < Ponto* > adaptaCurvaBySuperficie ( Curva* c,  map < Ponto*, Ponto* > &, double fator_dis = 1.0 );

	static tuple <double, double> curvatura ( CurvaturaAnalitica& ka, CurvaturaDiscreta& kd );
	static double novoTamanho ( const double, const double, const double, const double );
	static void adaptaCurvas ( CoonsPatch& patch, const unsigned int );
    static SubMalha* adaptaDominio (CoonsPatch* patch, double fator_dis = 1.0 );
	virtual void fazNada ()=0;

	friend class GeradorAdaptativoPorCurvatura;
};

#endif

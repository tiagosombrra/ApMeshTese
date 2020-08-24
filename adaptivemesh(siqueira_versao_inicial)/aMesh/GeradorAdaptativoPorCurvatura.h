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

#ifndef GeradorAdaptativoPorCurvatura_h
#define GeradorAdaptativoPorCurvatura_h

#include <iostream>
#include <list>
using namespace std;

#include "Noh.h"
#include "Triangulo.h"
#include "CoonsPatch.h"
#include "GeradorAdaptativo.h"
#include "CurvaturaAnalitica.h"
#include "CurvaturaDiscreta.h"
#include "AdaptadorPorCurvatura.h"

class GeradorAdaptativoPorCurvatura : public GeradorAdaptativo
{
		
		// gera a malha inicial e insere na lista de malhas do modelo
		// a lista de pontos da curva é preenchida durante a geração
		virtual SubMalha* malhaInicial ( CoonsPatch* );
		virtual double erroGlobal ( Malha* malha );
	
	public :
		GeradorAdaptativoPorCurvatura ( Modelo& );
};

#endif

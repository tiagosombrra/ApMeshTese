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
#include <sstream>
#include <fstream>
#include <string>
#include <omp.h>
#include "../Data/Noh.h"
#include "../Data/Triangulo.h"
#include "../Data/Patch/CoonsPatch.h"
#include "GeradorAdaptativo.h"
#include "../Curvature/CurvaturaAnalitica.h"
#include "../Curvature/CurvaturaDiscreta.h"
#include "../Adapter/AdaptadorPorCurvatura.h"
#include "../Basic/Definitions.h"

using namespace std;

static unsigned long int idv = 1;
static unsigned long int ide = 1;
extern double TOLERANCIA;
extern double TOLERANCIA_CURVATURA;
extern int NUM_THREADS;

class GeradorAdaptativoPorCurvatura : public GeradorAdaptativo
{

    // gera a malha inicial e insere na lista de malhas do modelo
    // a lista de pontos da curva é preenchida durante a geração
    virtual SubMalha* malhaInicial ( CoonsPatch* );
    void saveErroMesh(Malha *malha);

#if USE_OPENMP
    virtual double erroGlobalOmp ( Malha* malha );
#else
    virtual double erroGlobal ( Malha* malha );
#endif //#USE_OPENMP

public :
    GeradorAdaptativoPorCurvatura ( Modelo& );
};

#endif

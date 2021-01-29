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
#include "../Data/Definitions.h"
#include "../Timer/Timer.h"

#include "../Crabmesh/Performer/RangedIdManager.h"
#include "../Parallel/TMCommunicator.h"

using namespace std;
using namespace Data;

extern double TOLERANCIA;
extern double TOLERANCIA_CURVATURA;
extern int NUM_THREADS;

class GeradorAdaptativoPorCurvatura : public GeradorAdaptativo
{
public :
    // gera a malha inicial e insere na lista de malhas do modelo
    // a lista de pontos da curva é preenchida durante a geração

    void saveErroMesh(Malha *malha);

#if USE_OPENMP
    virtual SubMalha* malhaInicialOmp (CoonsPatch*, Performer::IdManager *idManager);
    virtual double erroGlobalOmp ( Malha* malha);
    GeradorAdaptativoPorCurvatura (Modelo &modelo, Timer *timer, int idrange = 0);
#else
    GeradorAdaptativoPorCurvatura ( Modelo &modelo, Timer *timer, int idrange = 0);
#endif //#USE_OPENMP

    virtual SubMalha* malhaInicial (CoonsPatch*, Performer::IdManager *idManager);
    virtual double erroGlobal ( Malha* malha);

    Performer::IdManager *makeIdManager(const Parallel::TMCommunicator *comm, Int id) const;
    Performer::IdManager *makeIdManagerElement(const Parallel::TMCommunicator *comm, Int id) const;
    Performer::IdManager *makeIdManagerElementOmp(const Parallel::TMCommunicator *comm, Int id) const;
protected:
    Parallel::TMCommunicator *comm;
    Performer::IdManager *idManager;
    Performer::IdManagerVector idManagers;
    mutable ULInt idoffset;
    ULInt idrange;
};

#endif

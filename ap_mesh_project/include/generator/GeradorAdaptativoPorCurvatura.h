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

#include <omp.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "../adapter/adapter.h"
#include "../crab_mesh/performer/ranged_id_manager.h"
#include "../curvature/curvature_analytical.h"
#include "../curvature/curvature_discrete.h"
#include "../data/Definitions.h"
#include "../data/Noh.h"
#include "../data/Triangulo.h"
#include "../data/patch/BezierPatch.h"
#include "../data/patch/CoonsPatch.h"
#include "../estimate/ChargeEstimateProcess.h"
#include "../input_output/Modelos3d.h"
#include "../input_output/ReaderPatches.h"
#include "../parallel/ApMeshCommunicator.h"
#include "../timer/Timer.h"
#include "GeradorAdaptativo.h"

using namespace std;
using namespace Data;

extern double TOLERANCIA;
extern double TOLERANCIA_CURVATURA;
extern int NUM_THREADS;
extern double DISCRETIZACAO_CURVA;
extern vector<CurveAdaptiveParametric *> ptr_aux;
extern int PASSOS;
extern std::string WRITE_MESH;
extern int RANK_MPI;
extern int SIZE_MPI;

class GeradorAdaptativoPorCurvatura : public GeradorAdaptativo {
 public:
  // gera a malha inicial e insere na lista de malhas do modelo
  // a lista de pontos da curva é preenchida durante a geração
  typedef std::vector<std::pair<int, Malha *> > MeshVector;
  typedef std::vector<std::pair<int, Malha *> > ErroMeshVector;

  GeradorAdaptativoPorCurvatura();

#if USE_MPI
  int execute(int argc, char *argv[], Timer *timer, MPI_Status status);
  std::list<BezierPatch *> estimateChargeofPatches(Geometria *geometria,
                                                   Timer *timer,
                                                   std::string INPUT_MODEL);
  std::vector<CurveAdaptive *> createVectorOfCurves(
      std::list<BezierPatch *> listBezierPt);
  std::list<BezierPatch *> orderPatchesDistribProcess(
      std::list<BezierPatch *> listPatches);
  bool verifyCurve(Ponto p0, Ponto p1, Ponto p2, Ponto p3,
                   std::vector<CurveAdaptive *> curves);
  void calculateEstimateProcessElements(int sizeProcess,
                                        std::list<BezierPatch *> listBezierPt);
  std::list<BezierPatch *>::iterator getIteratorListPatches(
      int numberPatches, std::list<BezierPatch *> listBezierPt);
  void generator(double listOfPatches[], int sizeOfListPatches, Timer *timer,
                 int idrange = 1024, int sizeRank = 1, int sizeThread = 1);
  Geometria *unpakGeometry(double listOfPatches[], int sizeOfListPatches);
#else
  int execute(int argc, char *argv[], Timer *timer);
  void generator(Modelo &modelo, Timer *timer, int idrange = 1024,
                 int sizeRank = 1, int sizeThread = 1);
#endif

  virtual SubMalha *malhaInicial(CoonsPatch *, Performer::IdManager *idManager);
  virtual double erroGlobal(Malha *malha, Timer *timer, int rank = 0,
                            int sizeThread = 0);
  Performer::IdManager *makeIdManager(const Parallel::TMCommunicator *comm,
                                      Int id) const;
  Performer::IdManager *makeIdManagerOmp(const Parallel::TMCommunicator *comm,
                                         Int id) const;
  Performer::IdManager *makeIdManagerElementOmp(
      const Parallel::TMCommunicator *comm, Int id) const;
  void escreveMalha(Malha *malha, int passo);
  void escreveMalha(Malha *malha, int passo, vector<double> erroPasso,
                    int rank = -1);
  void writeQualityMesh(Malha *malha, int passo, vector<double> erroPasso,
                        int rank = -1);
  void salvarErroMalha(Malha *malha);
  void salvaMalha(Malha *malha, int passo);
  void salvaErroMalha(Malha *malha, int passo);
  void adaptCurve(Geometria *geo);
  void adaptDomain(Geometria *geo, Malha *malha);
  void generatorInitialMesh(Geometria *geo, Malha *malha, Timer *timer,
                            int sizeThread, int sizePatch);
  void printElments(Malha *malha, int passo, vector<double> erroPasso,
                    int rank = -1);

#if USE_OPENMP
  virtual SubMalha *malhaInicialOmp(CoonsPatch *,
                                    Performer::IdManager *idManager);
  virtual double erroGlobalOmp(Malha *malha, Timer *timer, int rank = 0,
                               int sizeThread = 0);
  int generatorOmp(Modelo &modelo, Timer *timer, int idrange = 0,
                   int sizeRank = 1, int sizeThread = 1);
  void adaptCurveOmp(Geometria *geo);
  void adaptDomainOmp(Geometria *geo, Malha *malha, Timer *timer,
                      int sizeThread, int sizePatch);
#endif  // #USE_OPENMP

#if USE_MPI
  Modelo modelo;
  Geometria *geo;
  CoonsPatch *patch;
  Malha *malha;
#endif  // USE_MPI

 protected:
#if (USE_MPI || USE_OPENMP)
  ApMeshCommunicator *comm;
#else
  Parallel::TMCommunicator *comm;
#endif  // USE_MPI

  Performer::IdManager *idManager;
  Performer::IdManagerVector idManagers;
  mutable ULInt idoffset;
  ULInt idrange;

 private:
  MeshVector saveMesh;
  ErroMeshVector saveErroMesh;
  vector<double> erroPasso;
  Adapter adapter;
};
#endif

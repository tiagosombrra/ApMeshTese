/* Classe encarregada de fazer a geração adaptativa das malhas de acordo com as
curvaturas.
*/
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
#include "../data/definitions.h"
#include "../data/noh.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_coons.h"
#include "../data/triangle_adaptive.h"
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
  typedef std::vector<std::pair<int, MeshAdaptive *> > MeshVector;
  typedef std::vector<std::pair<int, MeshAdaptive *> > ErroMeshVector;

  GeradorAdaptativoPorCurvatura();

#if USE_MPI
  int execute(int argc, char *argv[], Timer *timer, MPI_Status status);
  std::list<PatchBezier *> estimateChargeofPatches(Geometry *geometria,
                                                   Timer *timer,
                                                   std::string INPUT_MODEL);
  std::vector<CurveAdaptive *> createVectorOfCurves(
      std::list<PatchBezier *> listBezierPt);
  std::list<PatchBezier *> orderPatchesDistribProcess(
      std::list<PatchBezier *> listPatches);
  bool verifyCurve(PointAdaptive p0, PointAdaptive p1, PointAdaptive p2,
                   PointAdaptive p3, std::vector<CurveAdaptive *> curves);
  void calculateEstimateProcessElements(int sizeProcess,
                                        std::list<PatchBezier *> listBezierPt);
  std::list<PatchBezier *>::iterator getIteratorListPatches(
      int numberPatches, std::list<PatchBezier *> listBezierPt);
  void generator(double listOfPatches[], int sizeOfListPatches, Timer *timer,
                 int idrange = 1024, int sizeRank = 1, int sizeThread = 1);
  Geometry *unpakGeometry(double listOfPatches[], int sizeOfListPatches);
#else
  int execute(int argc, char *argv[], Timer *timer);
  void generator(Modelo &modelo, Timer *timer, int idrange = 1024,
                 int sizeRank = 1, int sizeThread = 1);
#endif

  virtual SubMesh *malhaInicial(PatchCoons *, Performer::IdManager *idManager);
  virtual double erroGlobal(MeshAdaptive *malha, Timer *timer, int rank = 0,
                            int sizeThread = 0);
  Performer::IdManager *makeIdManager(const Parallel::TMCommunicator *comm,
                                      Int id) const;
  Performer::IdManager *makeIdManagerOmp(const Parallel::TMCommunicator *comm,
                                         Int id) const;
  Performer::IdManager *makeIdManagerElementOmp(
      const Parallel::TMCommunicator *comm, Int id) const;
  void escreveMalha(MeshAdaptive *malha, int passo);
  void escreveMalha(MeshAdaptive *malha, int passo, vector<double> erroPasso,
                    int rank = -1);
  void writeQualityMesh(MeshAdaptive *malha, int passo,
                        vector<double> erroPasso, int rank = -1);
  void salvarErroMalha(MeshAdaptive *malha);
  void salvaMalha(MeshAdaptive *malha, int passo);
  void salvaErroMalha(MeshAdaptive *malha, int passo);
  void adaptCurve(Geometry *geo);
  void adaptDomain(Geometry *geo, MeshAdaptive *malha);
  void generatorInitialMesh(Geometry *geo, MeshAdaptive *malha, Timer *timer,
                            int sizeThread, int sizePatch);
  void printElments(MeshAdaptive *malha, int passo, vector<double> erroPasso,
                    int rank = -1);

#if USE_OPENMP
  virtual SubMesh *malhaInicialOmp(PatchCoons *,
                                   Performer::IdManager *idManager);
  virtual double erroGlobalOmp(MeshAdaptive *malha, Timer *timer, int rank = 0,
                               int sizeThread = 0);
  int generatorOmp(Model &modelo, Timer *timer, int idrange = 0,
                   int sizeRank = 1, int sizeThread = 1);
  void adaptCurveOmp(Geometry *geo);
  void adaptDomainOmp(Geometry *geo, MeshAdaptive *malha, Timer *timer,
                      int sizeThread, int sizePatch);
#endif  // #USE_OPENMP

#if USE_MPI
  Model modelo;
  Geometry *geo;
  PatchCoons *patch;
  MeshAdaptive *malha;
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

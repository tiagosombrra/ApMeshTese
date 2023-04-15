#ifndef GENERATOR_GENERATOR_ADAPTIVE_H
#define GENERATOR_GENERATOR_ADAPTIVE_H

#include <omp.h>

#include <list>
#include <string>

#include "../adapter/adapter.h"
#include "../crab_mesh/performer/ranged_id_manager.h"
#include "../curvature/curvature_analytical.h"
#include "../curvature/curvature_discrete.h"
#include "../data/definitions.h"
#include "../data/geometry.h"
#include "../data/model.h"
#include "../data/node_adaptive.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_coons.h"
#include "../data/triangle_adaptive.h"
#include "../estimate/charge_estimate_process.h"
#include "../input_output/models_3d.h"
#include "../input_output/patch_reader.h"
#include "../parallel/ApMeshCommunicator.h"
#include "../timer/timer.h"

using namespace Data;

extern double TOLERANCE;
extern double TOLERANCE_CURVATURE;
extern int NUM_THREADS;
extern double DISCRETIZATION_CURVE_FACTOR;
extern int STEPS;
extern std::string WRITE_MESH;
extern int RANK_MPI;
extern int SIZE_MPI;
extern double TOLERANCE;
extern double EPSYLON;
extern vector<CurveAdaptiveParametric *> ptr_aux;

class GeneratorAdaptive {
 public:
  // gera a malha inicial e insere na lista de malhas do modelo
  // a lista de pontos da curva é preenchida durante a geração
  typedef std::vector<std::pair<int, MeshAdaptive *> > MeshVector;
  typedef std::vector<std::pair<int, MeshAdaptive *> > ErrorMeshVector;

  GeneratorAdaptive();
  ~GeneratorAdaptive() = default;

#if USE_MPI
  int Execute(char *argv[], Timer &timer, MPI_Status status);
  std::list<PatchBezier *> EstimateChargeofPatches(Geometry *geometry,
                                                   Timer &timer);
  std::vector<CurveAdaptive *> CreateVectorOfCurves(
      std::list<PatchBezier *> patches);
  std::list<PatchBezier *> OrderPatchesDistribProcess(
      std::list<PatchBezier *> patches);
  bool VerifyCurve(PointAdaptive p0, PointAdaptive p1, PointAdaptive p2,
                   PointAdaptive p3, std::vector<CurveAdaptive *> curves);
  void CalculateEstimateProcessElements(int size_process,
                                        std::list<PatchBezier *> patches);
  std::list<PatchBezier *>::iterator GetIteratorListPatches(
      int size_patches, std::list<PatchBezier *> patches);
  void Generator(double patches[], int size_patches, Timer &timer,
                 int id_range = 1024, int size_rank = 1, int size_thread = 1);
  Geometry *UnpakGeometry(double patches[], int size_patches);
#else
  int Execute(char *argv[], Timer &timer);
  void Generator(Model &model, Timer &timer, int id_range = 1024,
                 int size_rank = 1, int size_thread = 1);
#endif

  SubMesh *InitialMesh(PatchCoons *, Performer::IdManager *id_manager);
  double ErrorGlobal(MeshAdaptive *mesh, Timer &timer, int rank = 0,
                     int size_thread = 0);
  Performer::IdManager *MakeIdManager(const Parallel::TMCommunicator *comm,
                                      Int id) const;
  Performer::IdManager *MakeIdManagerOmp(const Parallel::TMCommunicator *comm,
                                         Int id) const;
  Performer::IdManager *MakeIdManagerElementOmp(
      const Parallel::TMCommunicator *comm, Int id) const;

  void WriteMesh(MeshAdaptive *mesh, int step);
  void WriteMesh(MeshAdaptive *mesh, int step, vector<double> error_step,
                 int rank = -1);
  void WriteQualityMesh(MeshAdaptive *mesh, int step, vector<double> error_step,
                        int rank = -1);
  void SaveErrorMesh(MeshAdaptive *mesh);
  void SaveMesh(MeshAdaptive *mesh, int step);
  void SaveErrorMesh(MeshAdaptive *mesh, int step);
  void AdaptCurve(Geometry &geometry);
  void AdaptDomain(Geometry &geometry, MeshAdaptive *mesh);
  void GeneratorInitialMesh(Geometry &geometry, MeshAdaptive *mesh,
                            Timer &timer, int size_thread, int size_patch);
  void PrintElments(MeshAdaptive *mesh, int step, vector<double> error_step,
                    int rank = -1);

#if USE_OPENMP
  virtual SubMesh *GeneratorInitialMeshOmp(PatchCoons *,
                                           Performer::IdManager *id_Manager);
  virtual double CalculateErrorGlobalOmp(MeshAdaptive *mesh, Timer &timer,
                                         int rank = 0, int size_thread = 0);
  int GeneratorOmp(Model &model, Timer &timer, int id_range = 0,
                   int sizeRank = 1, int sizeThread = 1);
  void AdaptCurveOmp(Geometry *geometry);
  void AdaptDomainOmp(Geometry *geometry, MeshAdaptive *mesh, Timer &timer,
                      int size_thread, int size_patch);
#endif  // #USE_OPENMP

#if USE_MPI
  Model model_;
  Geometry *geometry_;
  PatchCoons *patch_;
#endif  // USE_MPI

  MeshAdaptive *mesh_;

 protected:
#if (USE_MPI || USE_OPENMP)
  ApMeshCommunicator *communicator_;
#else
  Parallel::TMCommunicator *communicator_;
#endif  // USE_MPI

  Performer::IdManager *id_manager_;
  Performer::IdManagerVector id_managers_;
  mutable ULInt id_off_set_;
  ULInt id_range_;

 private:
  MeshVector save_mesh_;
  ErrorMeshVector save_error_mesh_;
  vector<double> error_step_;
  Adapter adapter_;

  int step_;
  double error_local_process_;
};

#endif  // GENERATOR_GENERATOR_ADAPTIVE_H

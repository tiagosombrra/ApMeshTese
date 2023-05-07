#ifndef GENERATOR_GENERATOR_ADAPTIVE_H
#define GENERATOR_GENERATOR_ADAPTIVE_H

#include <omp.h>

#include <list>
#include <memory>
#include <string>

#include "../adapter/adapter.h"
#include "../crab_mesh/performer/ranged_id_manager.h"
#include "../curvature/curvature_analytical.h"
#include "../curvature/curvature_discrete.h"
#include "../data/geometry.h"
#include "../data/model.h"
#include "../data/node_adaptive.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_coons.h"
#include "../data/triangle_adaptive.h"
#include "../definitions.h"
#include "../estimate/charge_estimate_process.h"
#include "../input_output/models_3d.h"
#include "../input_output/patch_reader.h"
#include "../parallel/ApMeshCommunicator.h"
#include "../timer/timer.h"

using namespace Data;

extern vector<std::shared_ptr<CurveAdaptiveParametric>> ptr_aux;

class GeneratorAdaptive {
 public:
  // gera a malha inicial e insere na lista de malhas do modelo
  // a lista de pontos da curva é preenchida durante a geração
  typedef std::vector<std::pair<int, std::shared_ptr<MeshAdaptive>>> MeshVector;
  typedef std::vector<std::pair<int, std::shared_ptr<MeshAdaptive>>>
      ErrorMeshVector;

  GeneratorAdaptive();
  ~GeneratorAdaptive() = default;

#if USE_MPI
  int Execute(char *argv[], Timer &timer, MPI_Status status);
  std::list<std::shared_ptr<PatchBezier>> EstimateChargeofPatches(
      std::shared_ptr<Geometry> &geometry, Timer &timer);
  std::vector<std::shared_ptr<CurveAdaptive>> CreateVectorOfCurves(
      std::list<std::shared_ptr<PatchBezier>> patches);
  std::list<std::shared_ptr<PatchBezier>> OrderPatchesDistribProcess(
      std::list<std::shared_ptr<PatchBezier>> &patches);
  bool VerifyCurve(PointAdaptive p0, PointAdaptive p1, PointAdaptive p2,
                   PointAdaptive p3,
                   std::vector<std::shared_ptr<CurveAdaptive>> curves);
  void CalculateEstimateProcessElements(
      int size_process, std::list<std::shared_ptr<PatchBezier>> patches);
  std::list<std::shared_ptr<PatchBezier>>::iterator GetIteratorListPatches(
      int size_patches, std::list<std::shared_ptr<PatchBezier>> patches);
  void Generator(double patches[], int size_patches, Timer &timer,
                 int id_range = 1024, int size_rank = 1, int size_thread = 1);
  std::shared_ptr<Geometry> UnpakGeometry(double patches[], int size_patches);
#else
  int Execute(char *argv[], Timer &timer);
  void Generator(Model &model, Timer &timer, int id_range = 1024,
                 int size_rank = 1, int size_thread = 1);
#endif

  std::shared_ptr<SubMesh> InitialMesh(
      std::shared_ptr<PatchCoons> &patch,
      std::shared_ptr<Performer::IdManager> &id_manager);
  double ErrorGlobal(std::shared_ptr<MeshAdaptive> &mesh, Timer &timer,
                     int rank = 0, int size_thread = 0);
  std::shared_ptr<Performer::IdManager> MakeIdManager(
      const std::shared_ptr<Parallel::TMCommunicator> &comm, Int id) const;
  std::shared_ptr<Performer::IdManager> MakeIdManagerOmp(
      const std::shared_ptr<Parallel::TMCommunicator> &comm, Int id) const;
  std::shared_ptr<Performer::IdManager> MakeIdManagerElementOmp(
      const std::shared_ptr<Parallel::TMCommunicator> &comm, Int id) const;

  void WriteMesh(const std::shared_ptr<MeshAdaptive> &mesh, int step);
  void WriteMesh(const std::shared_ptr<MeshAdaptive> &mesh, int step,
                 vector<double> error_step, int rank = -1);
  void WriteQualityMesh(const std::shared_ptr<MeshAdaptive> &mesh, int step,
                        vector<double> error_step, int rank = -1);
  void SaveErrorMesh(const std::shared_ptr<MeshAdaptive> &mesh);
  void SaveMesh(const std::shared_ptr<MeshAdaptive> &mesh, int step);
  void SaveErrorMesh(const std::shared_ptr<MeshAdaptive> &mesh, int step);
  void AdaptCurve(Geometry &geometry);
  void AdaptDomain(Geometry &geometry, std::shared_ptr<MeshAdaptive> &mesh);
  void GeneratorInitialMesh(std::shared_ptr<Geometry> &geometry,
                            std::shared_ptr<MeshAdaptive> &mesh, Timer &timer,
                            int size_thread, int size_patch);
  void PrintElments(const std::shared_ptr<MeshAdaptive> &mesh, int step,
                    vector<double> error_step, int rank = -1);

#if USE_OPENMP
  virtual std::shared_ptr<SubMesh> GeneratorInitialMeshOmp(
      std::shared_ptr<PatchCoons> &patch,
      std::shared_ptr<Performer::IdManager> &id_Manager);
  virtual double CalculateErrorGlobalOmp(std::shared_ptr<MeshAdaptive> &mesh,
                                         Timer &timer, int rank = 0,
                                         int size_thread = 0);
  int GeneratorOmp(Model &model, Timer &timer, int id_range = 0,
                   int sizeRank = 1, int sizeThread = 1);
  void AdaptCurveOmp(std::shared_ptr<Geometry> &geometry);
  void AdaptDomainOmp(std::shared_ptr<Geometry> &geometry,
                      std::shared_ptr<MeshAdaptive> &mesh, Timer &timer,
                      int size_thread, int size_patch);
#endif  // #USE_OPENMP

#if USE_MPI
  Model model_;
  std::shared_ptr<Geometry> geometry_;
  std::shared_ptr<PatchCoons> patch_;
#endif  // USE_MPI

  std::shared_ptr<MeshAdaptive> mesh_;

 protected:
#if (USE_MPI || USE_OPENMP)
  std::shared_ptr<ApMeshCommunicator> communicator_;
#else
  std::shared_ptr<Parallel::TMCommunicator> communicator_;
#endif  // USE_MPI

  std::shared_ptr<Performer::IdManager> id_manager_;
  Performer::IdManagerVector id_managers_;
  mutable ULInt id_off_set_;
  ULInt id_range_;

 private:
  MeshVector save_mesh_;
  ErrorMeshVector save_error_mesh_;
  std::vector<double> error_step_;
  Adapter adapter_;

  int step_;
  double error_local_process_;
};

#endif  // GENERATOR_GENERATOR_ADAPTIVE_H

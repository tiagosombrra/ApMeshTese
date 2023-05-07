#ifndef ADAPTER_ADAPTER_H
#define ADAPTER_ADAPTER_H

#include <cmath>
#include <cstdlib>
#include <map>
#include <memory>
#include <typeinfo>

#include "../crab_mesh/aft/advancing_front.h"
#include "../crab_mesh/aft/boundary.h"
#include "../crab_mesh/aft/edge.h"
#include "../crab_mesh/aft/face.h"
#include "../crab_mesh/aft/vertex.h"
#include "../crab_mesh/performer/ranged_id_manager.h"
#include "../curvature/curvature_analytical.h"
#include "../curvature/curvature_discrete.h"
#include "../data/curve/curve_adaptive_parametric.h"
#include "../data/mesh/sub_mesh.h"
#include "../data/patch/patch_coons.h"
#include "../data/tree/bin_tree.h"
#include "../definitions.h"
#include "../parallel/TMCommunicator.h"

class Adapter {
 public:
#if USE_OPENMP
  std::list<std::shared_ptr<PointAdaptive>> AdaptCurveByCurveOmp(
      std::shared_ptr<CurveAdaptiveParametric>& curve,
      std::shared_ptr<Performer::IdManager>& id_manager,
      double factor_disc_global = 1.0);
  std::list<std::shared_ptr<PointAdaptive>> AdaptCurveBySurfaceOmp(
      std::shared_ptr<CurveAdaptiveParametric>& curve,
      std::shared_ptr<Performer::IdManager>& id_manager,
      double factor_disc_global = 1.0);
  std::shared_ptr<SubMesh> AdaptDomainOmp(
      std::shared_ptr<PatchCoons>& coons_patch,
      std::shared_ptr<Performer::IdManager>& id_manager,
      double factor_disc_global = 1.0);
#endif  // #USE_OPENMP

  std::list<std::shared_ptr<PointAdaptive>> AdaptCurveByCurve(std::shared_ptr<CurveAdaptiveParametric>& curve,
      map<std::shared_ptr<PointAdaptive>, std::shared_ptr<PointAdaptive> > &map_points,
      std::shared_ptr<Performer::IdManager>& id_manager,
      double factor_disc_global = 1.0);
  std::list<std::shared_ptr<PointAdaptive>> AdaptCurveBySurface(
      std::shared_ptr<CurveAdaptiveParametric>& curve,
      map<std::shared_ptr<PointAdaptive>, std::shared_ptr<PointAdaptive>>&
          map_points,
      std::shared_ptr<Performer::IdManager>& id_manager,
      double factor_disc_global = 1.0);
  std::shared_ptr<SubMesh> AdaptDomain(
      std::shared_ptr<PatchCoons>& coons_patch,
      std::shared_ptr<Performer::IdManager>& id_manager,
      double factor_disc_global = 1.0);

 private:
  double CalculateNewSize(const double ka, const double kd,
                          const double factor_disc, const double length_old);
};
#endif  // ADAPTER_ADAPTER_H

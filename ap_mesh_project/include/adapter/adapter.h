#ifndef ADAPTER_ADAPTER_H
#define ADAPTER_ADAPTER_H

#include <cmath>
#include <cstdlib>
#include <map>

#include "../crab_mesh/aft/advancing_front.h"
#include "../crab_mesh/aft/boundary.h"
#include "../crab_mesh/aft/edge.h"
#include "../crab_mesh/aft/face.h"
#include "../crab_mesh/aft/vertex.h"
#include "../crab_mesh/performer/ranged_id_manager.h"
#include "../curvature/curvature_analytical.h"
#include "../curvature/curvature_discrete.h"
#include "../data/curve/curve_adaptive_parametric.h"
#include "../data/definitions.h"
#include "../data/mesh/sub_mesh.h"
#include "../data/patch/patch_coons.h"
#include "../data/tree/bin_tree.h"
#include "../parallel/TMCommunicator.h"

class Adapter {
 public:
#if USE_OPENMP
  list<Ponto*> AdaptCurveByCurveOmp(CurveAdaptive* curve,
                                    Performer::IdManager* id_manager,
                                    double factor_disc_global = 1.0);
  list<Ponto*> AdaptCurveBySurfaceOmp(CurveAdaptive* curve,
                                      Performer::IdManager* id_manager,
                                      double factor_disc_global = 1.0);
  SubMesh* AdaptDomainOmp(PatchCoons* coons_patch,
                          Performer::IdManager* id_manager,
                          double factor_disc_global = 1.0);
#endif  // #USE_OPENMP

  list<Ponto*> AdaptCurveByCurve(CurveAdaptive* curve,
                                 map<Ponto*, Ponto*>& map_points,
                                 Performer::IdManager* id_manager,
                                 double factor_disc_global = 1.0);
  list<Ponto*> AdaptCurveBySurface(CurveAdaptive* curve,
                                   map<Ponto*, Ponto*>& map_points,
                                   Performer::IdManager* id_manager,
                                   double factor_disc_global = 1.0);
  SubMesh* AdaptDomain(PatchCoons* coons_patch,
                       Performer::IdManager* id_manager,
                       double factor_disc_global = 1.0);

 private:
  double CalculateNewSize(const double ka, const double kd,
                          const double factor_disc, const double length_old);
};
#endif  // ADAPTER_ADAPTER_H

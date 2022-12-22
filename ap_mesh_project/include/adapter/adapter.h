#ifndef ADAPTER_H
#define ADAPTER_H

#include <cmath>
#include <cstdlib>
#include <map>

#include "../crab_mesh/aft/advancing_front.h"
#include "../crab_mesh/aft/boundary.h"
#include "../crab_mesh/performer/ranged_id_manager.h"
#include "../curvature/CurvaturaAnalitica.h"
#include "../curvature/CurvaturaDiscreta.h"
#include "../data/Definitions.h"
#include "../data/Edge.h"
#include "../data/Face.h"
#include "../data/Vertex.h"
#include "../data/curve/CurvaParametrica.h"
#include "../data/mesh/SubMalha.h"
#include "../data/patch/CoonsPatch.h"
#include "../data/tree/BinTree.h"
#include "../parallel/TMCommunicator.h"

class Adapter {
 public:
#if USE_OPENMP
  list<Ponto*> AdaptCurveByCurveOmp(Curva* curve,
                                    Performer::IdManager* id_manager,
                                    double factor_disc_global = 1.0);
  list<Ponto*> AdaptCurveBySurfaceOmp(Curva* curve,
                                      Performer::IdManager* id_manager,
                                      double factor_disc_global = 1.0);
  SubMalha* AdaptDomainOmp(CoonsPatch* coons_patch,
                           Performer::IdManager* id_manager,
                           double factor_disc_global = 1.0);
#endif  // #USE_OPENMP

  list<Ponto*> AdaptCurveByCurve(Curva* curve, map<Ponto*, Ponto*>& map_points,
                                 Performer::IdManager* id_manager,
                                 double factor_disc_global = 1.0);
  list<Ponto*> AdaptCurveBySurface(Curva* curve,
                                   map<Ponto*, Ponto*>& map_points,
                                   Performer::IdManager* id_manager,
                                   double factor_disc_global = 1.0);
  double CalculateNewSize(const double ka, const double kd,
                          const double factor_disc, const double length_old);
  SubMalha* AdaptDomain(CoonsPatch* coons_patch,
                        Performer::IdManager* id_manager,
                        double factor_disc_global = 1.0);
};
#endif  // ADAPTER_H

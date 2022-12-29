#ifndef CURVATURE_CURVATURE_ANALYTICAL_H
#define CURVATURE_CURVATURE_ANALYTICAL_H

#include <cmath>
#include <tuple>

#include "../data/Vetor.h"
#include "../data/patch/patch_coons.h"
#include "../data/point_adaptive.h"
#include "curvature.h"

class CurvatureAnalytical : public Curvature {
 public:
  CurvatureAnalytical(const PointAdaptive& v, PatchCoons& p);
  double CalculateMeanCurvature() override;
  double CalculateGaussCurvature() override;

 private:
  Vetor qu_;
  Vetor qv_;
  Vetor quu_;
  Vetor quv_;
  Vetor qvv_;
  Vetor prod_;
  double a_;
  double b_;
  double c_;
};

#endif  // CURVATURE_CURVATURE_ANALYTICAL_H

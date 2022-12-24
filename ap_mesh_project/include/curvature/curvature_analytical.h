#ifndef CURVATURE_CURVATURE_ANALYTICAL_H
#define CURVATURE_CURVATURE_ANALYTICAL_H

#include <cmath>
#include <tuple>

#include "../data/Ponto.h"
#include "../data/Vetor.h"
#include "../data/patch/CoonsPatch.h"
#include "curvature.h"

class CurvatureAnalytical : public Curvature {
 public:
  CurvatureAnalytical(const Ponto& v, CoonsPatch& p);
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

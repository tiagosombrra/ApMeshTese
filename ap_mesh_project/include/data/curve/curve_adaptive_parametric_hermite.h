#ifndef DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_HERMITE_H
#define DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_HERMITE_H

#include "curve_adaptive_parametric.h"

class CurveAdaptiveParametricHermite : public CurveAdaptiveParametric {
 public:
  CurveAdaptiveParametricHermite();
  CurveAdaptiveParametricHermite(Ponto point0, Ponto point1, Vetor vector0,
                                 Vetor vector1);
  CurveAdaptiveParametricHermite(CurveAdaptiveParametricHermite*);
  ~CurveAdaptiveParametricHermite();
  double CalculateCurvature(double t);

  Vetor GetVector0() const;
  void SetVector0(const Vetor& vector0);
  Vetor GetVector1() const;
  void SetVector1(const Vetor& vector1);

 protected:
  // vetor tangente no ponto inicial
  Vetor vector0_;
  // vetor tangente no ponto final
  Vetor vector1_;
};
#endif  // DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_HERMITE_H

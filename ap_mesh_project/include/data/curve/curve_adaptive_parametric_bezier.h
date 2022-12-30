#ifndef DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_BEZIER_H
#define DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_BEZIER_H

#include "curve_adaptive_parametric.h"

class CurveAdaptiveParametricBezier : public CurveAdaptiveParametric {
 public:
  // IMPORTANTE: Ao usar o construtor default, não esquecer de setar
  // os pontos de controle !!!
  CurveAdaptiveParametricBezier();
  // Esse construtor é mais recomendado !!! Evita erros ao esquecer de setar
  // os valores indicados na observação do construtor default !!!
  CurveAdaptiveParametricBezier(PointAdaptive point0, PointAdaptive point1,
                                PointAdaptive point2, PointAdaptive point3);
  CurveAdaptiveParametricBezier(CurveAdaptiveParametricBezier *);
  ~CurveAdaptiveParametricBezier();

  double CalculateCurvature(double parameter);

  PointAdaptive GetPoint2() const;
  void SetPoint2(const PointAdaptive &point);
  PointAdaptive GetPoint3() const;
  void SetPoint3(const PointAdaptive &point);

 protected:
  // vetor tangente no ponto inicial
  PointAdaptive point2_;
  // vetor tangente no ponto final
  PointAdaptive point3_;
};
#endif  // DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_BEZIER_H

#ifndef DATA_PATCH_PATCH_COONS_H
#define DATA_PATCH_PATCH_COONS_H

#include <tuple>
#include <vector>

#include "../../data/definitions.h"
#include "../Vetor.h"
#include "../curve/curve_adaptive.h"
#include "../point_adaptive.h"
#include "patch.h"

class PatchCoons : public Patch {
 public:
  PatchCoons();
  PatchCoons(PatchCoons* patch_coons);
  PatchCoons(vector<CurveAdaptive*> curves);
  virtual ~PatchCoons();

  void InsertCurve(CurveAdaptive* curve);
  unsigned int GetNumBerCurves() const;
  CurveAdaptive* GetCurve(const unsigned int position);
  // encontra as coordenadas parâmetricas u, v de um dado ponto p no patch,
  // ou a projeção desse ponto na superfície
  virtual tuple<double, double> FindUV(const PointAdaptive& p) = 0;

  // encontra o ponto p na curva dado um parâmetro p
  virtual PointAdaptive Parameterize(double u, double v) = 0;

  // calcula as derivadas parciais nas direçõe u e v
  virtual Vetor Qu(double u, double v) = 0;
  virtual Vetor Qv(double u, double v) = 0;
  virtual Vetor Quu(double u, double v) = 0;
  virtual Vetor Quv(double u, double v) = 0;
  virtual Vetor Qvu(double u, double v) = 0;
  virtual Vetor Qvv(double u, double v) = 0;

  // calcula as derivadas parciais de um ponto p
  virtual Vetor Qu(const PointAdaptive& point) = 0;
  virtual Vetor Qv(const PointAdaptive& point) = 0;
  virtual Vetor Quu(const PointAdaptive& point) = 0;
  virtual Vetor Quv(const PointAdaptive& point) = 0;
  virtual Vetor Qvu(const PointAdaptive& point) = 0;
  virtual Vetor Qvv(const PointAdaptive& point) = 0;

 protected:
  vector<CurveAdaptive*> curves_;
};

#endif  // DATA_PATCH_PATCH_COONS_H

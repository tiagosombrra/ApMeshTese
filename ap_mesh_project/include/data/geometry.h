#ifndef Geometria_h
#define Geometria_h

#include <map>
#include <vector>

#include "../data/curve/curve_adaptive.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/patch/patch_hermite.h"

class Geometry {
 public:
  Geometry();
  ~Geometry() = default;

  void InsertCurve(CurveAdaptive* curve);
  void InsertCurve(CurveAdaptive* curve, int position);
  unsigned int GetNumberCurves() const;
  CurveAdaptive* GetCurve(
      const unsigned int position);  // retorna a i-ésima curva
  void InsertPatch(Patch* patch);
  void InsertPatch(Patch* patch, int position);
  unsigned int GetNumberPatches() const;
  Patch* GetPatch(const unsigned int position);  // retorna o i-ésimo patch
  CurveAdaptiveParametricBezier* VerifyCurveGeometry(PointAdaptive*,
                                                     PointAdaptive*,
                                                     PointAdaptive*,
                                                     PointAdaptive*);

 protected:
  vector<CurveAdaptive*> curves_;
  vector<Patch*> patches_;
};
#endif

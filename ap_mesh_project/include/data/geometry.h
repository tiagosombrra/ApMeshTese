#ifndef Geometria_h
#define Geometria_h

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

#include "../data/curve/curve_adaptive.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_coons.h"
#include "../data/patch/patch_hermite.h"

class Geometry {
 public:
  Geometry();
  ~Geometry();

  void InsertCurve(std::shared_ptr<CurveAdaptiveParametric>& curve);
  void InsertCurve(std::shared_ptr<CurveAdaptiveParametric>& curve,
                   int position);
  void InsertCurve(
      std::shared_ptr<CurveAdaptiveParametricBezier>& curve_bezier);
  void InsertCurve(
      std::shared_ptr<CurveAdaptiveParametricHermite>& curve_hermite);

  unsigned int GetNumberCurves() const;
  std::shared_ptr<CurveAdaptiveParametric> GetCurve(
      const unsigned int position);  // retorna a i-ésima curva
  void InsertPatch(std::shared_ptr<PatchBezier>& patch);
  void InsertPatch(std::shared_ptr<PatchBezier>& patch, int position);
  void InsertPatch(std::shared_ptr<PatchHermite>& patch);
  unsigned int GetNumberPatches() const;
  std::shared_ptr<Patch> GetPatch(
      const unsigned int position);  // retorna o i-ésimo patch
  std::shared_ptr<CurveAdaptiveParametricBezier> VerifyCurveGeometry(
      const PointAdaptive& point0, const PointAdaptive& point1,
      const PointAdaptive& point2, const PointAdaptive& point3);
  bool CurveMatchesPoints(const CurveAdaptive* curve,
                          const PointAdaptive& point0,
                          const PointAdaptive& point1,
                          const PointAdaptive& point2,
                          const PointAdaptive& point3);

 protected:
  std::vector<std::shared_ptr<CurveAdaptiveParametric>> curves_;
  std::vector<std::shared_ptr<PatchCoons>> patches_;
};
#endif

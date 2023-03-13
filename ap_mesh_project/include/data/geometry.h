#ifndef Geometria_h
#define Geometria_h

#include <map>
#include <memory>
#include <vector>

#include "../data/curve/curve_adaptive.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/patch/patch_hermite.h"

class Geometry : public std::enable_shared_from_this<Geometry> {
 public:
  Geometry();
  ~Geometry() = default;

  void InsertCurve(std::shared_ptr<CurveAdaptive> curve);
  void InsertCurve(std::shared_ptr<CurveAdaptive> curve, int position);
  unsigned int GetNumberCurves() const;
  std::shared_ptr<CurveAdaptive> GetCurve(
      const unsigned int position);  // retorna a i-ésima curva
  void InsertPatch(std::shared_ptr<Patch> patch);
  void InsertPatch(std::shared_ptr<Patch> patch, int position);
  unsigned int GetNumberPatches() const;
  std::shared_ptr<Patch> GetPatch(
      const unsigned int position);  // retorna o i-ésimo patch
  std::shared_ptr<CurveAdaptiveParametricBezier> VerifyCurveGeometry(
      std::shared_ptr<PointAdaptive>, std::shared_ptr<PointAdaptive>,
      std::shared_ptr<PointAdaptive>, std::shared_ptr<PointAdaptive>);
  std::vector<std::shared_ptr<CurveAdaptive>> GetCurves();

 protected:
  std::vector<std::shared_ptr<CurveAdaptive>> curves_;
  std::vector<std::shared_ptr<Patch>> patches_;
};
#endif

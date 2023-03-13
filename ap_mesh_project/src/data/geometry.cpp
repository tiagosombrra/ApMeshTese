#include "../../include/data/geometry.h"

Geometry::Geometry() {}

void Geometry::InsertCurve(std::shared_ptr<CurveAdaptive> curve) {
  this->curves_.push_back(curve);
}

void Geometry::InsertCurve(std::shared_ptr<CurveAdaptive> curve, int position) {
  this->curves_[position] = curve;
}

unsigned int Geometry::GetNumberCurves() const {
  return static_cast<int>(curves_.size());
}

std::shared_ptr<CurveAdaptive> Geometry::GetCurve(const unsigned int position) {
  return (position < this->curves_.size()) ? this->curves_[position] : nullptr;
}

void Geometry::InsertPatch(std::shared_ptr<Patch> patch) { this->patches_.push_back(patch); }

void Geometry::InsertPatch(std::shared_ptr<Patch> patch, int position) {
  this->patches_[position] = patch;
}

unsigned int Geometry::GetNumberPatches() const {
  return static_cast<int>(patches_.size());
}

std::shared_ptr<Patch> Geometry::GetPatch(const unsigned int position) {
  return (position < this->patches_.size()) ? this->patches_[position]
                                            : nullptr;
}

std::shared_ptr<CurveAdaptiveParametricBezier> Geometry::VerifyCurveGeometry(
    std::shared_ptr<PointAdaptive> point0, std::shared_ptr<PointAdaptive> point1, std::shared_ptr<PointAdaptive> point2,
    std::shared_ptr<PointAdaptive> point3) {
  for (auto it = curves_.begin(); it != curves_.end(); it++) {
    std::shared_ptr<CurveAdaptiveParametricBezier> curve =
        std::dynamic_pointer_cast<CurveAdaptiveParametricBezier>(*it);
    if (curve && curve->GetPoint0() == *point0 && curve->GetPoint1() == *point1 &&
        curve->GetPoint2() == *point2 && curve->GetPoint3() == *point3) {
      return curve;
    }
  }
  return nullptr;
}


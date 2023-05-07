#include "../../include/data/geometry.h"

Geometry::Geometry() {}

Geometry::~Geometry() {}

void Geometry::InsertCurve(std::shared_ptr<CurveAdaptiveParametric>& curve) {
  this->curves_.push_back(curve);
}

void Geometry::InsertCurve(std::shared_ptr<CurveAdaptiveParametric>& curve,
                           int position) {
  this->curves_[position] = curve;
}

void Geometry::InsertCurve(
    std::shared_ptr<CurveAdaptiveParametricBezier>& curve_bezier) {
  this->curves_.push_back(curve_bezier);
}

void Geometry::InsertCurve(
    std::shared_ptr<CurveAdaptiveParametricHermite>& curve_hermite) {
  this->curves_.push_back(curve_hermite);
}

unsigned int Geometry::GetNumberCurves() const {
  return static_cast<int>(curves_.size());
}

std::shared_ptr<CurveAdaptiveParametric> Geometry::GetCurve(
    const unsigned int position) {
  return (position < this->curves_.size()) ? this->curves_[position]
                                           : this->curves_.back();
}

void Geometry::InsertPatch(std::shared_ptr<PatchBezier>& patch) {
  this->patches_.push_back(patch);
}

void Geometry::InsertPatch(std::shared_ptr<PatchBezier>& patch, int position) {
  this->patches_[position] = patch;
}

void Geometry::InsertPatch(std::shared_ptr<PatchHermite>& patch_hermite) {
  this->patches_.push_back(patch_hermite);
}

unsigned int Geometry::GetNumberPatches() const {
  return static_cast<int>(patches_.size());
}

std::shared_ptr<Patch> Geometry::GetPatch(const unsigned int position) {
  return (position < this->patches_.size()) ? this->patches_[position]
                                            : nullptr;
}

std::shared_ptr<CurveAdaptiveParametricBezier> Geometry::VerifyCurveGeometry(
    const PointAdaptive& point0, const PointAdaptive& point1,
    const PointAdaptive& point2, const PointAdaptive& point3) {
  auto it = std::find_if(
      curves_.begin(), curves_.end(),
      [&](const std::shared_ptr<CurveAdaptiveParametric>& curve) {
        return CurveMatchesPoints(curve.get(), point0, point1, point2, point3);
      });
  if (it != curves_.end()) {
    return std::dynamic_pointer_cast<CurveAdaptiveParametricBezier>(*it);
  } else {
    return nullptr;
  }
}

bool Geometry::CurveMatchesPoints(const CurveAdaptive* curve,
                                  const PointAdaptive& point0,
                                  const PointAdaptive& point1,
                                  const PointAdaptive& point2,
                                  const PointAdaptive& point3) {
  auto* bezier = dynamic_cast<const CurveAdaptiveParametricBezier*>(curve);
  if (bezier == nullptr) {
    return false;
  }
  return bezier->GetPoint0() == point0 && bezier->GetPoint1() == point1 &&
         bezier->GetPoint2() == point2 && bezier->GetPoint3() == point3;
}

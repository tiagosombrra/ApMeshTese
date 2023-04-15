#include "../../include/data/geometry.h"

Geometry::Geometry() {}

Geometry::~Geometry() {
  // // 1. apaga a lista de curvas
  // while (!this->curves_.empty()) {
  //   CurveAdaptive *curve = this->curves_.back();
  //   this->curves_.pop_back();
  //   delete curve;
  // }

  // // 3. apaga a lista de patches
  // while (!this->patches_.empty()) {
  //   Patch *patch = this->patches_.back();
  //   this->patches_.pop_back();
  //   delete patch;
  // }
}

void Geometry::InsertCurve(CurveAdaptive *curve) {
  this->curves_.push_back(curve);
}

void Geometry::InsertCurve(CurveAdaptive *curve, int position) {
  this->curves_[position] = curve;
}

unsigned int Geometry::GetNumberCurves() const {
  return static_cast<int>(curves_.size());
}

CurveAdaptive *Geometry::GetCurve(const unsigned int position) {
  return (position < this->curves_.size()) ? this->curves_[position] : nullptr;
}

void Geometry::InsertPatch(Patch *patch) { this->patches_.push_back(patch); }

void Geometry::InsertPatch(Patch *patch, int position) {
  this->patches_[position] = patch;
}

unsigned int Geometry::GetNumberPatches() const {
  return static_cast<int>(patches_.size());
}

Patch *Geometry::GetPatch(const unsigned int position) {
  return (position < this->patches_.size()) ? this->patches_[position]
                                            : nullptr;
}

CurveAdaptiveParametricBezier *Geometry::VerifyCurveGeometry(
    PointAdaptive *point0, PointAdaptive *point1, PointAdaptive *point2,
    PointAdaptive *point3) {
  for (vector<CurveAdaptive *>::iterator it = this->curves_.begin();
       it != this->curves_.end(); it++) {
    if (static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint0().
        operator==(point0) &&
        static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint1().
        operator==(point1) &&
        static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint2().
        operator==(point2) &&
        static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint3().
        operator==(point3)) {
      return static_cast<CurveAdaptiveParametricBezier *>(*it);
    }
  }

  return nullptr;
}

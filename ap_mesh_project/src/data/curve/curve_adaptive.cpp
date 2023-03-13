#include "../../../include/data/curve/curve_adaptive.h"

CurveAdaptive::CurveAdaptive()
    : id_(0), length_(0), points_({}), patches_({}) {}

CurveAdaptive::CurveAdaptive(std::shared_ptr<CurveAdaptive> curve)
    : id_(curve->id_),
      length_(curve->length_),
      points_(curve->points_),
      patches_(curve->patches_) {}

void CurveAdaptive::SetId(unsigned int id) { this->id_ = id; }

double CurveAdaptive::GetLength() { return this->length_; }

void CurveAdaptive::InsertPoint(std::shared_ptr<PointAdaptive> point) {
  this->points_.push_back(point);
}

unsigned int CurveAdaptive::GetNumBerPoints() const {
  return static_cast<int>(points_.size());
}

std::shared_ptr<PointAdaptive> CurveAdaptive::GetPoint(
    const unsigned int position) {
  list<std::shared_ptr<PointAdaptive>>::iterator it = this->points_.begin();
  advance(it, position);

  return *it;
}

void CurveAdaptive::InsertPatch(std::shared_ptr<Patch> patch) {
  this->patches_.push_back(patch);
}

void CurveAdaptive::SetPoints(
    std::list<std::shared_ptr<PointAdaptive>> new_points) {
  this->points_ = new_points;
}

std::list<std::shared_ptr<PointAdaptive>>& CurveAdaptive::GetPoints() {
  return this->points_;
}

unsigned int CurveAdaptive::GetNumBerPatches() const {
  return (int)patches_.size();
}

std::shared_ptr<Patch> CurveAdaptive::GetPatch(const unsigned int position) {
  return (position < this->patches_.size()) ? this->patches_[position]
                                            : nullptr;
}

// verifica se uma curva é de borda
bool CurveAdaptive::CheckIsOnBorder() {
  if (this->GetNumBerPatches() == 1) return true;

  return false;
}

double CurveAdaptive::CalculateLengthPoints(const PointAdaptive&,
                                            const PointAdaptive&) {
  return -1;
}

double CurveAdaptive::CalculateLengthPoint(const PointAdaptive&) { return -1; }

void CurveAdaptive::CalculateLengthCurve() {}

double CurveAdaptive::CalculateCurvature(double) { return -1; }

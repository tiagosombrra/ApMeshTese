#include "../../../include/data/curve/curve_adaptive.h"

CurveAdaptive::CurveAdaptive() {}

CurveAdaptive::CurveAdaptive(CurveAdaptive* curve) {
  this->id_ = curve->id_;
  this->length_ = curve->length_;
  this->points_ = curve->points_;
  this->patches_ = curve->patches_;
}

CurveAdaptive::~CurveAdaptive() {
  // 1. apaga a lista de pontos
  while (!this->points_.empty()) {
    Ponto* P = this->points_.back();
    this->points_.pop_back();
    delete P;
  }
  // 3. apaga a lista de pontos
  this->points_.clear();
  // 4. apaga a lista de patches
  this->patches_.clear();
}

void CurveAdaptive::SetId(unsigned int id) { this->id_ = id; }

double CurveAdaptive::GetLength() { return this->length_; }

void CurveAdaptive::InsertPoint(Ponto* p) { this->points_.push_back(p); }

unsigned int CurveAdaptive::GetNumBerPoints() const {
  return (int)points_.size();
}

Ponto* CurveAdaptive::GetPoint(const unsigned int position) {
  list<Ponto*>::iterator it = this->points_.begin();
  advance(it, position);

  return *it;
}

void CurveAdaptive::InsertPatch(Patch* patch) {
  this->patches_.push_back(patch);
}

void CurveAdaptive::SetPoints(list<Ponto*> new_points) {
  this->points_ = new_points;
}

list<Ponto*>& CurveAdaptive::GetPoints() { return this->points_; }

unsigned int CurveAdaptive::GetNumBerPatches() const {
  return (int)patches_.size();
}

Patch* CurveAdaptive::GetPatch(const unsigned int position) {
  return (position < this->patches_.size()) ? this->patches_[position]
                                            : nullptr;
}

// verifica se uma curva é de borda
bool CurveAdaptive::CheckIsOnBorder() {
  if (this->GetNumBerPatches() == 1) return true;

  return false;
}

double CurveAdaptive::CalculateLengthPoints(const Ponto&, const Ponto&) {
  return -1;
}

double CurveAdaptive::CalculateLengthPoint(const Ponto&) { return -1; }

void CurveAdaptive::CalculateLengthCurve() {}

double CurveAdaptive::CalculateCurvature(double) { return -1; }

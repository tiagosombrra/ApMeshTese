#include "../../../include/data/curve/Curva.h"

Curva::Curva() {}

Curva::Curva(Curva* curve) {
  this->id_ = curve->id_;
  this->length_ = curve->length_;
  this->points_ = curve->points_;
  this->patches_ = curve->patches_;
}

Curva::~Curva() {
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

void Curva::SetId(unsigned int id) { this->id_ = id; }

double Curva::GetLength() { return this->length_; }

void Curva::InsertPoint(Ponto* p) { this->points_.push_back(p); }

unsigned int Curva::GetNumBerPoints() const { return (int)points_.size(); }

Ponto* Curva::GetPoint(const unsigned int position) {
  list<Ponto*>::iterator it = this->points_.begin();
  advance(it, position);

  return *it;
}

void Curva::InsertPatch(Patch* patch) { this->patches_.push_back(patch); }

void Curva::SetPoints(list<Ponto*> new_points) { this->points_ = new_points; }

list<Ponto*>& Curva::GetPoints() { return this->points_; }

unsigned int Curva::GetNumBerPatches() const { return (int)patches_.size(); }

Patch* Curva::GetPatch(const unsigned int position) {
  return (position < this->patches_.size()) ? this->patches_[position]
                                            : nullptr;
}

// verifica se uma curva é de borda
bool Curva::CheckIsOnBorder() {
  if (this->GetNumBerPatches() == 1) return true;

  return false;
}

double Curva::calcularTamanho(const Ponto&, const Ponto&) { return -1; }

double Curva::calcularTamanho(const Ponto&) { return -1; }

void Curva::calcular_L() {}

double Curva::calcularCurvatura(double) { return -1; }

#include "../../include/data/noh.h"

Noh::Noh() : PointAdaptive() {}

Noh::Noh(double x, double y, double z) : PointAdaptive(x, y, z) {}

Noh::Noh(double x, double y, double z, unsigned long position)
    : PointAdaptive(x, y, z, position) {}

Noh::Noh(const PointAdaptive &point) {
  this->id_ = point.GetId();
  this->x_ = point.GetX();
  this->y_ = point.GetY();
  this->z_ = point.GetZ();
}

Noh::~Noh() {
  // 1. apaga a lista de elementos
  this->elements_.clear();
}

void Noh::InsertElement(ElementAdaptive *element) {
  this->elements_.push_back(element);
}

unsigned int Noh::GetNumberElements() const {
  return static_cast<unsigned int>(elements_.size());
}

ElementAdaptive *Noh::GetElement(const unsigned int position) {
  std::list<ElementAdaptive *>::iterator it = this->elements_.begin();
  advance(it, position);

  return *it;
}

void Noh::ClearElements() { elements_.clear(); }

const std::list<ElementAdaptive *> &Noh::GetElements() const {
  return elements_;
}

void Noh::setElements(const std::list<ElementAdaptive *> &elements) {
  elements_ = elements;
}

double Noh::GetGa() const { return ga_; }

void Noh::SetGa(double ga) { ga_ = ga; }

double Noh::GetGd() const { return gd_; }

void Noh::SetGd(double gd) { gd_ = gd; }

double Noh::GetHa() const { return ha_; }

void Noh::SetHa(double ha) { ha_ = ha; }

double Noh::GetHd() const { return hd_; }

void Noh::SetHd(double hd) { hd_ = hd; }

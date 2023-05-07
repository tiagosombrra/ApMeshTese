#include "../../include/data/vector_adaptive.h"

VectorAdaptive::VectorAdaptive()
    : x_(0), y_(0), z_(0), module_(CalculateModule()) {}

VectorAdaptive::VectorAdaptive(double x, double y, double z)
    : x_(x), y_(y), z_(z), module_(CalculateModule()) {}

VectorAdaptive::VectorAdaptive(const VectorAdaptive& vector)
    : x_(vector.x_), y_(vector.y_), z_(vector.z_), module_(CalculateModule()) {}

VectorAdaptive::VectorAdaptive(const PointAdaptive& point)
    : x_(point.GetX()),
      y_(point.GetY()),
      z_(point.GetZ()),
      module_(CalculateModule()) {}

VectorAdaptive::VectorAdaptive(const PointAdaptive& p, const PointAdaptive& q)
    : x_(q.GetX() - p.GetX()),
      y_(q.GetY() - p.GetY()),
      z_(q.GetZ() - p.GetZ()),
      module_(CalculateModule()) {}

VectorAdaptive VectorAdaptive::operator+(const VectorAdaptive& vector) const {
  VectorAdaptive soma(this->x_ + vector.x_, this->y_ + vector.y_,
                      this->z_ + vector.z_);

  return soma;
}

VectorAdaptive VectorAdaptive::operator-(const VectorAdaptive& vector) const {
  VectorAdaptive subt(this->x_ - vector.x_, this->y_ - vector.y_,
                      this->z_ - vector.z_);

  return subt;
}

VectorAdaptive VectorAdaptive::operator-() const {
  VectorAdaptive invr(-1 * this->x_, -1 * this->y_, -1 * this->z_);

  return invr;
}

double VectorAdaptive::operator^(const VectorAdaptive& vector) const {
  return (this->x_ * vector.x_ + this->y_ * vector.y_ + this->z_ * vector.z_);
}

VectorAdaptive VectorAdaptive::operator^(const double value) const {
  VectorAdaptive prod(value * this->x_, value * this->y_, value * this->z_);
  return prod;
}

VectorAdaptive VectorAdaptive::operator*(const VectorAdaptive& vector) const {
  VectorAdaptive prod(this->y_ * vector.z_ - this->z_ * vector.y_,
                      this->z_ * vector.x_ - this->x_ * vector.z_,
                      this->x_ * vector.y_ - this->y_ * vector.x_);

  return prod;
}

const VectorAdaptive& VectorAdaptive::operator=(const VectorAdaptive& vector) {
  this->x_ = vector.x_;
  this->y_ = vector.y_;
  this->z_ = vector.z_;
  this->module_ = CalculateModule();

  return *this;
}

const VectorAdaptive& VectorAdaptive::operator=(const PointAdaptive& point) {
  this->x_ = point.GetX();
  this->y_ = point.GetY();
  this->z_ = point.GetZ();
  this->module_ = CalculateModule();

  return *this;
}

double VectorAdaptive::CalculateModule() const {
  return std::hypot(std::hypot(this->x_, this->y_), this->z_);
}

double VectorAdaptive::CalculateAngle(const VectorAdaptive& vector) const {
  const VectorAdaptive& c = *this;

  const double cos =
      (c ^ vector) / (c.CalculateModule() * vector.CalculateModule());
  const double clamped_cos = std::fmin(1.0, std::fmax(-1.0, cos));

  return clamped_cos;
}

VectorAdaptive VectorAdaptive::CalculateUnitVector() {
  double module_inv = 1.0 / module_;

  return VectorAdaptive(this->x_ * module_inv, this->y_ * module_inv,
                        this->z_ * module_inv);
}

double VectorAdaptive::GetX() const { return x_; }

void VectorAdaptive::SetX(double x) { x_ = x; }

double VectorAdaptive::GetY() const { return y_; }

void VectorAdaptive::SetY(double y) { y_ = y; }

double VectorAdaptive::GetZ() const { return z_; }

void VectorAdaptive::SetZ(double z) { z_ = z; }

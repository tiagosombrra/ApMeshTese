#include "../../include/data/vector_adaptive.h"

VectorAdaptive::VectorAdaptive() : x_(0), y_(0), z_(0) {}

VectorAdaptive::VectorAdaptive(double c_x, double c_y, double c_z)
    : x_(c_x), y_(c_y), z_(c_z) {}

VectorAdaptive::VectorAdaptive(const VectorAdaptive& v)
    : x_(v.x_), y_(v.y_), z_(v.z_) {}

VectorAdaptive::VectorAdaptive(const PointAdaptive& point)
    : x_(point.GetX()), y_(point.GetY()), z_(point.GetZ()) {}

VectorAdaptive::VectorAdaptive(const PointAdaptive& p, const PointAdaptive& q)
    : x_(q.GetX() - p.GetX()),
      y_(q.GetY() - p.GetY()),
      z_(q.GetZ() - p.GetZ()) {}

VectorAdaptive VectorAdaptive::operator+(const VectorAdaptive& v) const {
  VectorAdaptive soma(this->x_ + v.x_, this->y_ + v.y_, this->z_ + v.z_);

  return soma;
}

VectorAdaptive VectorAdaptive::operator-(const VectorAdaptive& v) const {
  VectorAdaptive subt(this->x_ - v.x_, this->y_ - v.y_, this->z_ - v.z_);

  return subt;
}

VectorAdaptive VectorAdaptive::operator-() const {
  VectorAdaptive invr(-1 * this->x_, -1 * this->y_, -1 * this->z_);

  return invr;
}

double VectorAdaptive::operator^(const VectorAdaptive& v) const {
  return (this->x_ * v.x_ + this->y_ * v.y_ + this->z_ * v.z_);
}

VectorAdaptive VectorAdaptive::operator^(const double d) const {
  VectorAdaptive prod(d * this->x_, d * this->y_, d * this->z_);
  return prod;
}

VectorAdaptive VectorAdaptive::operator*(const VectorAdaptive& v) const {
  VectorAdaptive prod(this->y_ * v.z_ - this->z_ * v.y_,
                      this->z_ * v.x_ - this->x_ * v.z_,
                      this->x_ * v.y_ - this->y_ * v.x_);

  return prod;
}

const VectorAdaptive& VectorAdaptive::operator=(const VectorAdaptive& v) {
  this->x_ = v.x_;
  this->y_ = v.y_;
  this->z_ = v.z_;

  return *this;
}

const VectorAdaptive& VectorAdaptive::operator=(const PointAdaptive& p) {
  this->x_ = p.GetX();
  this->y_ = p.GetY();
  this->z_ = p.GetZ();

  return *this;
}

double VectorAdaptive::CalculateModule() const {
  return (
      sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_));
}

double VectorAdaptive::CalculateAngle(VectorAdaptive& vector) const {
  VectorAdaptive c(this->x_, this->y_, this->z_);

  double cos = static_cast<double>(c ^ vector) /
               (c.CalculateModule() * vector.CalculateModule());

  return (acos((cos > 1.0) ? 1.0 : ((cos < -1.0) ? -1.0 : cos)));
}

VectorAdaptive VectorAdaptive::CalculateUnitVector() {
  double modulo = this->CalculateModule();

  VectorAdaptive N(static_cast<double>(this->x_) / modulo,
                   static_cast<double>(this->y_) / modulo,
                   static_cast<double>(this->z_) / modulo);

  return N;
}

double VectorAdaptive::GetX() const { return x_; }

void VectorAdaptive::SetX(double x) { x_ = x; }

double VectorAdaptive::GetY() const { return y_; }

void VectorAdaptive::SetY(double y) { y_ = y; }

double VectorAdaptive::GetZ() const { return z_; }

void VectorAdaptive::SetZ(double z) { z_ = z; }

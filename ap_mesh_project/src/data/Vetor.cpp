/* Classe que define um vetor
 */
#include "../../include/data/Vetor.h"

Vetor::Vetor() : x_(0), y(0), z(0) {}

Vetor::Vetor(double c_x, double c_y, double c_z) : x_(c_x), y(c_y), z(c_z) {}

Vetor::Vetor(const Vetor& v) : x_(v.x_), y(v.y), z(v.z) {}

Vetor::Vetor(const PointAdaptive& point)
    : x_(point.GetX()), y(point.GetY()), z(point.GetZ()) {}

Vetor::Vetor(const PointAdaptive& p, const PointAdaptive& q)
    : x_(q.GetX() - p.GetX()), y(q.GetY() - p.GetY()), z(q.GetZ() - p.GetZ()) {}

Vetor Vetor::operator+(const Vetor& v) const {
  Vetor soma(this->x_ + v.x_, this->y + v.y, this->z + v.z);

  return soma;
}

Vetor Vetor::operator-(const Vetor& v) const {
  Vetor subt(this->x_ - v.x_, this->y - v.y, this->z - v.z);

  return subt;
}

Vetor Vetor::operator-() const {
  Vetor invr(-1 * this->x_, -1 * this->y, -1 * this->z);

  return invr;
}

double Vetor::operator^(const Vetor& v) const {
  return (this->x_ * v.x_ + this->y * v.y + this->z * v.z);
}

Vetor Vetor::operator^(const double d) const {
  Vetor prod(d * this->x_, d * this->y, d * this->z);
  return prod;
}

Vetor Vetor::operator*(const Vetor& v) const {
  Vetor prod(this->y * v.z - this->z * v.y, this->z * v.x_ - this->x_ * v.z,
             this->x_ * v.y - this->y * v.x_);

  return prod;
}

const Vetor& Vetor::operator=(const Vetor& v) {
  this->x_ = v.x_;
  this->y = v.y;
  this->z = v.z;

  return *this;
}

const Vetor& Vetor::operator=(const PointAdaptive& p) {
  this->x_ = p.GetX();
  this->y = p.GetY();
  this->z = p.GetZ();

  return *this;
}

double Vetor::modulo() const {
  return (sqrt(this->x_ * this->x_ + this->y * this->y + this->z * this->z));
}

double Vetor::CalculateAngle(Vetor& vector) const {
  Vetor c(this->x_, this->y, this->z);

  double cos = static_cast<double>(c ^ vector) / (c.modulo() * vector.modulo());

  return (acos((cos > 1.0) ? 1.0 : ((cos < -1.0) ? -1.0 : cos)));
}

Vetor Vetor::unitario() {
  double modulo = this->modulo();

  Vetor N(static_cast<double>(this->x_) / modulo,
          static_cast<double>(this->y) / modulo,
          static_cast<double>(this->z) / modulo);

  return N;
}

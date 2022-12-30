/* Classe que define um vértice
 */
#include "../../include/data/Vertice.h"

Vertice::Vertice() {}

Vertice::Vertice(double x, double y, double z) : PointAdaptive(x, y, z) {}

Vertice::Vertice(PointAdaptive *point) {
  this->x_ = point->GetX();
  this->y_ = point->GetY();
  this->z_ = point->GetZ();
}

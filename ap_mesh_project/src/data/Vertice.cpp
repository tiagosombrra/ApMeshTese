/* Classe que define um vértice
 */
#include "../../include/data/Vertice.h"

Vertice::Vertice() {}

Vertice::Vertice(double x, double y, double z) : Ponto(x, y, z) {}

Vertice::Vertice(Ponto *p) {
  this->x = p->x;
  this->y = p->y;
  this->z = p->z;
}

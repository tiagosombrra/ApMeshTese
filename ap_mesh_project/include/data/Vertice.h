/* Classe que define um vértice
 */
#ifndef Vertice_h
#define Vertice_h

#include "point_adaptive.h"

struct Vertice : public PointAdaptive {
  Vertice();
  Vertice(double x, double y, double z);
  Vertice(PointAdaptive* p);
};
#endif

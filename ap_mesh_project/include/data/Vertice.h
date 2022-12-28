/* Classe que define um vértice
 */
#ifndef Vertice_h
#define Vertice_h

#include "Ponto.h"

struct Vertice : public Ponto {
  Vertice();
  Vertice(double x, double y, double z);
  Vertice(Ponto* p);
};
#endif

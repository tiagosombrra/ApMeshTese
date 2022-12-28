/* Classe que define um modelo
 */
#ifndef Modelo_h
#define Modelo_h

#include <iostream>
#include <list>

#include "../data/mesh/mesh_adaptive.h"
#include "Geometria.h"
#include "Ponto.h"
using namespace std;

class Modelo {
  Geometria* geometria;
  vector<MeshAdaptive*> malhas;

 public:
  void setGeometria(Geometria* g);
  Geometria* getGeometria();
  void insereMalha(MeshAdaptive* m);
  unsigned int getNumDeMalhas() const;
  MeshAdaptive* getMalha(const unsigned int i);  // retorna a i-ésima malha
  Modelo();
  Modelo(Geometria*);
  ~Modelo();
};

#endif

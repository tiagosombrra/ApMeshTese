/* Classe encarregada de fazer a geração das malhas
 */
#ifndef Gerador_h
#define Gerador_h

#include <cmath>

#include "../data/Modelo.h"
#include "../data/mesh/sub_mesh.h"

extern double TOLERANCIA;
extern double EPSYLON;  // define a qualidade mínima da malha

class Gerador {
 protected:
  unsigned int passo;

  virtual SubMesh* malhaInicial();
  virtual double erroGlobal();

 public:
  double erro;  // erro local em cada processo

  Gerador();
  // Gerador ( Modelo& modelo );
};

#endif

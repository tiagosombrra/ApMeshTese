/* Classe encarregada de fazer a geração adaptativa das malhas
 */
#ifndef GeradorAdaptativo_h
#define GeradorAdaptativo_h

#include "Gerador.h"

class GeradorAdaptativo : public Gerador {
 protected:
  virtual SubMesh* malhaInicial();
  virtual double erroGlobal();

 public:
  GeradorAdaptativo();
  // GeradorAdaptativo ( Modelo& modelo );
};

#endif

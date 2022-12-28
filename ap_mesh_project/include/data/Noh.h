/* Classe que define um nó
 */
#ifndef Noh_h
#define Noh_h

#include <iostream>
#include <list>

#include "Elemento.h"
#include "Ponto.h"

using namespace std;

struct Noh : public Ponto {
  list<Elemento*> guarda_chuva;  // lista de elementos no nó

  double Ga;  // curvatura analítica gaussiana
  double Gd;  // curvatura discreta gaussiana
  double Ha;  // curvatura analítica média
  double Hd;  // curvatura discreta média

  void insereElemento(Elemento* elem);
  unsigned int getNumDeElementos() const;
  Elemento* getElemento(const unsigned int i);  // retorna o i-ésimo elemento
  void apagaGuardaChuva();
  Noh();
  Noh(double _x, double _y, double _z);
  Noh(double _x, double _y, double _z, unsigned long i);
  Noh(const Ponto& p);
  ~Noh();

  friend class CurvaturaDiscreta;
};

#endif

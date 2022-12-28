/* Classe que define um elemento
 */
#ifndef Elemento_h
#define Elemento_h

#include <iostream>

#include "Vetor.h"

using namespace std;

class Noh;

class Elemento {
 protected:
  unsigned long id;
  double area;
  Vetor normal;

 public:
  void setId(unsigned long i);
  unsigned long getId();

  double getArea() const;
  Vetor getNormal() const;

  virtual Noh getN(unsigned const int) const = 0;
  virtual double getAngulo(const Noh& n) = 0;
  virtual void calcularArea() = 0;
  virtual void calcularNormal() = 0;

  virtual ~Elemento();
};

#endif

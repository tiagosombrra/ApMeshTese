/* Classe que define um triangulo
 */
#ifndef Triangulo_h
#define Triangulo_h

#include <iostream>
using namespace std;

#include <tuple>

#include "Noh.h"
#include "Vetor.h"
#include "element_adaptive.h"

class Triangulo : public ElementAdaptive {
  Noh* n1;
  Noh* n2;
  Noh* n3;

  double angN1;  // o ângulo no nó 1
  double angN2;  // o ângulo no nó 2
  double angN3;  // o ângulo no nó 3

  double setAnguloN1();  // calcula o ângulo no nó 1
  double setAnguloN2();  // calcula o ângulo no nó 2
  double setAnguloN3();  // calcula o ângulo no nó 3

 public:
  tuple<double, double> p1;  // parametro do nó 1
  tuple<double, double> p2;  // parametro do nó 2
  tuple<double, double> p3;  // parametro do nó 3

  virtual Noh GetNoh(unsigned const int) const;  // retorna o i-ésimo nó
  virtual double GetAngle(const Noh& n);
  virtual void CalculateArea();    // OBS: Não normalizar a normal!!!!
  virtual void CalculateNormal();  // OBS: Não normalizar a normal!

  void substituir(Noh* velho, Noh* novo);

  Triangulo(Noh* v1, Noh* v2,
            Noh* v3);  // será construído no sentido v1->v2->v3
  virtual ~Triangulo();
  double quality();
};

#endif

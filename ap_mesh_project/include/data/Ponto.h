/* Classe para leitura de um ponto
 */
#ifndef Ponto_h
#define Ponto_h

#include <cmath>
#include <iostream>

using namespace std;

struct Ponto {
  unsigned long id;  // identificador
  // coordenadas
  double x;
  double y;
  double z;
  bool operator==(const Ponto& p) const;  // comparação
  bool operator==(const Ponto* p) const;  // comparação
  double distanciaPara(const Ponto& p) const;
  void mostraPonto();
  Ponto();
  Ponto(double _x, double _y, double _z);
  Ponto(double _x, double _y, double _z, unsigned long i);

 public:
  unsigned long getId() const;
  void setId(unsigned long value);
};
#endif

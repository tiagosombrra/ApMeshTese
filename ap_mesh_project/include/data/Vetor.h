/* Classe que define um vetor
 */
#ifndef Vetor_h
#define Vetor_h

#include <cmath>

#include "point_adaptive.h"

struct Vetor {
  // unsigned short id;

  // coordenadas
  double x_;
  double y;
  double z;

  Vetor operator+(const Vetor& v) const;   // soma vetorial
  Vetor operator-(const Vetor& v) const;   // diferença vetorial
  Vetor operator-() const;                 // vetor inverso
  double operator^(const Vetor& v) const;  // produto escalar
  Vetor operator^(const double d) const;   // produto por escalar
  Vetor operator*(const Vetor& v) const;   // produto vetorial

  const Vetor& operator=(const Vetor& v);          // atribuição
  const Vetor& operator=(const PointAdaptive& p);  // atribuição

  double modulo() const;
  double anguloCom(Vetor& v) const;  // ângulo entre dois vetores
  Vetor unitario();                  // vetor unitário

  Vetor();
  Vetor(double c_x, double c_y, double c_z);
  Vetor(const Vetor& v);
  Vetor(const PointAdaptive& p);
  Vetor(const PointAdaptive& p,
        const PointAdaptive& q);  // constrói um vetor entre dois pontos
};
#endif

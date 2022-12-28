/* Classe que define um patch de Coons
 */
#ifndef CoonsPatch_h
#define CoonsPatch_h

#include <cstdlib>
#include <tuple>
#include <vector>

#include "../../data/Definitions.h"
#include "../Ponto.h"
#include "../Vetor.h"
#include "../curve/curve_adaptive.h"
#include "Patch.h"

using namespace std;

class CoonsPatch : public Patch {
 protected:
  vector<CurveAdaptive*> curvas;

 public:
  void insereCurva(CurveAdaptive* c);
  unsigned int getNumDeCurvas() const { return (int)curvas.size(); }
  CurveAdaptive* getCurva(const unsigned int i);  // retorna a i-ésima curva

  // encontra as coordenadas parâmetricas u, v de um dado ponto p no patch,
  // ou a projeção desse ponto na superfície
  virtual tuple<double, double> encontrar_u_v(const Ponto& p) = 0;
  virtual tuple<double, double> find_u_v(const Ponto& p) = 0;

  // encontra o ponto p na curva dado um parâmetro p
  virtual Ponto parametrizar(double u, double v) = 0;

  // calcula as derivadas parciais nas direçõe u e v
  virtual Vetor Qu(double u, double v) = 0;
  virtual Vetor Qv(double u, double v) = 0;
  virtual Vetor Quu(double u, double v) = 0;
  virtual Vetor Quv(double u, double v) = 0;
  virtual Vetor Qvu(double u, double v) = 0;
  virtual Vetor Qvv(double u, double v) = 0;

  // calcula as derivadas parciais de um ponto p
  virtual Vetor Qu(const Ponto& p) = 0;
  virtual Vetor Qv(const Ponto& p) = 0;
  virtual Vetor Quu(const Ponto& p) = 0;
  virtual Vetor Quv(const Ponto& p) = 0;
  virtual Vetor Qvu(const Ponto& p) = 0;
  virtual Vetor Qvv(const Ponto& p) = 0;

  CoonsPatch();
  CoonsPatch(CoonsPatch* antigo);
  CoonsPatch(vector<CurveAdaptive*> listaDeCurvas);
  virtual ~CoonsPatch();
};

#endif

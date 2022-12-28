/* Classe que define uma geometria
 */
#ifndef Geometria_h
#define Geometria_h

#include <map>
#include <vector>

#include "../data/curve/curve_adaptive.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/patch/HermitePatch.h"
using namespace std;

class Geometria {
  vector<CurveAdaptive*> curvas;
  vector<Patch*> patches;

 public:
  void insereCurva(CurveAdaptive* c);
  void insereCurva(CurveAdaptive* c, int pos);
  unsigned int getNumDeCurvas() const;
  CurveAdaptive* getCurva(const unsigned int i);  // retorna a i-ésima curva
  void inserePatch(Patch* p);
  void inserePatch(Patch* p, int pos);
  unsigned int getNumDePatches() const;
  Patch* getPatch(const unsigned int i);  // retorna o i-ésimo patch
  CurveAdaptiveParametricBezier* verifyCurveGeometria(Ponto*, Ponto*, Ponto*,
                                                      Ponto*);
  Geometria();
  Geometria(Geometria*);
  ~Geometria();
};
#endif

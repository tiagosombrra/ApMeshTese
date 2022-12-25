/* Classe que define uma geometria
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef Geometria_h
#define Geometria_h

#include <map>
#include <vector>

#include "../data/curve/CurvParamBezier.h"
#include "../data/curve/curve_adaptive.h"
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
  CurvParamBezier* verifyCurveGeometria(Ponto*, Ponto*, Ponto*, Ponto*);
  Geometria();
  Geometria(Geometria*);
  ~Geometria();
};
#endif

/* Classe que define uma geometria
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Curvas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "../../include/data/Geometria.h"

void Geometry::insereCurva(CurveAdaptive *c) { this->curvas.push_back(c); }

void Geometry::insereCurva(CurveAdaptive *c, int pos) { this->curvas[pos] = c; }

unsigned int Geometry::getNumDeCurvas() const { return (int)curvas.size(); }

CurveAdaptive *Geometry::getCurva(const unsigned int i) {
  // adicionar excessão caso i > curvas.size();
  /*list <Curva*>::iterator it = this->curvas.begin();

  advance ( it, i );

  return *it;*/
  return (i < this->curvas.size()) ? this->curvas[i] : NULL;
}

void Geometry::inserePatch(Patch *p) { this->patches.push_back(p); }

void Geometry::inserePatch(Patch *p, int pos) { this->patches[pos] = p; }

unsigned int Geometry::getNumDePatches() const { return (int)patches.size(); }

Patch *Geometry::getPatch(const unsigned int i) {
  return (i < this->patches.size()) ? this->patches[i] : NULL;
}

CurveAdaptiveParametricBezier *Geometry::verifyCurveGeometria(Ponto *p0,
                                                              Ponto *p1,
                                                              Ponto *p2,
                                                              Ponto *p3) {
  for (vector<CurveAdaptive *>::iterator it = this->curvas.begin();
       it != this->curvas.end(); it++) {
    if (static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint0().
        operator==(p0) and
        static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint1().
        operator==(p1) and
        static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint2().
        operator==(p2) and
        static_cast<CurveAdaptiveParametricBezier *>(*it)->GetPoint3().
        operator==(p3)) {
      return static_cast<CurveAdaptiveParametricBezier *>(*it);
    }
  }

  return NULL;
}

Geometry::Geometry() {}

Geometry::Geometry(Geometry *antiga) {
  map<CurveAdaptive *, CurveAdaptive *> mapaCurvas;
  map<Patch *, Patch *> mapaPatches;

  for (vector<CurveAdaptive *>::iterator it = antiga->curvas.begin();
       it != antiga->curvas.end(); ++it) {
    CurveAdaptive *c = new CurveAdaptive((*it));

    mapaCurvas[(*it)] = c;
  }

  for (vector<Patch *>::iterator it = antiga->patches.begin();
       it != antiga->patches.end(); ++it) {
    Patch *p = new PatchHermite((PatchHermite *)(*it));

    for (unsigned int i = 0; i < ((PatchHermite *)(*it))->GetNumBerCurves();
         i++) {
      ((PatchHermite *)p)
          ->InsertCurve(mapaCurvas[((PatchHermite *)(*it))->GetCurve(i)]);
    }

    mapaPatches[(*it)] = p;
  }

  for (vector<CurveAdaptive *>::iterator it = antiga->curvas.begin();
       it != antiga->curvas.end(); ++it) {
    for (unsigned int i = 0; i < (*it)->GetNumBerPatches(); i++) {
      mapaCurvas[(*it)]->InsertPatch(mapaPatches[(*it)->GetPatch(i)]);
    }
  }
}

Geometry::~Geometry() {
  // 1. apaga a lista de curvas
  while (!this->curvas.empty()) {
    CurveAdaptive *C = this->curvas.back();
    this->curvas.pop_back();
    delete C;
  }

  // 3. apaga a lista de patches
  while (!this->patches.empty()) {
    Patch *P = this->patches.back();
    this->patches.pop_back();
    delete P;
  }
}

/* Classe que define um patch de Coons
 */
#include "../../../include/data/patch/CoonsPatch.h"

void CoonsPatch::insereCurva(CurveAdaptive* c) { this->curvas.push_back(c); }

CurveAdaptive* CoonsPatch::getCurva(const unsigned int i) {
  // adicionar excessão caso i > curvas.size();
  /*list <Curva*>::iterator it = this->curvas.begin();

  advance ( it, i );

  return *it;*/

  return (i < this->curvas.size()) ? this->curvas[i] : NULL;
}

CoonsPatch::CoonsPatch() {}

CoonsPatch::CoonsPatch(CoonsPatch* antigo) : Patch(antigo) {}

CoonsPatch::CoonsPatch(vector<CurveAdaptive*> listaDeCurvas)
    : curvas(listaDeCurvas) {}

CoonsPatch::~CoonsPatch() {
  // 1. apaga a lista de curvas
  this->curvas.clear();
}

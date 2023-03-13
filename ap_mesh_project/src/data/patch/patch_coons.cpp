/* Classe que define um patch de Coons
 */
#include "../../../include/data/patch/patch_coons.h"

PatchCoons::PatchCoons() : Patch() {}

PatchCoons::PatchCoons(std::shared_ptr<PatchCoons> patch_coons)
    : Patch(patch_coons) {}

PatchCoons::PatchCoons(const vector<std::shared_ptr<CurveAdaptive>> curves)
    : Patch(), curves_(curves) {}

PatchCoons::~PatchCoons() {
  // 1. apaga a lista de curvas
  this->curves_.clear();
}

void PatchCoons::InsertCurve(std::shared_ptr<CurveAdaptive> curve) {
  this->curves_.push_back(curve);
}

unsigned int PatchCoons::GetNumBerCurves() const { return (int)curves_.size(); }

std::shared_ptr<CurveAdaptive> PatchCoons::GetCurve(
    const unsigned int position) {
  // adicionar excessão caso i > curvas.size();
  /*list <Curva*>::iterator it = this->curvas.begin();

  advance ( it, i );

  return *it;*/

  return (position < this->curves_.size()) ? this->curves_[position] : nullptr;
}

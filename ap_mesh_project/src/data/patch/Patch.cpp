/* Classe que define um patch
 */
#include "../../../include/data/patch/Patch.h"

unsigned long Patch::getId() const { return id; }

void Patch::setId(unsigned long value) { id = value; }

void Patch::setMalha(SubMesh* m) { this->malha = m; }

SubMesh* Patch::getMalha() const { return this->malha; }

Patch::Patch() { this->malha = NULL; }

Patch::Patch(Patch*) { this->malha = NULL; }

Patch::~Patch() {
  // não delete o que não for alocado com new!
  if (this->malha != NULL) delete this->malha;
}

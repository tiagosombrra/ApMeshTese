/* Classe que define um nó
 */
#include "../../include/data/Noh.h"

void Noh::insereElemento(ElementAdaptive* elem) {
  this->guarda_chuva.push_back(elem);
}

unsigned int Noh::getNumDeElementos() const { return (int)guarda_chuva.size(); }

ElementAdaptive* Noh::getElemento(const unsigned int i) {
  list<ElementAdaptive*>::iterator it = this->guarda_chuva.begin();
  advance(it, i);

  return *it;
}

void Noh::apagaGuardaChuva() { guarda_chuva.clear(); }

Noh::Noh() {}

Noh::Noh(double _x, double _y, double _z) : Ponto(_x, _y, _z) {}

Noh::Noh(double _x, double _y, double _z, unsigned long i)
    : Ponto(_x, _y, _z, i) {}

Noh::Noh(const Ponto& p) {
  this->id = p.id;
  this->x = p.x;
  this->y = p.y;
  this->z = p.z;
}

Noh::~Noh() {
  // 1. apaga a lista de elementos
  this->guarda_chuva.clear();
}

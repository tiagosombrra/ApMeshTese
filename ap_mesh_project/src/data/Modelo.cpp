/* Classe que define um modelo
 */
#include "../../include/data/Modelo.h"

void Modelo::setGeometria(Geometry* g) { this->geometria = g; }

Geometry* Modelo::getGeometria() { return this->geometria; }

void Modelo::insereMalha(MeshAdaptive* m) { this->malhas.push_back(m); }

unsigned int Modelo::getNumDeMalhas() const { return (int)malhas.size(); }

MeshAdaptive* Modelo::getMalha(const unsigned int i) {
  // adicionar excessão caso i > malhas.size();
  /*list <Malha*>::iterator it = this->malhas.begin();

  advance ( it, i );

  return *it;*/

  return (i < this->malhas.size()) ? this->malhas[i] : NULL;
}

Modelo::Modelo() {}

Modelo::Modelo(Geometry* g) { this->geometria = g; }

Modelo::~Modelo() {
  // 1. apaga a lista de malha
  while (!this->malhas.empty()) {
    MeshAdaptive* M = this->malhas.back();
    this->malhas.pop_back();
    delete M;
  }

  //  cout<<"saida Modelo"<<endl;
}

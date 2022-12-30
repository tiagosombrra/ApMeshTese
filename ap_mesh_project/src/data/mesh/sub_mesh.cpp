
#include "../../../include/data/mesh/sub_mesh.h"

SubMesh::SubMesh() {}

SubMesh::~SubMesh() {
  // 1. apaga a lista de nós
  while (!this->nos_.empty()) {
    NodeAdaptive* noh = this->nos_.back();
    this->nos_.pop_back();
    delete noh;
  }

  // 2. apaga a lista de elementos
  while (!this->elements_.empty()) {
    ElementAdaptive* element = this->elements_.back();
    this->elements_.pop_back();
    delete element;
  }
}

Patch* SubMesh::GetPatch() const { return this->patch_; }

void SubMesh::SetPatch(Patch* patch) { this->patch_ = patch; }

NodeAdaptive* SubMesh::GetNoh(const unsigned int position) {
  /*// adicionar excessão caso i > nos.size();
  list <Noh*>::iterator it = this->nos.begin();

  advance ( it, i );

  return *it;*/
  return (position < this->nos_.size()) ? this->nos_[position] : nullptr;
}

void SubMesh::SetNoh(NodeAdaptive* noh) { this->nos_.push_back(noh); }

ElementAdaptive* SubMesh::GetElement(const unsigned int i) {
  // adicionar excessão caso i > elementos.size();
  /*list <Elemento*>::iterator it = this->elementos.begin();

  advance ( it, i );

  return *it;*/
  return (i < this->elements_.size()) ? this->elements_[i] : nullptr;
}

void SubMesh::SetElement(ElementAdaptive* element) {
  //    if (position > this->elementos.size()) {
  //        this->elementos.resize(position,NULL);
  //    }
  //    this->elementos.insert(this->elementos.begin()+position, elem);
  this->elements_.push_back(element);
}

unsigned int SubMesh::GetNumberNos() const { return (unsigned int)nos_.size(); }

unsigned int SubMesh::GetNumberElements() const {
  return (unsigned int)elements_.size();
}


#include "../../../include/data/mesh/sub_mesh.h"

#include <memory>

SubMesh::SubMesh() : patch_(nullptr), nos_({}), elements_({}) {}

SubMesh::~SubMesh() {
  // 1. apaga a lista de nós
  while (!this->nos_.empty()) {
    std::shared_ptr<NodeAdaptive> noh = this->nos_.back();
    this->nos_.pop_back();
  }

  // 2. apaga a lista de elementos
  while (!this->elements_.empty()) {
    std::shared_ptr<ElementAdaptive> element = this->elements_.back();
    this->elements_.pop_back();
  }
}

std::shared_ptr<Patch> SubMesh::GetPatch() const { return this->patch_; }

void SubMesh::SetPatch(std::shared_ptr<Patch> patch) { this->patch_ = patch; }

std::shared_ptr<NodeAdaptive> SubMesh::GetNoh(const unsigned int position) {
  return (position < this->nos_.size()) ? this->nos_[position] : nullptr;
}

void SubMesh::SetNoh(std::shared_ptr<NodeAdaptive> noh) {
  this->nos_.push_back(noh);
}

std::shared_ptr<ElementAdaptive> SubMesh::GetElement(const unsigned int i) {
  return (i < this->elements_.size()) ? this->elements_[i] : nullptr;
}

void SubMesh::SetElement(std::shared_ptr<ElementAdaptive> element) {
  this->elements_.push_back(element);
}

unsigned int SubMesh::GetNumberNos() const { return (unsigned int)nos_.size(); }

unsigned int SubMesh::GetNumberElements() const {
  return (unsigned int)elements_.size();
}

vector<std::shared_ptr<NodeAdaptive>> SubMesh::GetNos() { return nos_; }

vector<std::shared_ptr<ElementAdaptive>> SubMesh::GetElements() {
  return elements_;
}

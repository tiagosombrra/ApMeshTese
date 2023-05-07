
#include "../../../include/data/mesh/sub_mesh.h"

SubMesh::SubMesh() : patch_(nullptr), nos_({}), elements_({}) {}

SubMesh::~SubMesh() {}

std::shared_ptr<Patch> SubMesh::GetPatch() const { return this->patch_; }

void SubMesh::SetPatch(std::shared_ptr<Patch> patch) { this->patch_ = patch; }

std::shared_ptr<NodeAdaptive> SubMesh::GetNoh(const unsigned int position) {
  return (position < this->nos_.size()) ? this->nos_[position]
                                        : this->nos_.back();
}

void SubMesh::SetNoh(std::shared_ptr<NodeAdaptive>& noh) {
  this->nos_.push_back(noh);
}

std::shared_ptr<ElementAdaptive> SubMesh::GetElement(
    const unsigned int position) {
  return (position < this->elements_.size()) ? this->elements_[position]
                                             : this->elements_.back();
}

void SubMesh::SetElement(std::shared_ptr<ElementAdaptive> element) {
  this->elements_.push_back(element);
}

unsigned int SubMesh::GetNumberNos() const { return (unsigned int)nos_.size(); }

unsigned int SubMesh::GetNumberElements() const {
  return (unsigned int)elements_.size();
}

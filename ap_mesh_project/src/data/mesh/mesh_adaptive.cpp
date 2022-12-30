#include "../../../include/data/mesh/mesh_adaptive.h"

MeshAdaptive::MeshAdaptive() {}

MeshAdaptive::~MeshAdaptive() {
  // 1. apaga a lista de SubMalhas
  while (!this->sub_meshes_.empty()) {
    // SubMalha* SM = this->subMalhas.back ( );
    this->sub_meshes_.pop_back();
    // delete SM;
  }
}

void MeshAdaptive::InsertSubMeshAdaptiveByPosition(SubMesh* sub_mesh_adaptive,
                                                   const signed int position) {
  this->sub_meshes_[position] = sub_mesh_adaptive;
}

void MeshAdaptive::InsertSubMeshAdaptive(SubMesh* sub_mesh_adaptive) {
  this->sub_meshes_.push_back(sub_mesh_adaptive);
}

void MeshAdaptive::RemoveSubMeshAdaptive() { this->sub_meshes_.pop_back(); }

unsigned int MeshAdaptive::GetNumberSubMeshesAdaptive() const {
  return (unsigned int)this->sub_meshes_.size();
}

SubMesh* MeshAdaptive::GetSubMeshAdaptiveByPosition(
    const unsigned int position) {
  return (position < this->sub_meshes_.size()) ? this->sub_meshes_[position]
                                               : nullptr;
}

void MeshAdaptive::ResizeSubMeshAdaptiveByPosition(const signed int new_size) {
  this->sub_meshes_.resize(new_size, nullptr);
}

// Copyright (c) 2023 Tiago Guimarães Sombra. Todos os direitos reservados.
#include "../../../include/data/mesh/mesh_adaptive.h"

MeshAdaptive::MeshAdaptive() : sub_meshes_({}) {}

void MeshAdaptive::InsertSubMeshAdaptiveByPosition(
    std::shared_ptr<SubMesh> sub_mesh_adaptive, const signed int position) {
  this->sub_meshes_[position] = sub_mesh_adaptive;
}

void MeshAdaptive::InsertSubMeshAdaptive(
    std::shared_ptr<SubMesh> sub_mesh_adaptive) {
  this->sub_meshes_.push_back(sub_mesh_adaptive);
}

void MeshAdaptive::RemoveSubMeshAdaptive() { this->sub_meshes_.pop_back(); }

unsigned int MeshAdaptive::GetNumberSubMeshesAdaptive() const {
  return (unsigned int)this->sub_meshes_.size();
}

std::shared_ptr<SubMesh> MeshAdaptive::GetSubMeshAdaptiveByPosition(
    const unsigned int position) {
  return (position < this->sub_meshes_.size()) ? this->sub_meshes_[position]
                                               : nullptr;
}

void MeshAdaptive::ResizeSubMeshAdaptiveByPosition(const signed int new_size) {
  this->sub_meshes_.resize(new_size, nullptr);
}

std::vector<std::shared_ptr<SubMesh>> MeshAdaptive::GetSubMeshAdaptive() {
  return sub_meshes_;
}

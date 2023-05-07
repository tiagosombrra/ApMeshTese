// Copyright (c) 2023 Tiago Sombra
#include "../../include/data/model.h"

#include <algorithm>

Model::Model() {}

Model::~Model() {}

Model::Model(std::shared_ptr<Geometry>& geometry) : geometry_(geometry) {}

void Model::SetGeometry(std::shared_ptr<Geometry>& geometry) {
  geometry_ = geometry;
}

std::shared_ptr<Geometry> Model::GetGeometry() { return std::move(geometry_); }

void Model::InsertMeshAdaptive(std::shared_ptr<MeshAdaptive>& mesh_adaptive) {
  meshes_.push_back(mesh_adaptive);
}

uint64_t Model::GetNumberMeshes() const {
  return static_cast<uint64_t>(meshes_.size());
}

std::shared_ptr<MeshAdaptive> Model::GetMeshAdaptive(const uint64_t position) {
  return (position < this->meshes_.size()) ? std::move(this->meshes_[position])
                                           : nullptr;
}

// Copyright (c) 2023 Tiago Sombra
#include "../../include/data/model.h"
#include <algorithm>

Model::Model(std::unique_ptr<Geometry> geometry) : geometry_(std::move(geometry)) {}

void Model::SetGeometry(std::unique_ptr<Geometry> geometry) {
  geometry_ = std::move(geometry);
}

std::unique_ptr<Geometry> Model::GetGeometry() { return std::move(geometry_); }

void Model::InsertMeshAdaptive(std::unique_ptr<MeshAdaptive> mesh_adaptive) {
  meshes_.emplace_back(std::move(mesh_adaptive));
}

uint64_t Model::GetNumberMeshes() const {
  return static_cast<uint64_t>(meshes_.size());
}

std::unique_ptr<MeshAdaptive> Model::GetMeshAdaptive(const uint64_t position) {
  return (position < this->meshes_.size()) ? std::move(this->meshes_[position]) : nullptr;
}

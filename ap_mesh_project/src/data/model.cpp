#include "../../include/data/model.h"

Model::Model() : geometry_(nullptr) {}

Model::Model(std::shared_ptr<Geometry> geometry)
    : geometry_(std::move(geometry)) {}

void Model::SetGeometry(std::shared_ptr<Geometry> geometry) {
  this->geometry_ = std::move(geometry);
}

std::shared_ptr<Geometry> Model::GetGeometry() { return this->geometry_; }

void Model::InsertMeshAdaptive(std::shared_ptr<MeshAdaptive> mesh_adaptive) {
  this->meshes_.push_back(mesh_adaptive);
}

unsigned int Model::GetNumberMeshes() const {
  return static_cast<unsigned int>(meshes_.size());
}

std::shared_ptr<MeshAdaptive> Model::GetMeshAdaptive(
    const unsigned int position) {
  return (position < this->meshes_.size()) ? this->meshes_[position] : nullptr;
}

#include "../../include/data/model.h"

Model::Model() : geometry_(nullptr) {}

Model::Model(std::shared_ptr<Geometry> geometry) : geometry_(std::move(geometry)) {}

Model::~Model() {
  // 1. apaga a lista de malha
  while (!this->meshes_.empty()) {
    MeshAdaptive* mesh = this->meshes_.back();
    this->meshes_.pop_back();
    delete mesh;
  }
}

void Model::SetGeometry(std::shared_ptr<Geometry> geometry) {
    this->geometry_ = std::move(geometry);
}

std::shared_ptr<Geometry> Model::GetGeometry() { return this->geometry_; }

void Model::InsertMeshAdaptive(MeshAdaptive* mesh_adaptive) {
  this->meshes_.push_back(mesh_adaptive);
}

unsigned int Model::GetNumberMeshes() const {
  return static_cast<unsigned int>(meshes_.size());
}

MeshAdaptive* Model::GetMeshAdaptive(const unsigned int position) {
  return (position < this->meshes_.size()) ? this->meshes_[position] : nullptr;
}

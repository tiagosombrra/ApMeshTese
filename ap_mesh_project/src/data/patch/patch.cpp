#include "../../../include/data/patch/patch.h"

Patch::Patch() : id_(0), sub_mesh_(nullptr) {}

Patch::Patch(Patch* patch) : id_(patch->GetId()), sub_mesh_(nullptr) {}

Patch::~Patch() {
  // não delete o que não for alocado com new!
  if (this->sub_mesh_ != nullptr) {
    delete this->sub_mesh_;
  }
}

unsigned int Patch::GetId() const { return id_; }

void Patch::SetId(unsigned int id) { id_ = id; }

SubMesh* Patch::GetSubMesh() const { return this->sub_mesh_; }

void Patch::SetSubMesh(SubMesh* sub_mesh) { this->sub_mesh_ = sub_mesh; }

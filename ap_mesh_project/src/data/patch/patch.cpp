#include "../../../include/data/patch/patch.h"

Patch::Patch() : id_(0), sub_mesh_(nullptr) {}

Patch::Patch(std::shared_ptr<Patch> patch)
    : id_(patch->GetId()), sub_mesh_(patch->sub_mesh_) {}

Patch::~Patch() {}

unsigned int Patch::GetId() const { return id_; }

void Patch::SetId(unsigned int id) { id_ = id; }

std::shared_ptr<SubMesh> Patch::GetSubMesh() const { return this->sub_mesh_; }

void Patch::SetSubMesh(std::shared_ptr<SubMesh>& sub_mesh) {
  this->sub_mesh_ = sub_mesh;
}

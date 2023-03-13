#ifndef DATA_PATCH_PATCH_H
#define DATA_PATCH_PATCH_H

#include <memory>

#include "../../data/mesh/sub_mesh.h"

// class SubMalha;

class Patch {
 public:
  Patch();
  explicit Patch(std::shared_ptr<Patch> patch);
  ~Patch() = default;

  unsigned int GetId() const;
  void SetId(unsigned int id);
  std::shared_ptr<SubMesh> GetSubMesh() const;
  void SetSubMesh(std::shared_ptr<SubMesh> sub_mesh);

 protected:
  unsigned int id_;
  std::shared_ptr<SubMesh> sub_mesh_;
};

#endif  // DATA_PATCH_PATCH_H

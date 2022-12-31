#ifndef DATA_PATCH_PATCH_H
#define DATA_PATCH_PATCH_H

#include "../../data/mesh/sub_mesh.h"

// class SubMalha;

class Patch {
 public:
  Patch();
  explicit Patch(Patch* patch);
  ~Patch();

  unsigned int GetId() const;
  void SetId(unsigned int id);
  SubMesh* GetSubMesh() const;
  void SetSubMesh(SubMesh* sub_mesh);

 protected:
  unsigned int id_;
  SubMesh* sub_mesh_;
};

#endif  // DATA_PATCH_PATCH_H

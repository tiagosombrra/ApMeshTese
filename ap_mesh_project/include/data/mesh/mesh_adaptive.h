#ifndef DATA_MESH_MESH_ADAPTIVE_H
#define DATA_MESH_MESH_ADAPTIVE_H

#include <iostream>
#include <vector>

#include "sub_mesh.h"

class MeshAdaptive {
 public:
  MeshAdaptive();
  ~MeshAdaptive() = default;

  void InsertSubMeshAdaptiveByPosition(SubMesh* sub_mesh_adaptive,
                                       const signed int position);
  void InsertSubMeshAdaptive(SubMesh* sub_mesh_adaptive);
  void RemoveSubMeshAdaptive();
  unsigned int GetNumberSubMeshesAdaptive() const;
  SubMesh* GetSubMeshAdaptiveByPosition(const unsigned int position);
  void ResizeSubMeshAdaptiveByPosition(const signed int new_size);

 protected:
  vector<SubMesh*> sub_meshes_;
};

#endif  // DATA_MESH_MESH_ADAPTIVE_H

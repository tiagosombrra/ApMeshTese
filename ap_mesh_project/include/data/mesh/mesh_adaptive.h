#ifndef DATA_MESH_MESH_ADAPTIVE_H
#define DATA_MESH_MESH_ADAPTIVE_H

#include <iostream>
#include <memory>
#include <vector>

#include "sub_mesh.h"

class MeshAdaptive {
 public:
  MeshAdaptive();
  ~MeshAdaptive();

  void InsertSubMeshAdaptiveByPosition(
      std::shared_ptr<SubMesh>& sub_mesh_adaptive, const signed int position);
  void InsertSubMeshAdaptive(std::shared_ptr<SubMesh>& sub_mesh_adaptive);
  void RemoveSubMeshAdaptive();
  unsigned int GetNumberSubMeshesAdaptive() const;
  std::shared_ptr<SubMesh> GetSubMeshAdaptiveByPosition(
      const unsigned int position);
  void ResizeSubMeshAdaptiveByPosition(const signed int new_size);

 protected:
  std::vector<std::shared_ptr<SubMesh>> sub_meshes_;
};

#endif  // DATA_MESH_MESH_ADAPTIVE_H

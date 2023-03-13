// Copyright (c) 2023 Tiago Guimarães Sombra. Todos os direitos reservados.
#ifndef AP_MESH_PROJECT_INCLUDE_DATA_MESH_MESH_ADAPTIVE_H_
#define AP_MESH_PROJECT_INCLUDE_DATA_MESH_MESH_ADAPTIVE_H_

#include <iostream>
#include <memory>
#include <vector>

#include "sub_mesh.h"

class MeshAdaptive {
 public:
  MeshAdaptive();
  ~MeshAdaptive() = default;

  void InsertSubMeshAdaptiveByPosition(
      std::shared_ptr<SubMesh> sub_mesh_adaptive, const signed int position);
  void InsertSubMeshAdaptive(std::shared_ptr<SubMesh> sub_mesh_adaptive);
  void RemoveSubMeshAdaptive();
  unsigned int GetNumberSubMeshesAdaptive() const;
  std::shared_ptr<SubMesh> GetSubMeshAdaptiveByPosition(
      const unsigned int position);
  void ResizeSubMeshAdaptiveByPosition(const signed int new_size);
  std::vector<std::shared_ptr<SubMesh>> GetSubMeshAdaptive();

 protected:
  vector<std::shared_ptr<SubMesh>> sub_meshes_;
};

#endif  // AP_MESH_PROJECT_INCLUDE_DATA_MESH_MESH_ADAPTIVE_H_

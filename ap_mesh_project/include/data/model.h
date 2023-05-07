// Copyright (c) 2023 Tiago Sombra
#ifndef AP_MESH_PROJECT_INCLUDE_DATA_MODEL_H_
#define AP_MESH_PROJECT_INCLUDE_DATA_MODEL_H_

#include <memory>
#include <vector>

#include "../data/mesh/mesh_adaptive.h"
#include "geometry.h"

class Model {
 public:
  Model();
  ~Model();

  explicit Model(std::shared_ptr<Geometry>& geometry);
  void SetGeometry(std::shared_ptr<Geometry>& geometry);
  std::shared_ptr<Geometry> GetGeometry();
  void InsertMeshAdaptive(std::shared_ptr<MeshAdaptive>& mesh_adaptive);
  uint64_t GetNumberMeshes() const;
  std::shared_ptr<MeshAdaptive> GetMeshAdaptive(const uint64_t position);

 protected:
  std::shared_ptr<Geometry> geometry_;
  std::vector<std::shared_ptr<MeshAdaptive>> meshes_;
};

#endif  // AP_MESH_PROJECT_INCLUDE_DATA_MODEL_H_

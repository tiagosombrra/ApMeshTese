// Copyright (c) 2023 Tiago Sombra
#ifndef AP_MESH_PROJECT_INCLUDE_DATA_MODEL_H_
#define AP_MESH_PROJECT_INCLUDE_DATA_MODEL_H_

#include <memory>
#include <vector>

#include "../data/mesh/mesh_adaptive.h"
#include "geometry.h"

class Model {
 public:
  Model() = default;
  explicit Model(std::unique_ptr<Geometry> geometry);
  ~Model() = default;

  void SetGeometry(std::unique_ptr<Geometry> geometry);
  std::unique_ptr<Geometry> GetGeometry();
  void InsertMeshAdaptive(std::unique_ptr<MeshAdaptive> mesh_adaptive);
  uint64_t GetNumberMeshes() const;
  std::unique_ptr<MeshAdaptive> GetMeshAdaptive(const uint64_t position);

 protected:
  std::unique_ptr<Geometry> geometry_;
  std::vector<std::unique_ptr<MeshAdaptive>> meshes_;
};

#endif  // AP_MESH_PROJECT_INCLUDE_DATA_MODEL_H_

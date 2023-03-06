#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <memory>

#include "../data/mesh/mesh_adaptive.h"
#include "geometry.h"

class Model {
 public:
  Model();
  explicit  Model(std::shared_ptr<Geometry> geometry);
  ~Model();

  void SetGeometry(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> GetGeometry();
  void InsertMeshAdaptive(MeshAdaptive* mesh_adaptive);
  unsigned int GetNumberMeshes() const;
  MeshAdaptive* GetMeshAdaptive(const unsigned int position);

 protected:
  std::shared_ptr<Geometry> geometry_;
  vector<MeshAdaptive*> meshes_;
};

#endif  // DATA_MODEL_H

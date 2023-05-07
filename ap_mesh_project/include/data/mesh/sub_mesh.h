
#ifndef DATA_MESH_SUB_MESH_H
#define DATA_MESH_SUB_MESH_H

#include <memory>
#include <vector>

#include "../definitions.h"
#include "../element_adaptive.h"
#include "../node_adaptive.h"

class Patch;

class SubMesh {
 public:
  SubMesh();
  ~SubMesh();

  std::shared_ptr<Patch> GetPatch() const;
  void SetPatch(std::shared_ptr<Patch> patch);
  std::shared_ptr<NodeAdaptive> GetNoh(const unsigned int position);
  void SetNoh(std::shared_ptr<NodeAdaptive> &noh);
  std::shared_ptr<ElementAdaptive> GetElement(const unsigned int position);
  void SetElement(std::shared_ptr<ElementAdaptive> element);
  unsigned int GetNumberNos() const;
  unsigned int GetNumberElements() const;

 protected:
  std::shared_ptr<Patch> patch_;
  std::vector<std::shared_ptr<NodeAdaptive>> nos_;
  std::vector<std::shared_ptr<ElementAdaptive>> elements_;
};

#endif  // DATA_MESH_SUB_MESH_H

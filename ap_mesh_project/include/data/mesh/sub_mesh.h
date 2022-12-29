
#ifndef DATA_MESH_SUB_MESH_H
#define DATA_MESH_SUB_MESH_H

#include <vector>

#include "../definitions.h"
#include "../element_adaptive.h"
#include "../noh.h"

class Patch;

class SubMesh {
 public:
  SubMesh();
  ~SubMesh();

  Patch* GetPatch() const;
  void SetPatch(Patch* patch);
  Noh* GetNoh(const unsigned int position);
  void SetNoh(Noh* noh);
  ElementAdaptive* GetElement(const unsigned int position);
  void SetElement(ElementAdaptive* element);
  unsigned int GetNumberNos() const;
  unsigned int GetNumberElements() const;

 protected:
  Patch* patch_;
  vector<Noh*> nos_;
  vector<ElementAdaptive*> elements_;
};

#endif  // DATA_MESH_SUB_MESH_H

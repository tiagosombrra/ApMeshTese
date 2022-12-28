
#ifndef DATA_MESH_SUB_MESH_H
#define DATA_MESH_SUB_MESH_H

#include <vector>

#include "../Definitions.h"
#include "../Elemento.h"
#include "../Noh.h"

class Patch;

class SubMesh {
 public:
  SubMesh();
  ~SubMesh();

  Patch* GetPatch() const;
  void SetPatch(Patch* patch);
  Noh* GetNoh(const unsigned int position);
  void SetNoh(Noh* noh);
  Elemento* GetElement(const unsigned int position);
  void SetElement(Elemento* element);
  unsigned int GetNumberNos() const;
  unsigned int GetNumberElements() const;

 protected:
  Patch* patch_;
  vector<Noh*> nos_;
  vector<Elemento*> elements_;
};

#endif  // DATA_MESH_SUB_MESH_H

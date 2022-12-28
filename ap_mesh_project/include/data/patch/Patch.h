/* Classe que define um patch
 */
#ifndef Patch_h
#define Patch_h

#include "../../data/mesh/sub_mesh.h"

// class SubMalha;

class Patch {
 protected:
  SubMesh* malha;

 public:
  unsigned long id;
  void setMalha(SubMesh* m);
  SubMesh* getMalha() const;

  Patch();
  Patch(Patch*);
  ~Patch();
  unsigned long getId() const;
  void setId(unsigned long value);
};

#endif

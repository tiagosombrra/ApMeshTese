#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "../../definitions.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::Basics;

namespace Par2DJMesh {
namespace Basics {
class Shape {
 protected:
  long int id;

 public:
  Shape(long int id = 0);
  virtual ~Shape();

  void setId(long int id);
  long int getId();
  virtual string getText() = 0;
};
}  // namespace Basics
}  // namespace Par2DJMesh

#endif  // #ifndef _SHAPE_H_

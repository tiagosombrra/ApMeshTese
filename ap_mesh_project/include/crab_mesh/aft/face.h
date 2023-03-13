#ifndef _FACE_H_
#define _FACE_H_

#include <memory>

#include "../../data/definitions.h"
#include "edge.h"
#include "shape.h"
#include "vertex.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::Basics;

namespace Par2DJMesh {
namespace Basics {
class Face : public Shape, std::enable_shared_from_this<Face> {
 private:
  std::shared_ptr<Vertex> v[3];
  std::shared_ptr<Vertex> mid;

 private:
  std::shared_ptr<Vertex> makeMid();

 public:
  double h;

  Face(std::shared_ptr<Vertex> v1 = NULL, std::shared_ptr<Vertex> v2 = NULL,
       std::shared_ptr<Vertex> v3 = NULL, long int id = 0);
  Face(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
       std::shared_ptr<Vertex> v3, std::shared_ptr<Vertex> mid, long int id);
  ~Face() = default;

  void setV1(std::shared_ptr<Vertex> v);
  void setV2(std::shared_ptr<Vertex> v);
  void setV3(std::shared_ptr<Vertex> v);
  void setVertices(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
                   std::shared_ptr<Vertex> v3);
  std::shared_ptr<Vertex> getV1();
  std::shared_ptr<Vertex> getV2();
  std::shared_ptr<Vertex> getV3();

  std::shared_ptr<Vertex> getMid();
  std::shared_ptr<Vertex> circumcenter();
  std::shared_ptr<Vertex> incenter();

  double surface();
  double orientedSurface();

  bool in(double x, double y);
  bool in(std::shared_ptr<Vertex> v);
  bool on(std::shared_ptr<Vertex> v);
  bool out(double x, double y);
  bool out(std::shared_ptr<Vertex> v);

  void barycentricCoordinates(std::shared_ptr<Vertex> v, double &l1, double &l2,
                              double &l3);

  double gama();
  double quality();
  bool isBad();

  bool hasEdge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  bool hasEdge(std::shared_ptr<Edge> e);
  bool isAdjacent(std::shared_ptr<Face> f);

  // testa se uma face intercepta outra
  bool hits(std::shared_ptr<Face> f);
  bool hits(std::shared_ptr<Edge> e);

  string getText();
};
}  // namespace Basics
}  // namespace Par2DJMesh

#endif  // #ifndef _FACE_H_

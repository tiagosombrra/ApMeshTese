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
class Face : public Shape {
 private:
  std::shared_ptr<Vertex> v[3];
  std::shared_ptr<Vertex> mid;

 private:
  std::shared_ptr<Vertex> makeMid();

 public:
  double h;

  Face(std::shared_ptr<Vertex> v1 = nullptr,
       std::shared_ptr<Vertex> v2 = nullptr,
       std::shared_ptr<Vertex> v3 = nullptr, long int id = 0);
  Face(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
       std::shared_ptr<Vertex> v3, std::shared_ptr<Vertex> mid, long int id);
  ~Face();

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
  bool in(Vertex v);
  bool on(Vertex v);
  bool out(double x, double y);
  bool out(Vertex v);

  void barycentricCoordinates(Vertex v, double &l1, double &l2, double &l3);

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

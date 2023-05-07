#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <memory>

#include "../../definitions.h"
#include "edge.h"
#include "shape.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::Basics;

namespace Par2DJMesh {
namespace Basics {
class Vertex : public Shape {
 private:
  double x;
  double y;

  EdgeSet adjacentEdges;

 public:
  Vertex(double x = 0.0, double y = 0.0, long int id = 0);
  ~Vertex();

  void setX(double x);
  void setY(double y);
  void setPosition(double x, double y);
  double getX();
  double getY();
  void getPosition(double *x, double *y);

  void addAdjacentEdge(std::shared_ptr<Edge> e);
  void removeAdjacentEdge(std::shared_ptr<Edge> e);
  EdgeSet getAdjacentEdges();

  void sum(std::shared_ptr<Vertex> v);
  void scalarMultiplication(double d);

  double dot(Vertex v);
  double cross(Vertex v);

  double norm();

  double distance(Vertex v);
  double distance(std::shared_ptr<Vertex> v);

  double angle(Vertex v);
  double pseudoAngle();
  double pseudoAngle(Vertex v);
  double squarePseudoAngle();

  double orientedAngle();
  double orientedAngle(std::shared_ptr<Vertex> v);

  bool left(Vertex v);
  bool right(Vertex v);

  double surface(Vertex v1, Vertex v2);
  double orientedSurface(Vertex v1, Vertex v2);

  bool counterClockWise(Vertex v1, Vertex v2);
  bool clockWise(Vertex v1, Vertex v2);

  bool equals(double x, double y);
  bool matches(std::shared_ptr<Vertex> v);

  string getText() override;
};
}  // namespace Basics
}  // namespace Par2DJMesh

#endif  // #ifndef _VERTEX_H_

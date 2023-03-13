#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <memory>

#include "../../data/definitions.h"
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
  void getPosition(std::shared_ptr<double> x, std::shared_ptr<double> y);

  // #if USE_OPENGL
  //     void setSize(double size);
  // #endif //#if USE_OPENGL

  void addAdjacentEdge(std::shared_ptr<Edge> e);
  void removeAdjacentEdge(std::shared_ptr<Edge> e);
  EdgeSet getAdjacentEdges();

  void sum(std::shared_ptr<Vertex> v);
  void scalarMultiplication(double d);

  double dot(std::shared_ptr<Vertex> v);
  double cross(std::shared_ptr<Vertex> v);

  double norm();

  double distance(std::shared_ptr<Vertex> v);

  double angle(std::shared_ptr<Vertex> v);
  double pseudoAngle();
  double pseudoAngle(std::shared_ptr<Vertex> v);
  double squarePseudoAngle();

  double orientedAngle();
  double orientedAngle(std::shared_ptr<Vertex> v);

  bool left(std::shared_ptr<Vertex> v);
  bool right(std::shared_ptr<Vertex> v);

  double surface(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  double orientedSurface(std::shared_ptr<Vertex> v1,
                         std::shared_ptr<Vertex> v2);

  bool counterClockWise(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  bool clockWise(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);

  bool equals(double x, double y);
  bool matches(std::shared_ptr<Vertex> v);

  string getText() override;
};
}  // namespace Basics
}  // namespace Par2DJMesh

#endif  // #ifndef _VERTEX_H_

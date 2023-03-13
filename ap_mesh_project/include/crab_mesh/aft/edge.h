#ifndef _EDGE_H_
#define _EDGE_H_

#include <cstdint>
#include <memory>

#include "../../data/curve/curve_adaptive_parametric.h"
#include "../../data/definitions.h"
#include "quadtree_cell.h"
#include "shape.h"
#include "vertex.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::Basics;

namespace Par2DJMesh {
namespace Basics {
using namespace Par2DJMesh::AFT;

class Edge : public Shape {
 private:
  bool free;
  bool inBoundary;
  std::shared_ptr<Vertex> v[2];
  std::shared_ptr<Vertex> mid;
  std::shared_ptr<Vertex> vector;
  double len;

  std::shared_ptr<QuadtreeCell> cell;

  double xmin;
  double xmax;
  double ymin;
  double ymax;

  // #if USE_OPENGL
  //     double width;
  // #endif //#if USE_OPENGL

  // Daniel Siqueira
  // ponteiro para a curva
  std::shared_ptr<CurveAdaptiveParametric> c;

 public:
  std::shared_ptr<Vertex> makeMid();
  std::shared_ptr<Vertex> makeVector();

 public:
  Edge(std::shared_ptr<Vertex> v1 = nullptr,
       std::shared_ptr<Vertex> v2 = nullptr, std::uint64_t id = 0);
  ~Edge() = default;

  void setV1(std::shared_ptr<Vertex> v);
  void setV2(std::shared_ptr<Vertex> v);
  void setVertices(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  std::shared_ptr<Vertex> getV1();
  std::shared_ptr<Vertex> getV2();

  void setInBoundary(bool inBoundary);
  bool isInBoundary();

  // #if USE_OPENGL
  //     void setWidth(double width);
  // #endif //#if USE_OPENGL

  void setFree(bool free);
  bool isFree();

  void setCell(std::shared_ptr<QuadtreeCell> cell);
  std::shared_ptr<QuadtreeCell> getCell();

  std::shared_ptr<Vertex> getMid();

  double length();

  bool intercept(std::shared_ptr<Edge> e);
  bool intercept(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  bool intercept(std::shared_ptr<Vertex> v);
  bool intercept(double x, double y);

  double straightDistance(std::shared_ptr<Vertex> v);
  double distance(std::shared_ptr<Vertex> v);
  double distance(double x, double y);

  double dot(std::shared_ptr<Vertex> v);

  bool left(std::shared_ptr<Vertex> v);
  bool left(double x, double y);

  bool right(std::shared_ptr<Vertex> v);
  bool right(double x, double y);

  bool accordingToNormal(std::shared_ptr<Vertex> v, bool inEdgeTest = false);

  double angle(std::shared_ptr<Edge> e);
  double angle(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);

  std::shared_ptr<Vertex> normal();

  bool equals(std::shared_ptr<Edge> e);
  bool equals(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  bool matches(std::shared_ptr<Edge> e);
  bool matches(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);

  // metodos do Daniel Siqueira
  void setCurva(std::shared_ptr<CurveAdaptiveParametric>);
  void setLen(double len);
  double getLen();
  void makeParamMid();

  string getText() override;
};
}  // namespace Basics
}  // namespace Par2DJMesh

#endif  // #ifndef _EDGE_H_

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

#include <memory>

#include "../../data/curve/curve_adaptive_parametric.h"
#include "../../definitions.h"
#include "edge.h"
#include "quadtree.h"
#include "quadtree_cell.h"
#include "shape.h"
#include "vertex.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::AFT;
using namespace Par2DJMesh::Basics;

namespace Par2DJMesh {
namespace AFT {
class Boundary : public Shape {
 private:
  VertexList boundary;
  EdgeList edges;

  long int lastVertexId;
  long int lastEdgeId;

  std::shared_ptr<Vertex> first;

 private:
  std::shared_ptr<Edge> makeEdge(std::shared_ptr<Vertex> v);

 public:
  Boundary();
  ~Boundary();

  void setBoundary(VertexList boundary);
  VertexList getBoundary();

  void setEdges(EdgeList edges);
  EdgeList getEdges();

  long int getLastVertexId();
  long int getLastEdgeId();

  std::shared_ptr<Vertex> addVertex(double x, double y,
                                    std::shared_ptr<CurveAdaptiveParametric> c);
  std::shared_ptr<Vertex> addVertex(long int id, double x, double y);
  bool close(std::shared_ptr<CurveAdaptiveParametric> c);

  std::shared_ptr<Edge> getEdge(long int id);
  std::shared_ptr<Vertex> getVertex(long int id);

  void getBox(double &minX, double &minY, double &maxX, double &maxY);

  bool belongs(std::shared_ptr<Edge> e);
  bool belongs(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);

  string getText() override;
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // #ifndef _BOUNDARY_H_

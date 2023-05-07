#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <memory>

#include "../../definitions.h"
#include "boundary.h"
#include "edge.h"
#include "face.h"
#include "quadtree_cell.h"
#include "shape.h"
#include "vertex.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::AFT;
using namespace Par2DJMesh::Basics;

namespace Par2DJMesh {
namespace AFT {
class Quadtree : public Shape {
 private:
  double factor;

  QuadtreeCell *root;

  QuadtreeCellList leaves;

  std::shared_ptr<Boundary> boundary;

  // para a geracao baseada em templates
  EdgeList front;
  EdgeList edges;
  VertexList vertices;
  FaceList mesh;
  long int lastVertexId;
  long int lastEdgeId;
  long int lastFaceId;

 public:
  Quadtree(std::shared_ptr<Boundary> boundary = nullptr, double factor = 0.85);
  ~Quadtree();

  void setFactor(double factor);
  double getFactor();

  void setBoundary(std::shared_ptr<Boundary> boundary);
  std::shared_ptr<Boundary> getBoundary();

  void setRoot(QuadtreeCell *root);
  QuadtreeCell *getRoot();

  // retira cell como folha e adiciona os filhos de cell
  void addLeaves(QuadtreeCell *cell);
  QuadtreeCellList getLeaves();

  int getNumCells();

  std::shared_ptr<Vertex> getMin();
  std::shared_ptr<Vertex> getMax();

  long int getCellId();

  void findCell(std::shared_ptr<Edge> e);

  bool in(std::shared_ptr<Vertex> v);
  bool on(std::shared_ptr<Vertex> v);
  bool out(std::shared_ptr<Vertex> v);

  enum MethodStatus generate(FaceList &oldmesh);
  enum MethodStatus refineToLevel();
  enum MethodStatus refineAccordingToNeighbors();

  bool execute(FaceList &oldmesh);

  // para a geracao baseada em templates
  long int vertexId();
  long int edgeId();
  long int faceId();

  EdgeList getFront();
  EdgeList getEdges();
  VertexList getVertices();
  FaceList getMesh();

  void add(std::shared_ptr<Vertex> v);
  void add(std::shared_ptr<Edge> e);
  void add(std::shared_ptr<Face> f);
  void addFront(std::shared_ptr<Edge> e);

  enum MethodStatus makeTemplateBasedMesh();

  string getText() override;
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // #ifndef _QUADTREE_H_

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <cstdint>
#include <memory>
#include <string>

#include "../../data/definitions.h"
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
class Quadtree : public Shape, std::enable_shared_from_this<Quadtree> {
 private:
  double factor;

  std::shared_ptr<QuadtreeCell> root;

  QuadtreeCellList leaves;

  std::shared_ptr<Boundary> boundary;

  // para a geracao baseada em templates
  EdgeList front;
  EdgeList edges;
  VertexList vertices;
  FaceList mesh;
  std::uint64_t lastVertexId;
  std::uint64_t lastEdgeId;
  std::uint64_t lastFaceId;

 public:
  Quadtree(std::shared_ptr<Boundary> boundary = nullptr, double factor = 0.85);
  ~Quadtree();

  void setFactor(double factor);
  double getFactor();

  void setBoundary(std::shared_ptr<Boundary> boundary);
  std::shared_ptr<Boundary> getBoundary();

  void setRoot(std::shared_ptr<QuadtreeCell> root);
  std::shared_ptr<QuadtreeCell> getRoot();

  // retira cell como folha e adiciona os filhos de cell
  void addLeaves(std::shared_ptr<QuadtreeCell> cell);
  QuadtreeCellList getLeaves();

  int getNumCells();

  std::shared_ptr<Vertex> getMin();
  std::shared_ptr<Vertex> getMax();

  std::uint64_t getCellId();

  void findCell(std::shared_ptr<Edge> e);

  bool in(std::shared_ptr<Vertex> v);
  bool on(std::shared_ptr<Vertex> v);
  bool out(std::shared_ptr<Vertex> v);

  enum MethodStatus generate(const FaceList &oldmesh);
  enum MethodStatus refineToLevel();
  enum MethodStatus refineAccordingToNeighbors();

  bool execute(const FaceList &oldmesh);

  // para a geracao baseada em templates
  std::uint64_t vertexId();
  std::uint64_t edgeId();
  std::uint64_t faceId();

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

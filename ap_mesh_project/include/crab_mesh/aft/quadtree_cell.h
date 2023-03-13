#ifndef _QUADTREE_CELL_H_
#define _QUADTREE_CELL_H_

#include <cstdint>
#include <memory>
#include <string>

#include "../../data/definitions.h"
#include "boundary.h"
#include "edge.h"
#include "face.h"
#include "quadtree.h"
#include "shape.h"
#include "vertex.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::AFT;
using namespace Par2DJMesh::Basics;

extern double TOLERANCE_AFT;

namespace Par2DJMesh {
namespace AFT {
class QuadtreeCell : public Shape, std::enable_shared_from_this<QuadtreeCell> {
 private:
  std::uint64_t level;
  double size;
  bool highlighted;
  bool highlightedAsNeighbor;

  // para a geracao baseada em templates
  std::shared_ptr<Vertex> v[8];
  std::shared_ptr<Edge> e[8];

  std::shared_ptr<Vertex> min;
  std::shared_ptr<Vertex> max;
  std::shared_ptr<std::array<std::shared_ptr<Vertex>, 5>> mids;

  std::shared_ptr<QuadtreeCell> children[4];
  std::shared_ptr<QuadtreeCell> parent;

  std::shared_ptr<Quadtree> tree;

  QuadtreeCellList neighbors[4];

  EdgeList edges;

 private:
  std::shared_ptr<std::array<std::shared_ptr<Vertex>, 5>> makeMids();

  void subdivide(bool setChildrenEdges = true);

  void setChildrenNeighbors();

  void setChildrenEdges();

  // para a geracao baseada em templates
  std::shared_ptr<Edge> makeE(int i);
  std::shared_ptr<Edge> makeE(int i, int j);
  std::shared_ptr<Edge> makeE(std::shared_ptr<Vertex> v1,
                              std::shared_ptr<Vertex> v2);

  void makeMeshType1();
  void makeMeshType2(int c);
  void makeMeshType3(int c);
  void makeMeshType4(int c);
  void makeMeshType5(int c);
  void makeMeshType6();

 public:
  QuadtreeCell(std::uint64_t id, std::shared_ptr<Quadtree> tree = nullptr,
               std::shared_ptr<Vertex> min = nullptr,
               std::shared_ptr<Vertex> max = nullptr,
               std::shared_ptr<QuadtreeCell> parent = nullptr,
               std::uint64_t level = 0);
  ~QuadtreeCell();

  void setQuadtree(std::shared_ptr<Quadtree> tree);

  void setLevel(std::uint64_t level);
  std::uint64_t getLevel();

  double getSize();

  void setParent(std::shared_ptr<QuadtreeCell> parent);
  std::shared_ptr<QuadtreeCell> getParent();

  void setChild(int position, std::shared_ptr<QuadtreeCell> child);
  std::shared_ptr<QuadtreeCell> getChild(int position);

  void setBox(std::shared_ptr<Vertex> min, std::shared_ptr<Vertex> max);
  std::shared_ptr<Vertex> getMin();
  std::shared_ptr<Vertex> getMax();

  void setNeighbors(int direction, QuadtreeCellList neighbors);
  QuadtreeCellList getNeighbors(int direction);

  // retorna o numero de celulas da subarvore que tem this como raiz
  int getNumCells();

  // retorna os vertices das arestas internas à essa celula
  VertexList getVertices();
  EdgeList getEdges();

  bool shouldSubdivide(std::shared_ptr<Edge> e);
  bool shouldSubdivide(std::shared_ptr<Face> f);

  void addEdge(std::shared_ptr<Edge> e);
  void removeEdge(std::shared_ptr<Edge> e);
  void clearEdges();
  std::shared_ptr<QuadtreeCell> findCell(std::shared_ptr<Edge> e);
  std::shared_ptr<QuadtreeCell> findCell(std::shared_ptr<Face> f);

  bool in(std::shared_ptr<Vertex> v);
  bool on(std::shared_ptr<Vertex> v);
  bool out(std::shared_ptr<Vertex> v);

  double height();
  double surface();

  bool subdivide(std::shared_ptr<Edge> e);
  bool subdivide(std::shared_ptr<Face> f);
  void subdivideToLevel(std::uint64_t level);
  bool subdivideAccordingToNeighbors();

  bool subdivided();

  // para a geracao baseada em templates
  void sortNeighbors();
  bool inBoundary();

  void makeOuterVertices();
  void makeOuterEdges();
  void makeTemplateBasedMesh();

  string getText() override;
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // #ifndef _QUADTREE_CELL_H_

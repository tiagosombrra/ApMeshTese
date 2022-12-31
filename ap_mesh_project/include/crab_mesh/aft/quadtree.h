#ifndef _QUADTREE_H_
#define _QUADTREE_H_

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
class Quadtree : public Shape {
 private:
  double factor;

  QuadtreeCell *root;

  QuadtreeCellList leaves;

  Boundary *boundary;

  // para a geracao baseada em templates
  EdgeList front;
  EdgeList edges;
  VertexList vertices;
  FaceList mesh;
  long int lastVertexId;
  long int lastEdgeId;
  long int lastFaceId;

 public:
  Quadtree(Boundary *boundary = NULL, double factor = 0.85);
  ~Quadtree();

  void setFactor(double factor);
  double getFactor();

  void setBoundary(Boundary *boundary);
  Boundary *getBoundary();

  void setRoot(QuadtreeCell *root);
  QuadtreeCell *getRoot();

  // retira cell como folha e adiciona os filhos de cell
  void addLeaves(QuadtreeCell *cell);
  QuadtreeCellList getLeaves();

  int getNumCells();

  Vertex *getMin();
  Vertex *getMax();

  long int getCellId();

  void findCell(Edge *e);

  bool in(Vertex *v);
  bool on(Vertex *v);
  bool out(Vertex *v);

  enum MethodStatus generate(const FaceList &oldmesh);
  enum MethodStatus refineToLevel();
  enum MethodStatus refineAccordingToNeighbors();

  bool execute(const FaceList &oldmesh);

  // para a geracao baseada em templates
  long int vertexId();
  long int edgeId();
  long int faceId();

  EdgeList getFront();
  EdgeList getEdges();
  VertexList getVertices();
  FaceList getMesh();

  void add(Vertex *v);
  void add(Edge *e);
  void add(Face *f);
  void addFront(Edge *e);

  enum MethodStatus makeTemplateBasedMesh();

  string getText() override;

  // #if USE_OPENGL
  //     void highlight();
  //     void unhighlight();

  //    void draw();
  // #endif //#if USE_OPENGL
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // #ifndef _QUADTREE_H_

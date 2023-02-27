#ifndef ADVANCING_FRONT_H_
#define ADVANCING_FRONT_H_

#include "../../data/definitions.h"
#include "boundary.h"
#include "edge.h"
#include "face.h"
#include "quadtree.h"
#include "quadtree_cell.h"
#include "shape.h"
#include "vertex.h"

using namespace Par2DJMesh;
using namespace Par2DJMesh::AFT;
using namespace Par2DJMesh::Basics;

extern double TOLERANCE_AFT;
extern std::string USE_TEMPLATE;

namespace Par2DJMesh {
namespace AFT {
class AdvancingFront : public Shape {
 protected:
  long int lastVertexId;
  long int lastEdgeId;
  long int lastFaceId;

  bool boundarySorted;

  // quantidade de vezes que smoothing + local back-tracking, na
  // fase de improvement serao feitos
  double phi;
  unsigned int numImproves;

  Boundary *boundary;

  Quadtree *quadtree;

  FaceList mesh;

  EdgeList front;
  EdgeList innerEdges;
  EdgeList edges;
  EdgeList rejected;

  VertexList frontVertices;
  VertexList innerVertices;
  VertexList vertices;

 protected:
  void makeInitialFront();
  void sortFront();

  // Testa intersecoes com arestas jah existentes
  // e com as faces jah existentes
  bool interceptionTest(Edge *e, Vertex *candidate, bool inFaceTest = true,
                        bool onlyFrontEdges = false);
  bool interceptionTest(Edge *e);

  Vertex *makeIdealVertex(Edge *e, double &h);
  virtual bool findBestVertex(Edge *e, Vertex *&best, bool geometryPhase);

  void insertInFront(Edge *last, Edge *e);

  Edge *findEdge(Vertex *v1, Vertex *v2);
  EdgeList findAdjacentEdges(Vertex *v);
  FaceList findAdjacentFaces(const FaceList &faces, Vertex *v);

  void removeFromFront(Vertex *v1, Vertex *v2 = NULL, Vertex *v3 = NULL);

  virtual enum MethodStatus makeMesh(bool frontBased,
                                     bool geometryPhase = true);

  void fillMesh();

  bool laplacianSmoothing(bool &changed);

 public:
  AdvancingFront(double factor = 0.85, double tolerance = 1.e-8,
                 unsigned int numImproves = 5, double phi = 0.5);
  AdvancingFront(Boundary *boundary, Quadtree *quadtree,
                 double tolerance = 1.e-8, unsigned int numImproves = 5);

  virtual ~AdvancingFront();

  //    static double getTolerance();

  void setBoundarySorted(bool boundarySorted);
  bool isBoundarySorted();

  void setNumImproves(unsigned int numImproves);
  unsigned int getNumImproves();

  void setBoundary(Boundary *boundary);
  Boundary *getBoundary();

  void setQuadtree(Quadtree *quadtree);
  Quadtree *getQuadtree();

  VertexList getVertices();
  VertexList getInnerVertices();

  EdgeList getEdges();
  EdgeList getInnerEdges();

  FaceList getMesh();

  void addVertices(VertexList vertices);
  void addEdges(EdgeList edges);
  void addEdgesToFront(EdgeList front);
  void addMesh(FaceList mesh);

  bool belongsToAdvFront(Edge *e);

  virtual enum MethodStatus makeGeometryBasedMesh();
  virtual enum MethodStatus makeTopologyBasedMesh();
  enum MethodStatus improveMesh();

  bool execute(const FaceList &oldmesh);

  string getText();

  // #if USE_OPENGL
  //     void highlight();
  //     void unhighlight();

  //    void draw();
  //    void drawNormals();
  // #endif //#if USE_OPENGL
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // ADVANCING_FRONT_H_

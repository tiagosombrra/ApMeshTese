#ifndef ADVANCING_FRONT_H_
#define ADVANCING_FRONT_H_

#include <memory>

#include "../../definitions.h"
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

  std::shared_ptr<Boundary> boundary;
  std::shared_ptr<Quadtree> quadtree;

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
  bool interceptionTest(std::shared_ptr<Edge> e,
                        std::shared_ptr<Vertex> candidate,
                        bool inFaceTest = true, bool onlyFrontEdges = false);
  bool interceptionTest(std::shared_ptr<Edge> e);

  std::shared_ptr<Vertex> makeIdealVertex(std::shared_ptr<Edge> e, double &h);
  virtual bool findBestVertex(std::shared_ptr<Edge> e,
                              std::shared_ptr<Vertex> &best,
                              bool geometryPhase);

  void insertInFront(std::shared_ptr<Edge> last, std::shared_ptr<Edge> e);

  std::shared_ptr<Edge> findEdge(std::shared_ptr<Vertex> v1,
                                 std::shared_ptr<Vertex> v2);
  EdgeList findAdjacentEdges(std::shared_ptr<Vertex> v);
  FaceList findAdjacentFaces(const FaceList &faces, std::shared_ptr<Vertex> v);

  void removeFromFront(std::shared_ptr<Vertex> v1,
                       std::shared_ptr<Vertex> v2 = nullptr,
                       std::shared_ptr<Vertex> v3 = nullptr);

  virtual enum MethodStatus makeMesh(bool frontBased,
                                     bool geometryPhase = true);

  void fillMesh();

  bool laplacianSmoothing(bool &changed);

 public:
  AdvancingFront(double factor = 0.85, double tolerance = 1.e-8,
                 unsigned int numImproves = 5, double phi = 0.5);
  AdvancingFront(std::shared_ptr<Boundary> boundary,
                 std::shared_ptr<Quadtree> quadtree, double tolerance = 1.e-8,
                 unsigned int numImproves = 5);

  virtual ~AdvancingFront();

  void setBoundarySorted(bool boundarySorted);
  bool isBoundarySorted();

  void setNumImproves(unsigned int numImproves);
  unsigned int getNumImproves();

  void setBoundary(std::shared_ptr<Boundary> boundary);
  std::shared_ptr<Boundary> getBoundary();

  void setQuadtree(std::shared_ptr<Quadtree> quadtree);
  std::shared_ptr<Quadtree> getQuadtree();

  VertexList getVertices();
  VertexList getInnerVertices();

  EdgeList getEdges();
  EdgeList getInnerEdges();

  FaceList getMesh();

  void addVertices(VertexList vertices);
  void addEdges(EdgeList edges);
  void addEdgesToFront(EdgeList front);
  void addMesh(FaceList mesh);

  bool belongsToAdvFront(std::shared_ptr<Edge> e);

  virtual enum MethodStatus makeGeometryBasedMesh();
  virtual enum MethodStatus makeTopologyBasedMesh();
  enum MethodStatus improveMesh();

  bool execute(FaceList &oldmesh);

  string getText() override;
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // ADVANCING_FRONT_H_

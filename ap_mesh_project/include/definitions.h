#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <cmath>
#include <memory>
#include <string>

// Global defines

#ifndef TRUE
#define TRUE 1
#endif  // #ifndef TRUE

#ifndef FALSE
#define FALSE 0
#endif  // #ifndef FALSE

#ifndef TRACING_ENABLED
#define TRACING_ENABLED TRUE
#endif  // #ifndef TRUE

#ifndef USE_MPI
#define USE_MPI FALSE
#define RANK_ROOT 0
#endif  // #ifndef USE_MPI

#ifndef USE_OPENMP
#define USE_OPENMP FALSE
#define THREAD_ROOT 0
#endif  // #ifndef USE_OPENMP

#ifndef USE_PRINT_RESULTS
#define USE_PRINT_RESULTS FALSE
#endif  // #ifndef USE_PRINT_RESULTS

#ifndef USE_PRINT_COMENT
#define USE_PRINT_COMENT FALSE
#endif  // #ifndef USE_PRINT_COMENT

#ifndef USE_PRINT_ESTIMATE
#define USE_PRINT_ESTIMATE FALSE
#endif  // #ifndef USE_PRINT_ESTIMATE

#ifndef USE_PRINT_ERRO
#define USE_PRINT_ERRO TRUE
#endif  // #ifndef USE_PRINT_ERRO

#ifndef USE_PRINT_TIME
#define USE_PRINT_TIME TRUE
#endif  // #ifndef USE_PRINT_TIME

#ifndef USE_WRITE_MESH
#define USE_WRITE_MESH FALSE
#endif  // #ifndef USE_WRITE_MESH

#ifndef USE_SAVE_MESH
#define USE_SAVE_MESH FALSE
#endif  // #ifndef USE_SAVE_MESH

#ifndef USE_SAVE_ERRO_MESH
#define USE_SAVE_ERRO_MESH FALSE
#endif  // #ifndef USE_SAVE_ERRO_MESH

#ifndef USE_TIME_MEASUREMENTS
#define USE_TIME_MEASUREMENTS FALSE
#endif  // #ifndef USE_TIME_MEASUREMENTS

#ifndef USE_C__11
#define USE_C__11 TRUE
#endif  // #ifndef USE_C__11

#ifndef USE_NUMERICAL
#define USE_NUMERICAL FALSE
#endif  // #ifndef USE_NUMERICAL

#ifndef USE_TECGRAF
#define USE_TECGRAF FALSE
#endif  // #ifndef USE_TECGRAF

#ifndef USE_TECGRAF_MSH
#define USE_TECGRAF_MSH ((USE_TECGRAF) && (TRUE))
#endif  // #ifndef USE_TECGRAF_MSH

#ifndef USE_TECGRAF_MSH_2D
#define USE_TECGRAF_MSH_2D ((USE_TECGRAF_MSH) && (TRUE))
#endif  // #ifndef USE_TECGRAF_MSH_2D

#ifndef USE_TECGRAF_MSH_3D
#define USE_TECGRAF_MSH_3D ((USE_TECGRAF_MSH) && (TRUE))
#endif  // #ifndef USE_TECGRAF_MSH_3D

#ifndef USE_TECGRAF_MSH_SURF
#define USE_TECGRAF_MSH_SURF ((USE_TECGRAF_MSH) && (TRUE))
#endif  // #ifndef USE_TECGRAF_MSH_2SURF

#ifndef USE_WORKER_FILE_LOADER
#define USE_WORKER_FILE_LOADER TRUE
#endif  // #ifndef USE_WORKER_FILE_LOADER

#ifndef USE_OPENCV
#define USE_OPENCV TRUE
#endif  // #ifndef USE_OPENCV

#ifndef USE_EXPRTK
#define USE_EXPRTK ((USE_NUMERICAL) && (FALSE))
#endif  // #ifndef USE_EXPRTK

/// End global defines

#if USE_MPI
#include <mpi.h>
#define PROCESS_ROOT 0
#define TAG_SIZE_OF_DOUBLE 0
#define TAG_DOUBLE 1
#define TAG_PASSO 2
#define USE_SERIAL false
#define TAG_TIME 3
#endif  // #if USE_MPI

#include <sys/time.h>

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <set>
#include <sstream>
#include <tuple>

// Verificação de sistema operacional
#ifdef __linux__
#include "../libs/Eigen/Eigen/Dense"
#include "../libs/Eigen/Eigen/Eigen"
#elif __APPLE__
#include <Eigen/Eigen>
#else
#error "Unknown compiler"
#endif

#if __cplusplus

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <typeinfo>
#include <vector>

#if USE_C__11
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#endif  // #if USE_C__11

#include <cfloat>
#include <climits>
#include <cmath>

extern "C" {
#else
#define true TRUE
#define false FALSE
#endif  // #if __cplusplus

#define USE_LONG_LONG_INT TRUE
#define USE_DOUBLE TRUE

#define USE_LIST (FALSE)
#define USE_SET ((!USE_LIST) && (TRUE))

#define USE_ARRAY TRUE
#define USE_EXPERIMENTAL_5 ((USE_ARRAY) && (TRUE))
#define USE_EXPERIMENTAL_4 ((USE_ARRAY) && (TRUE))
#define USE_EXPERIMENTAL_3 ((USE_ARRAY) && (TRUE))
#define USE_EXPERIMENTAL_2 ((USE_ARRAY) && (TRUE))
#define USE_EXPERIMENTAL ((USE_ARRAY) && (TRUE))
#define USE_EXPERIMENTAL_MESH TRUE
#define USE_EXPERIMENTAL_TREE ((USE_EXPERIMENTAL_5) && (TRUE))
#define USE_DELETE_FELS TRUE

// defining it false makes classes not to inherit from Data::Poolable<T>
#define USE_POOL TRUE
#define USE_TREE_CELL_POOL ((USE_POOL) && (FALSE))
#define DEBUG_POOL_LEAK FALSE
#define DEBUG_POOL_FREE FALSE
#define DEBUG_POOL_CHECK_CHUNKS FALSE

#define USE_EXACT TRUE
#define USE_ZERO_ELIM ((USE_EXACT) && (TRUE))
#define USE_FAST_SUM ((USE_EXACT) && (FALSE))
#define USE_LINEAR_SUM ((USE_EXACT) && (!USE_FAST_SUM) && (TRUE))

// #define USE_TIME_MEASUREMENTS TRUE
#define USE_TIMING TRUE

#define USE_BUFFER FALSE

#if __cplusplus
}

namespace Data {
// type definitions or redefinitions

// these basic types were redefined because, in some other implementation,
// they can change. also, they change depending on the architecture, for
// example, in a 32 bit linux, gcc considers int and long int with the same
// size (in bytes), whereas in a 64 bit linux, gcc considers long int and
// long long int with the same size (in bytes). thus, it is recommended
// to use int and long long int, when you want the LInt type to have
// two times the size of Int.

typedef signed char Short;
typedef unsigned char UShort;

typedef signed int Int;
typedef unsigned int UInt;

#if USE_LONG_LONG_INT
typedef signed long long int LInt;
typedef unsigned long long int ULInt;
#else   // #if USE_LONG_LONG_INT
typedef signed long int LInt;
typedef unsigned long int ULInt;
#endif  // #if USE_LONG_LONG_INT

#if USE_DOUBLE
typedef double Real;
typedef long double LReal;

#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN
#else  // #if USE_DOUBLE
typedef float Real;
typedef double LReal;

#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN
#endif  // #if USE_DOUBLE

// streams
class OStream;
class TableStream;

// base class for all Data classes
class Identifiable;
class State;

// #if USE_POOL
//  classes for Data::Poolable library
//   code based on book Modern C++ Design: Generic Programming and Design
//   Patterns Applied, by Andrei Alexandrescu, Addison-Wesley Pub Co, 2001
template <typename T>
class Chunk;
template <typename T>
class Pool;
// has openmp code, which should be only on namespace Parallel
template <typename T>
class Poolable;
// #endif //#if USE_POOL

// classes for Data::Math library

#if USE_EXACT
template <typename T>
class Exact;
template <typename T>
class ExactFraction;
#endif  // #if USE_EXACT

// classes for Data::Math::Geometry library
class Vector;
class Vector2D;
class Vector3D;

#if USE_EXACT
class EVector;
class EVector2D;
class EVector3D;
#endif  // #if USE_EXACT

class Point;
class Point2D;
class Point3D;

#if USE_EXACT
class EPoint;
class EPoint2D;
class EPoint3D;
#endif  // #if USE_EXACT

// base class for all Data::Math::Geometry classes, except Point and Vector
class GeometricShape;
class Edge;
class Edge2D;
class Edge3D;
class Polygon;
class Triangle;
class Triangle2D;
class Triangle3D;
class Quadrilateral;
class Polyhedron;
class Tetrahedron;
class Pyramid;
class Wedge;
class Brick;
class Box;

// classes for Data::Math::Topology library

// classes for Data::Math::Topology::List library
class Vertex;
class Element;

// classes for Data::Structures library

// classes for Data::Structures::Graph library
// base class for all graphs
class Graph;
// base class for all graph nodes
class GraphNode;

// classes for Data::Structures::Graph::Tree library
// base class for all trees
class Tree;
// base class for all tree nodes
class TreeNode;

// classes for Data::Structures::Graph::GeometricalGraph library
class GeometricSearchTree;
class GeometricSearchTreeNode;
// class RecursiveDecompositionTree;
// class RecursiveDecompositionTreeNode;

class RecursiveDecompositionTree;
class RecursiveDecompositionTreeCell;

// base class for trees bounded by a box
class BoxTree;
class BinTree;
class QuadTree;
class OctTree;
class BoxSearchTree;
#if USE_EXPERIMENTAL_5
class BoxSearchTree2D;
class BoxSearchTree3D;
#endif  // #if USE_EXPERIMENTAL_5
#if USE_EXPERIMENTAL_5
class VertexBoxSearchTree2D;
class VertexBoxSearchTree3D;
#else
class VertexBoxSearchTree;
#endif  // #if USE_EXPERIMENTAL_5
#if USE_EXPERIMENTAL_5
class FrontElementBoxSearchTree2D;
class FrontElementBoxSearchTree3D;
#else
class FrontElementBoxSearchTree;
#endif  // #if USE_EXPERIMENTAL_5
class BSPTree;
class AlternatingDigitalTree;
class VertexADTree;
class FrontElementADTree;

// base class for tree nodes bounded by a box
class BoxTreeCell;
class BinTreeCell;
class QuadTreeCell;
class OctTreeCell;
class BoxSearchTreeCell;
#if USE_EXPERIMENTAL_5
class BoxSearchTreeCell2D;
class BoxSearchTreeCell3D;
#endif  // #if USE_EXPERIMENTAL_5
#if USE_EXPERIMENTAL_5
class VertexBoxSearchTreeCell2D;
class VertexBoxSearchTreeCell3D;
#else
class VertexBoxSearchTreeCell;
#endif  // #if USE_EXPERIMENTAL_5
#if USE_EXPERIMENTAL_5
class FrontElementBoxSearchTreeCell2D;
class FrontElementBoxSearchTreeCell3D;
#else
class FrontElementBoxSearchTreeCell;
#endif  // #if USE_EXPERIMENTAL_5
class BSPTreeCell;
class AlternatingDigitalTreeCell;
class VertexADTreeCell;
class FrontElementADTreeCell;

// classes for Data::Structures::Mesh library
class FrontElement;
class AbstractMesh;
class Boundary;
#if USE_SET
class AbstractFront;
#endif  // #if USE_SET
class Front;
class Mesh;

// enumerations

enum Position { UNKNOWN = 0, IN = 1, ON, OUT };

// type definitions or redefinitions for STL data structures

typedef std::set<ULInt> ULIntSet;
typedef std::set<Real> RealSet;
typedef std::set<std::shared_ptr<Identifiable>> IdentifiableSet;
typedef std::set<std::shared_ptr<Point>> PointSet;
typedef std::set<std::shared_ptr<Vertex>> VertexSet;
typedef std::set<std::shared_ptr<FrontElement>> FrontElementSet;
typedef std::set<std::shared_ptr<Element>> ElementSet;
typedef std::set<std::shared_ptr<GraphNode>> GraphNodeSet;

#if USE_SET
typedef std::multiset<std::shared_ptr<FrontElement>, AbstractFront>
    FrontElementMultiSet;
#endif  // #if USE_SET

#if USE_C__11
typedef std::unordered_set<ULInt> ULIntHash;
typedef std::unordered_set<Real> RealHash;
typedef std::unordered_set<std::shared_ptr<Identifiable>> IdentifiableHash;
typedef std::unordered_set<std::shared_ptr<Point>> PointHash;
typedef std::unordered_set<std::shared_ptr<Vertex>> VertexHash;
typedef std::unordered_set<std::shared_ptr<FrontElement>> FrontElementHash;
typedef std::unordered_set<std::shared_ptr<Element>> ElementHash;
typedef std::unordered_set<std::shared_ptr<GraphNode>> GraphNodeHash;

#if USE_SET
typedef std::unordered_multiset<std::shared_ptr<FrontElement>, AbstractFront>
    FrontElementMultiHash;
#endif  // #if USE_SET
#endif  // #if USE_C__11

typedef std::list<Int> IntList;
typedef std::list<UInt> UIntList;
typedef std::list<ULInt> ULIntList;
typedef std::list<std::shared_ptr<Identifiable>> IdentifiableList;
#if USE_EXPERIMENTAL_3
typedef std::list<Point2D> Point2DObjList;
typedef std::list<Point3D> Point3DObjList;
#else
typedef std::list<Point> PointObjList;
#endif  // #if USE_EXPERIMENTAL_3
typedef std::list<std::shared_ptr<Point>> PointList;
typedef std::list<std::shared_ptr<Edge>> EdgeList;
typedef std::list<std::shared_ptr<Box>> BoxList;
typedef std::list<std::shared_ptr<Polygon>> PolygonList;
typedef std::list<std::shared_ptr<Vertex>> VertexList;
typedef std::list<std::shared_ptr<FrontElement>> FrontElementList;
typedef std::list<std::shared_ptr<Element>> ElementList;
typedef std::list<std::shared_ptr<Boundary>> BoundaryList;
typedef std::list<std::shared_ptr<Front>> FrontList;
typedef std::list<std::shared_ptr<GraphNode>> GraphNodeList;
typedef std::list<std::shared_ptr<Mesh>> MeshList;

typedef std::vector<bool> BoolVector;
typedef std::vector<Real> RealVector;
typedef std::vector<Int> IntVector;
typedef std::vector<UInt> UIntVector;
typedef std::vector<ULInt> ULIntVector;
#if USE_EXPERIMENTAL_3
typedef std::vector<Point2D> Point2DObjVector;
typedef std::vector<Point3D> Point3DObjVector;
#else
typedef std::vector<Point> PointObjVector;
#endif  // #if USE_EXPERIMENTAL_3
typedef std::vector<std::shared_ptr<Point>> PointVector;
typedef std::vector<PointVector> PointMatrix;
typedef std::vector<std::shared_ptr<Vector>> VectorVector;
typedef std::vector<std::shared_ptr<Polygon>> PolygonVector;
typedef std::vector<std::shared_ptr<Vertex>> VertexVector;
typedef std::vector<std::shared_ptr<FrontElement>> FrontElementVector;
typedef std::vector<std::shared_ptr<Element>> ElementVector;
typedef std::vector<std::shared_ptr<GraphNode>> GraphNodeVector;

typedef std::map<ULInt, UShort> UShortMap;
typedef std::map<ULInt, Int> IntMap;
typedef std::map<ULInt, UInt> UIntMap;
typedef std::map<ULInt, ULInt> ULIntMap;
typedef std::map<ULInt, Real> RealMap;
typedef std::map<ULInt, std::shared_ptr<Point>> PointMap;
typedef std::map<ULInt, std::shared_ptr<Vertex>> VertexMap;
typedef std::map<ULInt, std::shared_ptr<FrontElement>> FrontElementMap;
typedef std::map<ULInt, std::shared_ptr<Element>> ElementMap;
typedef std::map<ULInt, enum Data::Position> PositionMap;
typedef std::map<ULInt, std::shared_ptr<GraphNode>> GraphNodeMap;

#if USE_C__11
typedef std::unordered_map<ULInt, UShort> UShortHashMap;
typedef std::unordered_map<ULInt, Int> IntHashMap;
typedef std::unordered_map<ULInt, UInt> UIntHashMap;
typedef std::unordered_map<ULInt, ULInt> ULIntHashMap;
typedef std::unordered_map<ULInt, Real> RealHashMap;
typedef std::unordered_map<ULInt, std::shared_ptr<Point>> PointHashMap;
typedef std::unordered_map<ULInt, std::shared_ptr<Vertex>> VertexHashMap;
typedef std::unordered_map<ULInt, std::shared_ptr<FrontElement>>
    FrontElementHashMap;
typedef std::unordered_map<ULInt, std::shared_ptr<Element>> ElementHashMap;
typedef std::unordered_map<ULInt, enum Data::Position> PositionHashMap;
typedef std::unordered_map<ULInt, std::shared_ptr<GraphNode>> GraphNodeHashMap;
#endif  // #if USE_C__11

#if USE_C__11
typedef std::tuple<Real, Real> R2Tuple;
typedef std::tuple<Real, Real, Real> R3Tuple;
typedef std::tuple<Real, Real, Real, Real> R4Tuple;
typedef std::tuple<UInt, UInt> UI2Tuple;
typedef std::tuple<ULInt, ULInt> ULI2Tuple;
typedef std::tuple<ULInt, ULInt, ULInt, ULInt> ULI4Tuple;
typedef std::tuple<std::shared_ptr<Vertex>, std::shared_ptr<FrontElementList>,
                   std::shared_ptr<Element>>
    ElementTuple;
#else
struct R2Tuple {
  Real values[2];
};
struct R3Tuple {
  Real values[3];
};
struct R4Tuple {
  Real values[4];
};
struct UI2Tuple {
  UInt values[2];
};
struct ULI2Tuple {
  ULInt values[2];
};
struct ULI4Tuple {
  ULInt values[4];
};
struct ElementTuple {
  Vertex *v;
  FrontElementList *fes;
  Element *e;
};
#endif  // #if USE_C__11

// functions
// initialize global stuff
void init(int *argc, char **argv);
void finalize();
// returns a backtrace of called functions
std::string trace(int maxLines = -1);

// streams and strings
// OStream &cout();
OStream &cerr();
OStream &clog();
OStream &endl(OStream &s);

template <typename Type>
#if USE_C__11
Type convert(const std::string &s) noexcept(false);
#else
Type convert(const std::string &s) throw(std::ios_base::failure);
#endif  // #if USE_C__11
template <typename Type>
std::string convert(Type t);

#if USE_TIMING
Real time();
#endif  // #if USE_TIMING
std::string time(Real seconds);
std::string time(UInt seconds);
std::string time(Real hours, Real mins, Real secs);
std::string time(UInt hours, UInt mins, UInt secs);

#if defined(__linux__)
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);
#endif  // #if defined(__linux__)

std::string position(enum Position pos);

void setState(const State &state);
State &getState();

void setTolerance(Real tolerance);
Real getTolerance();

#if USE_BUFFER
void setBuffer(const std::string &buffer);
const std::string &getBuffer();
#endif  // #if USE_BUFFER
}  // namespace Data

template <typename Type>
#if USE_C__11
Type Data::convert(const std::string &s) noexcept(false)
#else
Type Data::convert(const std::string &s) throw(std::ios_base::failure)
#endif  // #if USE_C__11
{
  Type value = static_cast<Type>(0);

  std::stringstream str;
  str << s;

  if (str.eof()) {
    std::stringstream err;

    err << "Impossible to convert from string '" << s << "' to value of type "
        << typeid(Type).name();

    throw(std::ios_base::failure(err.str()));
  }

  str >> value;

  if ((str.fail()) || (str.bad())) {
    std::stringstream err;

    err << "Impossible to convert from string '" << s << "' to value of type "
        << typeid(Type).name();

    throw(std::ios_base::failure(err.str()));
  }

  return value;
}

template <typename Type>
std::string Data::convert(Type value) {
  std::stringstream str;

  str << value;

  return str.str();
}

#endif  // #if __cplusplus

using namespace std;
using namespace Eigen;

typedef Matrix<double, 4, 4> Matrix4x4;
typedef Matrix<double, 4, 1> Matrix4x1;
typedef Matrix<double, 1, 4> Matrix1x4;
typedef Matrix<double, 1, 1> Matrix1x1;

namespace Par2DJMesh {
namespace Basics {
class Shape;

class Vertex;

class Edge;

class Face;
}  // namespace Basics

namespace AFT {
class Boundary;

class Quadtree;

class QuadtreeCell;

class AdvancingFront;
}  // namespace AFT

// Definicoes de listas e conjuntos
namespace Basics {
typedef list<std::shared_ptr<Vertex>> VertexList;
typedef list<std::shared_ptr<Edge>> EdgeList;
typedef list<std::shared_ptr<Face>> FaceList;

typedef set<std::shared_ptr<Vertex>> VertexSet;
typedef set<std::shared_ptr<Edge>> EdgeSet;
typedef set<std::shared_ptr<Face>> FaceSet;
}  // namespace Basics

namespace AFT {
typedef list<std::shared_ptr<Boundary>> BoundaryList;
typedef list<QuadtreeCell *> QuadtreeCellList;
}  // namespace AFT

namespace AFT {
enum MethodStatus  // current status of the method
{
  // initialization related
  INIT_INITIALIZED = 0,

  // quadtree related
  QUAD_INITIAL_TREE_DONE,
  QUAD_REFINE_TO_LEVEL_DONE,
  QUAD_REFINE_ACCORDING_TO_NEIGHBORS_DONE,
  QUAD_MAKE_TEMPLATE_BASED_MESH_DONE,

  // advancing front related - unique process
  ADVF_GEOMETRY_MESH_DONE,
  ADVF_TOPOLOGY_MESH_DONE,

  // improvement related
  IMPR_IMPROVEMENT_DONE,

  // debug
  ADVF_GEOMETRY_EDGE_REJECTED_TWICE,
  // advancing front related - non unique process
  ADVF_MESH_DONE,
  // endebug

  // debug related
  QUAD_DEBUG,
  ADVF_DEBUG,

  // method exceptions related
  INIT_EMPTY,
  INIT_NOTHING_DONE,
  ADVF_EMPTY,

  ADVF_TOPOLOGY_EDGE_REJECTED_TWICE,

  // errors related
  ADVF_OUT_QUADTREE,
  ADVF_EDGE_ZERO_LENGTH,
  IMPR_LOCAL_BACK_TRACKING_FAILED,
  AFT_ERROR
};

static string methodNotices[] = {
    // initialization related
    "Initialized",

    // quadtree related
    "Initial tree done", "Tree refined to minimum level",
    "Tree refined according to neighbors", "Template-based mesh done",

    // advancing front related
    "Geometry mesh done", "Topology mesh done",

    // improvement related
    "Improvement done",

    // advancing front related - non unique process
    "Same edge rejected twice on geometry phase",
    "Mesh done by all the processes",

    // debug related
    "Debug mode - Quadtree", "Debug mode - Advancing Front",

    // method exceptions related
    "Load a polygon", "Already done", "Load a polygon",

    "Same edge rejected twice on topology phase",

    // errors related
    "Vertex out of quadtree created", "Edge of zero length found",
    "Local back-tracking failed", "Error"};

enum  // lados de busca dos vizinhos da celula da quadtree
{
  QUAD_BOTTOM = 0,
  QUAD_B = QUAD_BOTTOM,
  QUAD_DOWN = QUAD_BOTTOM,
  QUAD_D = QUAD_BOTTOM,

  QUAD_RIGHT,
  QUAD_R = QUAD_RIGHT,

  QUAD_TOP,
  QUAD_T = QUAD_TOP,
  QUAD_UP = QUAD_TOP,
  QUAD_U = QUAD_TOP,

  QUAD_LEFT,
  QUAD_L = QUAD_LEFT
};

enum  // posicoes dos filhos de uma celula
{
  QUAD_BOTTOM_LEFT = 0,
  QUAD_LEFT_BOTTOM = QUAD_BOTTOM_LEFT,
  QUAD_BL = QUAD_BOTTOM_LEFT,
  QUAD_LB = QUAD_BOTTOM_LEFT,

  QUAD_BOTTOM_RIGHT,
  QUAD_RIGHT_BOTTOM = QUAD_BOTTOM_RIGHT,
  QUAD_BR = QUAD_BOTTOM_RIGHT,
  QUAD_RB = QUAD_BOTTOM_RIGHT,

  QUAD_TOP_RIGHT,
  QUAD_RIGHT_TOP = QUAD_TOP_RIGHT,
  QUAD_TR = QUAD_TOP_RIGHT,
  QUAD_RT = QUAD_TOP_RIGHT,

  QUAD_TOP_LEFT,
  QUAD_LEFT_TOP = QUAD_TOP_LEFT,
  QUAD_TL = QUAD_TOP_LEFT,
  QUAD_LT = QUAD_TOP_LEFT
};

enum  // posicoes dos vertices do meio da quadrtree
{
  QUAD_MID = QUAD_LEFT + 1,
  QUAD_M = QUAD_MID
};
}  // namespace AFT
}  // namespace Par2DJMesh

#endif  // DEFINITIONS_H_

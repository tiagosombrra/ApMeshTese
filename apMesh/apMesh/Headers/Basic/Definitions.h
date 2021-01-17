#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_


#define USE_PRINT_RESULT       false
#define USE_PRINT_COMENT       false
#define USE_PRINT_ERRO         true
#define USE_PRINT_TIME         true
#define USE_OPENMP             false
#define USE_OPENMPI            false
#define USE_SAVE_MESH          true

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <set>
#include <tuple>
#include <limits>

#include <cfloat>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>

//Verificação de sistema operacional
#ifdef __APPLE__
#include <Eigen/Eigen>
#elif __linux__
#include "../../Libs/Eigen/Eigen/Eigen"
#else
#   error "Unknown compiler"
#endif

using namespace std;
using namespace Eigen;

typedef Matrix<double, 4, 4> Matrix4x4;
typedef Matrix<double, 4, 1> Matrix4x1;
typedef Matrix<double, 1, 4> Matrix1x4;
typedef Matrix<double, 1, 1> Matrix1x1;

namespace Par2DJMesh
{
namespace Basics
{
class Shape;

class Vertex;

class Edge;

class Face;
}

namespace AFT
{
class Boundary;

class Quadtree;

class QuadtreeCell;

class AdvancingFront;
}


//Definicoes de listas e conjuntos
namespace Basics
{
typedef list<Vertex *> VertexList;
typedef list<Edge *> EdgeList;
typedef list<Face *> FaceList;

typedef set<Vertex *> VertexSet;
typedef set<Edge *> EdgeSet;
typedef set<Face *> FaceSet;
}

namespace AFT
{
typedef list<Boundary *> BoundaryList;
typedef list<QuadtreeCell *> QuadtreeCellList;
}

namespace AFT
{
enum MethodStatus //current status of the method
{
    //initialization related
    INIT_INITIALIZED = 0,

    //quadtree related
    QUAD_INITIAL_TREE_DONE,
    QUAD_REFINE_TO_LEVEL_DONE,
    QUAD_REFINE_ACCORDING_TO_NEIGHBORS_DONE,
    QUAD_MAKE_TEMPLATE_BASED_MESH_DONE,

    //advancing front related - unique process
    ADVF_GEOMETRY_MESH_DONE,
    ADVF_TOPOLOGY_MESH_DONE,

    //improvement related
    IMPR_IMPROVEMENT_DONE,

    //debug
    ADVF_GEOMETRY_EDGE_REJECTED_TWICE,
    //advancing front related - non unique process
    ADVF_MESH_DONE,
    //endebug

    //debug related
    QUAD_DEBUG,
    ADVF_DEBUG,

    //method exceptions related
    INIT_EMPTY,
    INIT_NOTHING_DONE,
    ADVF_EMPTY,

    ADVF_TOPOLOGY_EDGE_REJECTED_TWICE,

    //errors related
    ADVF_OUT_QUADTREE,
    ADVF_EDGE_ZERO_LENGTH,
    IMPR_LOCAL_BACK_TRACKING_FAILED,
    AFT_ERROR
};

static string methodNotices[] =
{
    //initialization related
    "Initialized",

    //quadtree related
    "Initial tree done",
    "Tree refined to minimum level",
    "Tree refined according to neighbors",
    "Template-based mesh done",

    //advancing front related
    "Geometry mesh done",
    "Topology mesh done",

    //improvement related
    "Improvement done",

    //advancing front related - non unique process
    "Same edge rejected twice on geometry phase",
    "Mesh done by all the processes",

    //debug related
    "Debug mode - Quadtree",
    "Debug mode - Advancing Front",

    //method exceptions related
    "Load a polygon",
    "Already done",
    "Load a polygon",

    "Same edge rejected twice on topology phase",

    //errors related
    "Vertex out of quadtree created",
    "Edge of zero length found",
    "Local back-tracking failed",
    "Error"
};

enum //lados de busca dos vizinhos da celula da quadtree
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

enum //posicoes dos filhos de uma celula
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

enum //posicoes dos vertices do meio da quadrtree
{
    QUAD_MID = QUAD_LEFT + 1,
    QUAD_M = QUAD_MID
};
}
}

#endif //#ifndef _DEFINITIONS_H_

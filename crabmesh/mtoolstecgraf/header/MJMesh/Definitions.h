#ifndef _MJMESH_DEFINITIONS_H_
#define _MJMESH_DEFINITIONS_H_

#include "Performer/Definitions.h"

#define USE_ATTRIBS TRUE
#define USE_RECALC_ATTRIBS (USE_ATTRIBS && FALSE)

#define USE_ID_HASH_IN_MESH_SPLIT ((USE_C__11) && (TRUE))

//para comparacoes com a implementacao do prof. Bento, use
//  USE_JMSH_IMPLEMENTATION_DETAILS TRUE
//  USE_JMSH_BACK_TRACKING TRUE
//  USE_JMSH_SMOOTHING FALSE

//para versao final, use
//  USE_JMSH_IMPLEMENTATION_DETAILS FALSE
//  USE_JMSH_BACK_TRACKING TRUE
//  USE_JMSH_SMOOTHING TRUE

//para desativar os incrementos no back tracking, use
//  USE_JMSH_BACK_TRACKING FALSE

#define USE_JMSH_IMPLEMENTATION_DETAILS FALSE
#define USE_JMSH_BACK_TRACKING TRUE
#define USE_JMSH_SMOOTHING TRUE
#define USE_JMSH_DEBUG_MESSAGES FALSE

#define USE_QUADS TRUE
#define USE_BRICKS TRUE

#define USE_ADT FALSE

#define DEBUG_TIMING_PMJMESH ((USE_TIME_MEASUREMENTS) && (FALSE))
#define DEBUG_TIMING_PMTOOLS ((USE_TIME_MEASUREMENTS) && (FALSE))

#if USE_ID_HASH_IN_MESH_SPLIT
#include "Data/Identifiable.h"
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

namespace MJMesh
{
	using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	// MJMesh::Data
	class Vertex;
	class Edge2D;
	class Edge3D;
	class Triangle3D;
	class Element;
	class Element2D;
	class Element3D;
	class ElementSurface;
	class FrontElement;
	class FrontElement2D;
	class FrontElement3D;
	class FrontElementSurface;

#if USE_EXPERIMENTAL_5
	class VertexBoxSearchTree2D;
	class VertexBoxSearchTreeCell2D;
	class VertexBoxSearchTree3D;
	class VertexBoxSearchTreeCell3D;
#else
	class VertexBoxSearchTree;
	class VertexBoxSearchTreeCell;
#endif //#if USE_EXPERIMENTAL_5

#if USE_EXPERIMENTAL_5
	class FrontElementBoxSearchTree2D;
	class FrontElementBoxSearchTreeCell2D;
	class FrontElementBoxSearchTree3D;
	class FrontElementBoxSearchTreeCell3D;
#else
	class FrontElementBoxSearchTree;
	class FrontElementBoxSearchTreeCell;
#endif //#if USE_EXPERIMENTAL_5

	class QuadTree;
	class OctTree;
	class QuadTreeCell;
	class OctTreeCell;
	class Boundary;
	class Front;
	class Mesh;

	// MJMesh::Performer
	class AnglePointInObjectDecider;
	class BoundaryBuilder;
	class HotellingTransformer;
	class TreeGenerator;
	class MinTreeRefiner;
	class NeighborTreeRefiner;
	class VertexSearchTreeGenerator;
	class FrontElementSearchTreeGenerator;
	class FrontAdvancer;
	class GeometryFrontAdvancer;
	class TopologyFrontAdvancer;
	class BackTrackingFrontAdvancer;
	class LaplacianSmoother;
	class MeshBackTracker;
	class MeshImprover;

	class MainDrive;

	struct SpecificData;
	struct VertexSpecificData;
	struct FrontElementSpecificData;
	struct ElementSpecificData;

#if USE_ID_HASH_IN_MESH_SPLIT
	typedef std::unordered_set<Data::Vertex *, Data::Identifiable::Hash, Data::Identifiable::Equal> VertexIdHash;
	typedef std::unordered_set<Data::FrontElement *, Data::Identifiable::Hash, Data::Identifiable::Equal> FrontElementIdHash;
	typedef std::unordered_set<Data::Element *, Data::Identifiable::Hash, Data::Identifiable::Equal> ElementIdHash;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

#if USE_C__11
	typedef std::unordered_map<Data::Element *, Data::FrontElementList> ElFElsHashMap;
	//typedef std::unordered_map<UInt, Data::ElementList> IntElsHashMap;
	typedef std::unordered_multimap<UInt, Data::Element *> IntElsHashMap;
	//typedef std::unordered_map<UInt, Data::FrontElementList> IntFElsHashMap;
	typedef std::unordered_multimap<UInt, Data::FrontElement *> IntFElsHashMap;
#endif //#if USE_C__11

	typedef std::map<Data::Element *, Data::FrontElementList> ElFElsMap;
	//typedef std::map<UInt, Data::ElementList> IntElsMap;
	typedef std::multimap<UInt, Data::Element *> IntElsMap;
	//typedef std::map<UInt, Data::FrontElementList> IntFElsMap;
	typedef std::multimap<UInt, Data::FrontElement *> IntFElsMap;

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
}

#endif //#ifndef _MJMESH_DEFINITIONS_H_

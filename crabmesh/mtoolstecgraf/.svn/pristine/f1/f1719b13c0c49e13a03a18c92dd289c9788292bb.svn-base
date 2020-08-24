#ifndef _MJMESH_SPECIFIC_DATA_H_
#define _MJMESH_SPECIFIC_DATA_H_

#include "MJMesh/Definitions.h"

#include "Data/GeometricSearchTreeNode.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"

namespace MJMesh
{
	struct SpecificData
	{
		//typedef std::list<const Boundary *> BoundaryList;

		struct SearchTreeNodeSpecificData
		{
			const Data::GeometricSearchTreeNode *stnode;
			Data::GeometricSearchTreeNode::ShapeSet::iterator stnodeIt;
		};

		typedef std::list<SearchTreeNodeSpecificData> SearchTreeDataList;

		//BoundaryList boundaries;
	};

	struct VertexSpecificData : public SpecificData
	{
#if USE_C__11
		typedef std::unordered_map<const Boundary *, Data::Boundary::VertexIterator> BoundaryDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Boundary *, Data::Boundary::VertexIterator> BoundaryDataMap;

		BoundaryDataMap boundaries;

		struct FrontSpecificData
		{
			Data::Front::VertexIterator frontIt;

			SearchTreeDataList searchNodes;
		};

#if USE_C__11
		typedef std::unordered_map<const Front *, FrontSpecificData> FrontDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Front *, FrontSpecificData> FrontDataMap;

		struct MeshSpecificData
		{
			bool inner;

			Data::Mesh::VertexIterator meshIt;
#if (!USE_EXPERIMENTAL_MESH)
			Data::Mesh::VertexIterator innerMeshIt;
#endif //#if (!USE_EXPERIMENTAL_MESH)
		};

#if USE_C__11
		typedef std::unordered_map<const Mesh *, MeshSpecificData> MeshDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Mesh *, MeshSpecificData> MeshDataMap;

		FrontDataMap fronts;

		MeshDataMap meshes;
	};

	struct FrontElementSpecificData : public SpecificData
	{
#if USE_C__11
		typedef std::unordered_map<const Boundary *, Data::Boundary::FrontElementIterator> BoundaryDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Boundary *, Data::Boundary::FrontElementIterator> BoundaryDataMap;

		BoundaryDataMap boundaries;

		struct FrontSpecificData
		{
			UShort state; //almost as original Jmesh 'use' attribute
			Data::Front::FrontElementIterator frontIt;

			SearchTreeDataList searchNodes;
		};

#if USE_C__11
		typedef std::unordered_map<const Front *, FrontSpecificData> FrontDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Front *, FrontSpecificData> FrontDataMap;

#if USE_C__11
		typedef std::unordered_map<const Mesh *, Data::Mesh::FrontElementIterator> MeshDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Mesh *, Data::Mesh::FrontElementIterator> MeshDataMap;

		FrontDataMap fronts;

		MeshDataMap meshes;
	};

	struct ElementSpecificData
	{
#if USE_C__11
		typedef std::unordered_map<const Mesh *, Data::Mesh::ElementIterator> MeshDataHashMap;
#endif //#if USE_C__11

		typedef std::map<const Mesh *, Data::Mesh::ElementIterator> MeshDataMap;

		MeshDataMap meshes;
	};
}

#endif //#ifndef _MJMESH_SPECIFIC_DATA_H_

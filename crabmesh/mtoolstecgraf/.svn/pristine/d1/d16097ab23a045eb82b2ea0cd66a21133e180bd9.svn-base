#ifndef _MJMESH_VERTEX_SEARCH_TREE_GENERATOR_H_
#define _MJMESH_VERTEX_SEARCH_TREE_GENERATOR_H_

#include "MJMesh/Definitions.h"
#include "Performer/TreeGenerator.h"
#include "Data/VertexBoxSearchTree.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"

namespace MJMesh
{
	class VertexSearchTreeGenerator : public Performer::TreeGenerator
	{
	public:

		VertexSearchTreeGenerator(const MJMesh::Front *front,
			const Data::BoxTree *tree);
		virtual ~VertexSearchTreeGenerator();

#if USE_GUI
		using Performer::TreeGenerator::setTree;
#endif //#if USE_GUI

		virtual void setTree(const Data::BoxTree *tree);

		virtual void execute();

		virtual Data::GeometricSearchTree *makeTree() const;

	protected:

		const Data::BoxTree *tree;

		const MJMesh::Front *front;
	};
}

#endif //#ifndef _MJMESH_VERTEX_SEARCH_TREE_GENERATOR_H_

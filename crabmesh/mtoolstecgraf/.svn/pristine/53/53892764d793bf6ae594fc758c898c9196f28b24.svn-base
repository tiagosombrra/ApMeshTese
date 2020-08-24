#ifndef _PMJMESH_VERTEX_SEARCH_TREE_GENERATOR_H_
#define _PMJMESH_VERTEX_SEARCH_TREE_GENERATOR_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/VertexSearchTreeGenerator.h"
#include "PMJMesh/VertexBoxSearchTree.h"

namespace PMJMesh
{
	class VertexSearchTreeGenerator : public MJMesh::VertexSearchTreeGenerator
	{
	public:

		VertexSearchTreeGenerator(const MJMesh::Front *front,
			const Data::BoxTree *tree, const Data::Box *box);
		virtual ~VertexSearchTreeGenerator();

		virtual void setBox(const Data::Box *box);

		virtual Data::GeometricSearchTree *makeTree() const;

	protected:

		const Data::Box *box;
	};
}

#endif //#ifndef _PMJMESH_VERTEX_SEARCH_TREE_GENERATOR_H_

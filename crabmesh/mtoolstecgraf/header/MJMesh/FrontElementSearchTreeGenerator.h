#ifndef _MJMESH_FRONT_ELEMENT_SEARCH_TREE_GENERATOR_H_
#define _MJMESH_FRONT_ELEMENT_SEARCH_TREE_GENERATOR_H_

#include "MJMesh/Definitions.h"
#include "Performer/TreeGenerator.h"

#include "Data/FrontElementBoxSearchTree.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"

namespace MJMesh
{
	class FrontElementSearchTreeGenerator : public Performer::TreeGenerator
	{
	public:

		FrontElementSearchTreeGenerator(const MJMesh::Front *front, const Data::BoxTree *tree);
		FrontElementSearchTreeGenerator(const MJMesh::Boundary *boundary, const Data::BoxTree *tree);
		virtual ~FrontElementSearchTreeGenerator();

#if USE_GUI
		using Performer::TreeGenerator::setTree;
#endif //#if USE_GUI

		virtual void setTree(const Data::BoxTree *tree);

		virtual void execute();

		virtual Data::GeometricSearchTree *makeTree() const;

	protected:

		const Data::BoxTree *tree;

		const MJMesh::Front *front;
		const MJMesh::Boundary *boundary;
	};
}

#endif //#ifndef _MJMESH_FRONT_ELEMENT_SEARCH_TREE_GENERATOR_H_

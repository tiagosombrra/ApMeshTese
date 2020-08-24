#ifndef _PMJMESH_FRONT_ELEMENT_SEARCH_TREE_GENERATOR_H_
#define _PMJMESH_FRONT_ELEMENT_SEARCH_TREE_GENERATOR_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/FrontElementSearchTreeGenerator.h"
#include "PMJMesh/FrontElementBoxSearchTree.h"

namespace PMJMesh
{
	class FrontElementSearchTreeGenerator : public MJMesh::FrontElementSearchTreeGenerator
	{
	public:

		FrontElementSearchTreeGenerator(const MJMesh::Front *front,
			const Data::BoxTree *tree, const Data::Box *box);
		virtual ~FrontElementSearchTreeGenerator();

		virtual void setBox(const Data::Box *box);

		virtual Data::GeometricSearchTree *makeTree() const;

#if USE_ADT
		virtual void execute();
#endif //#if USE_ADT

	protected:

		const Data::Box *box;
	};
}

#endif //#ifndef _PMJMESH_FRONT_ELEMENT_SEARCH_TREE_GENERATOR_H_

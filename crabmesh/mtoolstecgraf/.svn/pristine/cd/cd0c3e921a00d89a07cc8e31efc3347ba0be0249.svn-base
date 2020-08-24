#ifndef _PJMESH_TREE_GENERATOR_H_
#define _PJMESH_TREE_GENERATOR_H_

#include "PJmesh/Definitions.h"

#include "Jmesh/TreeGenerator.h"
#include "PMJMesh/TreeGenerator.h"

namespace PJmesh
{
	class TreeGenerator : public Jmesh::TreeGenerator, public PMJMesh::TreeGenerator
	{
	public:

		TreeGenerator(MJMesh::Boundary *boundary = NULL, Msh3DOctTree root = NULL
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			, const Parallel::ThreadManager *tm = NULL
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			);
		virtual ~TreeGenerator();

		virtual void execute();

	protected:

		using PMJMesh::TreeGenerator::makeTree;
		virtual Data::Tree *makeTree(const Data::Point3D &min, const Data::Point3D &max) const;

		using Jmesh::TreeGenerator::subdivide;
		using PMJMesh::TreeGenerator::subdivide;
		virtual void subdivide(Data::OctTreeCell *cell) const;
	};
}

#endif //#ifndef _PJMESH_TREE_GENERATOR_H_

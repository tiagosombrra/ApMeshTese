#ifndef _PMTOOLS_TREE_GENERATOR_H_
#define _PMTOOLS_TREE_GENERATOR_H_

#include "PMTools/Definitions.h"

#if USE_MTOOLS

#include "MTools/TreeGenerator.h"
#include "PMJMesh/TreeGenerator.h"

namespace PMTools
{
	class TreeGenerator : public MTools::TreeGenerator, public PMJMesh::TreeGenerator
	{
	public:

		TreeGenerator(MJMesh::Boundary *boundary = NULL, const FTools::OctTree *ftree = NULL
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			, const Parallel::ThreadManager *tm = NULL
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			);
		virtual ~TreeGenerator();

		virtual void execute();

	protected:

		using PMJMesh::TreeGenerator::makeTree;
		virtual Data::Tree *makeTree(const Data::Point3D &min, const Data::Point3D &max) const;

		using MTools::TreeGenerator::subdivide;
		using PMJMesh::TreeGenerator::subdivide;
		virtual void subdivide(Data::OctTreeCell *cell) const;

#if DEBUG_TIMING_PMTOOLS
	public:

		mutable Real timeGeneration, timeClassification;
#endif //#if DEBUG_TIMING_PMTOOLS
	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_TREE_GENERATOR_H_

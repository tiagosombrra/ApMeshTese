#ifndef _PMJMESH_BOUNDARY_NUMBER_TREE_PARTITIONER_H_
#define _PMJMESH_BOUNDARY_NUMBER_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/MinNumberTreePartitioner.h"

namespace PMJMesh
{
	class BoundaryNumberTreePartitioner : public PMJMesh::MinNumberTreePartitioner
	{
	public:

		BoundaryNumberTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses,
			LoadCalculator *loadCalculator, bool useMean, UInt minNumPartitions
			/*, bool refinement21 = false*/);
		virtual ~BoundaryNumberTreePartitioner();

	protected:

		virtual bool shouldCount(const Data::BoxTreeCell *cell, bool increasing) const;

	};
}

#endif //#ifndef _PMJMESH_BOUNDARY_NUMBER_TREE_PARTITIONER_H_

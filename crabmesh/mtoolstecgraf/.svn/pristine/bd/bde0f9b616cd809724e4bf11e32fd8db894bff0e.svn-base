#ifndef _PMJMESH_BOUNDARY_MIN_NUMBER_BSP_TREE_PARTITIONER_H_
#define _PMJMESH_BOUNDARY_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"
#include "PMJMesh/MinNumberBSPTreePartitioner.h"

namespace PMJMesh
{
	class BoundaryMinNumberBSPTreePartitioner : public MinNumberBSPTreePartitioner
	{
	public:

		BoundaryMinNumberBSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
			bool usePartitioningThreshold, UInt minNumPartitions);
		virtual ~BoundaryMinNumberBSPTreePartitioner();

	protected:

		virtual bool shouldCount(const Data::BoxTreeCell *cell, bool increasing) const;

		virtual bool hasOnCell(const Data::Box *box) const;
		virtual bool hasOnCell(const Data::Box *box, const Data::BoxTreeCell *cell, bool test = true) const;

	};
}

#endif //#ifndef _PMJMESH_BOUNDARY_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

#ifndef _PMJMESH_BOUNDARY_SMART_MIN_NUMBER_BSP_TREE_PARTITIONER_H_
#define _PMJMESH_BOUNDARY_SMART_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"
#include "PMJMesh/SmartMinNumberBSPTreePartitioner.h"

namespace PMJMesh
{
	class BoundarySmartMinNumberBSPTreePartitioner : public SmartMinNumberBSPTreePartitioner
	{
	public:

		BoundarySmartMinNumberBSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
			bool usePartitioningThreshold, UInt minNumPartitions,
			UInt removeLayersInLoad = 0, bool proportional = true);
		virtual ~BoundarySmartMinNumberBSPTreePartitioner();

	protected:

		virtual bool shouldCount(const Data::BoxTreeCell *cell, bool increasing) const;

	};
}

#endif //#ifndef _PMJMESH_BOUNDARY_SMART_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

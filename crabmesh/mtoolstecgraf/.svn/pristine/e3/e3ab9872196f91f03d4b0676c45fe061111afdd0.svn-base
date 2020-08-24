#ifndef _PMJMESH_MAX_NUMBER_TREE_PARTITIONER_H_
#define _PMJMESH_MAX_NUMBER_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/TreePartitioner.h"

namespace PMJMesh
{
	class MinNumberTreePartitioner : public PMJMesh::TreePartitioner
	{
	public:

		MinNumberTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses,
			LoadCalculator *loadCalculator, bool usePartitioningThreshold,
			UInt minNumPartitions/*, bool refinement21 = false*/);
		virtual ~MinNumberTreePartitioner();

		virtual void setUsePartitioningThreshold(bool usePartitioningThreshold);

		virtual void setMinNumPartitions(UInt minNumPartitions);

		//virtual void setRefinement21(bool refinement21);

	protected:

		virtual UInt findMinNumPartitions() const;

		virtual bool shouldCount(const Data::BoxTreeCell *cell, bool increasing) const;

		virtual void executePartitioning();

	protected:

		virtual bool shouldSkip(const Data::BoxTreeCell *partitionCell) const;

		virtual void buildTree();

	protected:

		bool usePartitioningThreshold;

		UInt minNumPartitions;

		//bool refinement21;
	};
}

#endif //#ifndef _PMJMESH_MAX_NUMBER_TREE_PARTITIONER_H_

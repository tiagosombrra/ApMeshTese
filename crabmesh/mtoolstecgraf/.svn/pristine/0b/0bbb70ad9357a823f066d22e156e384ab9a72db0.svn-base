#ifndef _PMJMESH_MIN_NUMBER_BSP_TREE_PARTITIONER_H_
#define _PMJMESH_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"
#include "PMJMesh/BSPTreePartitioner.h"

namespace PMJMesh
{
	class MinNumberBSPTreePartitioner : public PMJMesh::BSPTreePartitioner
	{
	public:

		MinNumberBSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
			bool usePartitioningThreshold, UInt minNumPartitions);
		virtual ~MinNumberBSPTreePartitioner();

		virtual void setUsePartitioningThreshold(bool usePartitioningThreshold);

		virtual void setMinNumPartitions(UInt minNumPartitions);

	protected:

		virtual UInt findMinNumPartitions() const;

		virtual bool shouldCount(const Data::BoxTreeCell *cell, bool increasing) const;

		virtual void executePartitioning();

	protected:

		virtual bool shouldSkip(const Data::Box *box, const Data::BoxTreeCell *cell) const;
		virtual bool shouldSkip(const Data::Box *box) const;
		virtual bool shouldSkip(const Data::BoxTreeCell *partitionCell) const;

		virtual void buildTree();

		virtual void adjustBox() const;
		virtual void findBox(Data::Point &min, Data::Point &max) const;
		virtual void findBox(const Data::BoxTreeCell *c, Data::Point &min, Data::Point &max) const;
		virtual bool setBox(Data::BSPTreeCell *c, const Data::Point &min, const Data::Point &max) const;

		virtual Data::BoxTreeCell *findCell(const Data::Box *box);

	protected:

		bool usePartitioningThreshold;

		UInt minNumPartitions;
	};
}

#endif //#ifndef _PMJMESH_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

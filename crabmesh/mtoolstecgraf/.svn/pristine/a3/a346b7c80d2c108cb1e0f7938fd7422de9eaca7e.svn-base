#ifndef _PMJMESH_TREE_PARTITIONER_H_
#define _PMJMESH_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Partitioner.h"

namespace PMJMesh
{
	class TreePartitioner : public Parallel::Partitioner
	{
	public:

		TreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator);
		virtual ~TreePartitioner();

		virtual void setDimension(UInt dimension);

#if USE_C__11
		virtual void setLoad(const Data::RealHashMap *load);
#else
		virtual void setLoad(const Data::RealMap *load);
#endif //#if USE_C__11

		virtual void setTree(const Data::BoxTree *tree);

		virtual void setNumProcesses(UInt numProcesses);

		virtual void setLoadCalculator(LoadCalculator *loadCalculator);

		virtual void setSkippingInner(bool skippingInner);

		virtual Data::BoxTree *getPartitionTree() const;

		virtual Real getMaxCalculatedLoad() const;

		virtual const Data::ULIntVector &getTreeNewCells() const;

		virtual void execute();

	protected:

		virtual bool isSkipped(Data::BoxTreeCell *partitionCell) const;
		virtual void skip(Data::BoxTreeCell *partitionCell);
		virtual bool shouldSkip(const Data::BoxTreeCell *partitionCell) const = 0;

		virtual void executePartitioning() = 0;

	protected:

		UInt dimension;

		UInt numProcesses;

#if USE_C__11
		const Data::RealHashMap *load;
#else
		const Data::RealMap *load;
#endif //#if USE_C__11

		const Data::BoxTree *tree;
		Data::BoxTree *partitionTree;

		Data::ULIntVector treeNewCells;

		Real maxCalculatedLoad;

		LoadCalculator *loadCalculator;

		bool skippingInner;
		Data::GraphNodeSet skipped;
	};
}

#endif //#ifnder _PMJMESH_TREE_PARTITIONER_H_

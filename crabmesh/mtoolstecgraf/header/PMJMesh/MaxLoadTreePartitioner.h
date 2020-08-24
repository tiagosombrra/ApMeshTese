#ifndef _PMJMESH_MAX_LOAD_TREE_PARTITIONER_H_
#define _PMJMESH_MAX_LOAD_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/TreePartitioner.h"

namespace PMJMesh
{
	class MaxLoadTreePartitioner : public PMJMesh::TreePartitioner
	{
	public:

		MaxLoadTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses,
			LoadCalculator *loadCalculator, bool usePartitioningThreshold,
			Real maxAllowedLoad/*, bool refinement21 = false*/);
		virtual ~MaxLoadTreePartitioner();

		virtual void setUsePartitioningThreshold(bool usePartitioningThreshold);

		virtual void setMaxAllowedLoad(Real maxAllowedLoad);

		//virtual void setRefinement21(bool refinement21);

	protected:

		virtual Real findMaxLoad() const = 0;

		virtual void executePartitioning();

	protected:

		virtual bool shouldSkip(const Data::BoxTreeCell *partitionCell) const;

		virtual bool shouldSubdivide(Real load) const;
		virtual bool shouldSubdivide(const Data::BoxTreeCell *original) const;
		virtual void subdivide(Data::BoxTreeCell *c, const Data::BoxTreeCell *original);

		virtual void buildTree();

	protected:

		bool usePartitioningThreshold;

		Real maxAllowedLoad;

		//bool refinement21;
	};
}

#endif //#ifndef _PMJMESH_MAX_LOAD_TREE_PARTITIONER_H_

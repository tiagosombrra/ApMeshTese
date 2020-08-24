#ifndef _PMJMESH_BSP_TREE_PARTITIONER_H_
#define _PMJMESH_BSP_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"
#include "PMJMesh/TreePartitioner.h"

namespace PMJMesh
{
	class BSPTreePartitioner : public PMJMesh::TreePartitioner
	{
	public:

		BSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator);
		virtual ~BSPTreePartitioner();

		//virtual void setLoadCalculator(LoadCalculator *loadCalculator);

		virtual void execute();

	protected:

		Data::RealMap bspLoad;

		//LoadCalculator *loadCalculator;

	};
}

#endif //#ifndef _PMJMESH_BSP_TREE_PARTITIONER_H_

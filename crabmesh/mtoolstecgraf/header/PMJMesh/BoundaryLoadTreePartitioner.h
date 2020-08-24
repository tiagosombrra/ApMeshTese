#ifndef _PMJMESH_BOUNDARY_LOAD_TREE_PARTITIONER_H_
#define _PMJMESH_BOUNDARY_LOAD_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/MaxLoadTreePartitioner.h"

namespace PMJMesh
{
	class BoundaryLoadTreePartitioner : public PMJMesh::MaxLoadTreePartitioner
	{
	public:

		BoundaryLoadTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses,
			LoadCalculator *loadCalculator, bool useMean, Real maxAllowedLoad);
		virtual ~BoundaryLoadTreePartitioner();

	protected:

		virtual Real findMaxLoad() const;
	};
}

#endif //#ifndef _PMJMESH_BOUNDARY_LOAD_TREE_PARTITIONER_H_

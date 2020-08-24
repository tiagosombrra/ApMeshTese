#ifndef _PMJMESH_BSP_TREE_SHIFTER_H_
#define _PMJMESH_BSP_TREE_SHIFTER_H_

#include "PMJMesh/Definitions.h"
#include "PMJMesh/BoxTreeShifter.h"

namespace PMJMesh
{
	class BSPTreeShifter : public PMJMesh::BoxTreeShifter
	{
	public:

		BSPTreeShifter(UInt dimension, Data::BoxTree *tree,
			enum ShiftType type = LATERAL, Real factor = 0.5, UInt shift = 0, bool skippingCenter = true);
		virtual ~BSPTreeShifter();

	protected:

		virtual void displace(const Data::BoxTreeCell *c, const Data::Point *min, const Data::Point *max,
			const Data::GraphNodeList &moreNeighs, Data::Point *newMin, Data::Point *newMax) const;

	};
}

#endif //#ifndef _PMJMESH_BSP_TREE_SHIFTER_H_

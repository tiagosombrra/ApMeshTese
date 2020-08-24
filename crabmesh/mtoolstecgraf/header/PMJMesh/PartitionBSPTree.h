#ifndef _PMJMESH_PARTITION_BSP_TREE_H_
#define _PMJMESH_PARTITION_BSP_TREE_H_

#include "PMJMesh/Definitions.h"
#include "Data/BSPTree.h"

namespace PMJMesh
{
	class PartitionBSPTree : public Data::BSPTree
	{
	public:

		PartitionBSPTree(const Data::Point &min, const Data::Point &max, ULInt id = 0);
		PartitionBSPTree(Data::Box *box, PMJMesh::PartitionBSPTreeCell *root, ULInt id = 0);
		virtual ~PartitionBSPTree();

		using Data::BSPTree::neighbors;
		virtual void neighbors(const Data::BoxTreeCell *c, const Data::UIntVector &sides, Data::GraphNodeList &neighs) const;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_BSP_TREE_H_

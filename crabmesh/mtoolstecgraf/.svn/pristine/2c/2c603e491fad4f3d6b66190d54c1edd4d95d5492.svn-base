#ifndef _PMJMESH2_PARTITION_BSP_TREE_H_
#define _PMJMESH2_PARTITION_BSP_TREE_H_

#include "PMJMesh2/Definitions.h"
#include "Data/BSPTree.h"

namespace PMJMesh2
{
    class PartitionBSPTree : public Data::BSPTree
    {
    public:

        PartitionBSPTree(const Data::Point &min, const Data::Point &max, ULInt id = 0);
		PartitionBSPTree(Data::Box *box, PartitionBSPTreeCell *root, ULInt id = 0);
		virtual ~PartitionBSPTree();

    };
}

#endif //#ifndef _PMJMESH2_PARTITION_BSP_TREE_H_

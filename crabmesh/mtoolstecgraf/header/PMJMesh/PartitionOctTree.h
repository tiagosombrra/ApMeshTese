#ifndef _PMJMESH_PARTITION_OCT_TREE_H_
#define _PMJMESH_PARTITION_OCT_TREE_H_

#include "PMJMesh/Definitions.h"

#include "Data/OctTree.h"

namespace PMJMesh
{
	class PartitionOctTree : public Data::OctTree
	{
	public:

		PartitionOctTree(const Data::Point3D &min, const Data::Point3D &max, ULInt id = 0);
		PartitionOctTree(Data::Box *box, PMJMesh::PartitionOctTreeCell *root, ULInt id = 0);
		virtual ~PartitionOctTree();

		using Data::OctTree::neighbors;
		virtual Data::GraphNodeList neighbors(const Data::BoxTreeCell *c, UInt side) const;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_OCT_TREE_H_

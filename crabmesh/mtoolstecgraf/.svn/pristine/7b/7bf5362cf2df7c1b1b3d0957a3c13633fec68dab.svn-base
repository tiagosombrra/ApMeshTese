#ifndef _PMJMESH_PARTITION_QUAD_TREE_H_
#define _PMJMESH_PARTITION_QUAD_TREE_H_

#include "PMJMesh/Definitions.h"

#include "Data/QuadTree.h"

namespace PMJMesh
{
	class PartitionQuadTree : public Data::QuadTree
	{
	public:

		PartitionQuadTree(const Data::Point2D &min, const Data::Point2D &max, ULInt id = 0);
		PartitionQuadTree(Data::Box *box, PMJMesh::PartitionQuadTreeCell *root, ULInt id = 0);
		virtual ~PartitionQuadTree();

		using Data::QuadTree::neighbors;
		virtual Data::GraphNodeList neighbors(const Data::BoxTreeCell *c, UInt side) const;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_QUAD_TREE_H_

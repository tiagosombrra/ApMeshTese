#include "PMJMesh/PartitionBSPTree.h"

#include "Data/Point.h"
#include "Data/Box.h"
#include "PMJMesh/PartitionBSPTreeCell.h"

PMJMesh::PartitionBSPTree::PartitionBSPTree(const Data::Point &min, const Data::Point &max, ULInt id) :
	Data::BSPTree(new Data::Box(min, max), new PMJMesh::PartitionBSPTreeCell(NULL, 0), id)
{

}

PMJMesh::PartitionBSPTree::PartitionBSPTree(Data::Box *box, PMJMesh::PartitionBSPTreeCell *root, ULInt id) :
	Data::BSPTree(box, root, id)
{

}

PMJMesh::PartitionBSPTree::~PartitionBSPTree()
{

}

void PMJMesh::PartitionBSPTree::neighbors(const Data::BoxTreeCell *c, const Data::UIntVector &sides, Data::GraphNodeList &neighs) const
{
	if (!c)
	{
		return;
	}

	if (!c->isLeaf())
	{
		Data::BSPTree::neighbors(c, sides, neighs);

		return;
	}

	PartitionBSPTreeCell *pc = const_cast<PartitionBSPTreeCell *>(static_cast<const PartitionBSPTreeCell *>(c));

	if (pc->hasNeighbors(sides))
	{
		pc->getNeighbors(sides, neighs);

		return;
	}

	Data::GraphNodeList ns;

	Data::BSPTree::neighbors(c, sides, ns);

	pc->setNeighbors(sides, ns);

	neighs.splice(neighs.end(), ns);
}

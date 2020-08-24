#include "PMJMesh/PartitionQuadTree.h"

#include "Data/Box.h"
#include "PMJMesh/PartitionQuadTreeCell.h"

PMJMesh::PartitionQuadTree::PartitionQuadTree(const Data::Point2D &min, const Data::Point2D &max, ULInt id) :
	//Data::QuadTree(min, max, id)
	Data::QuadTree(new Data::Box(min, max), new PMJMesh::PartitionQuadTreeCell(NULL, 0), id)
{
	/*this->nodes.clear();

	delete this->getRoot();

	this->setRoot(new PMJMesh::PartitionQuadTreeCell(NULL, 0));*/
}

PMJMesh::PartitionQuadTree::PartitionQuadTree(Data::Box *box, PMJMesh::PartitionQuadTreeCell *root, ULInt id) :
	Data::QuadTree(box, root, id)
{

}

PMJMesh::PartitionQuadTree::~PartitionQuadTree()
{

}

Data::GraphNodeList PMJMesh::PartitionQuadTree::neighbors(const Data::BoxTreeCell *c, UInt side) const
{
	if (!c)
	{
		return Data::GraphNodeList();
	}

	if (!c->isLeaf())
	{
		return Data::QuadTree::neighbors(c, side);
	}

	PMJMesh::PartitionQuadTreeCell *pc = const_cast<PMJMesh::PartitionQuadTreeCell *>(static_cast<const PMJMesh::PartitionQuadTreeCell *>(c));

	if (!pc->hasNeighbors(side))
	{
		pc->setNeighbors(side, Data::QuadTree::neighbors(c, side));
	}

	return pc->getNeighbors(side);
}

#include "PMJMesh/PartitionOctTree.h"

#include "Data/Box.h"
#include "PMJMesh/PartitionOctTreeCell.h"

PMJMesh::PartitionOctTree::PartitionOctTree(const Data::Point3D &min, const Data::Point3D &max, ULInt id) :
	//Data::OctTree(min, max, id)
	Data::OctTree(new Data::Box(min, max), new PMJMesh::PartitionOctTreeCell(NULL, 0), id)
{
	/*this->nodes.clear();

	delete this->getRoot();

	this->setRoot(new PMJMesh::PartitionOctTreeCell(NULL, 0));*/
}

PMJMesh::PartitionOctTree::PartitionOctTree(Data::Box *box, PMJMesh::PartitionOctTreeCell *root, ULInt id) :
	Data::OctTree(box, root, id)
{

}

PMJMesh::PartitionOctTree::~PartitionOctTree()
{

}

Data::GraphNodeList PMJMesh::PartitionOctTree::neighbors(const Data::BoxTreeCell *c, UInt side) const
{
	if (!c)
	{
		return Data::GraphNodeList();
	}

	if (!c->isLeaf())
	{
		return Data::OctTree::neighbors(c, side);
	}

	PMJMesh::PartitionOctTreeCell *pc = const_cast<PMJMesh::PartitionOctTreeCell *>(static_cast<const PMJMesh::PartitionOctTreeCell *>(c));

	if (!pc->hasNeighbors(side))
	{
		pc->setNeighbors(side, Data::OctTree::neighbors(c, side));
	}

	return pc->getNeighbors(side);
}

#include "PMJMesh/PartitionBSPTreeCell.h"

#include "PMJMesh/PartitionBSPTree.h"

PMJMesh::PartitionBSPTreeCell::PartitionBSPTreeCell(PMJMesh::PartitionBSPTreeCell *parent, ULInt id) :
	Data::BSPTreeCell::BSPTreeCell(parent, id)
{

}

PMJMesh::PartitionBSPTreeCell::~PartitionBSPTreeCell()
{

}

void PMJMesh::PartitionBSPTreeCell::subdivide()
{
	ULInt id = 2*this->dimension()*this->getId() + 2*this->getSplitDirection();

	new PartitionBSPTreeCell(this, ++id);
	new PartitionBSPTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

bool PMJMesh::PartitionBSPTreeCell::hasNeighbors(Data::UIntVector sides) const
{
	std::sort(sides.begin(), sides.end());

	return (this->neighs.find(sides) != this->neighs.end());
}

void PMJMesh::PartitionBSPTreeCell::setNeighbors(Data::UIntVector sides, const Data::GraphNodeList &neighs)
{
	std::sort(sides.begin(), sides.end());

	this->neighs[sides] = neighs;
}

void PMJMesh::PartitionBSPTreeCell::getNeighbors(Data::UIntVector sides, Data::GraphNodeList &neighs) const
{
	std::sort(sides.begin(), sides.end());

	NeighborMap::const_iterator it = this->neighs.find(sides);

	if (it == this->neighs.end())
	{
		return;
	}

	neighs.insert(neighs.end(), (*it).second.begin(), (*it).second.end());
}

void PMJMesh::PartitionBSPTreeCell::clearNeighbors()
{
	this->neighs.clear();
}

#if USE_ARRAY
PMJMesh::PartitionBSPTreeCell &PMJMesh::PartitionBSPTreeCell::operator=(const PartitionBSPTreeCell &c)
{
	if (this != &c)
	{
		Data::BSPTreeCell::operator=(c);

		this->neighs = c.neighs;
	}

	return *this;
}
#endif //#if USE_ARRAY

#include "PMJMesh/PartitionOctTreeCell.h"

#include "PMJMesh/PartitionOctTree.h"

using namespace PMJMesh;

PMJMesh::PartitionOctTreeCell::PartitionOctTreeCell(Data::BoxTreeCell *original,
	PMJMesh::PartitionOctTreeCell *parent, ULInt id) :
	Data::OctTreeCell(parent, id)
{
	this->setOriginal(original);

	this->neighborsSet.resize(6);

	for (UInt i = 0; i < 6; i++)
	{
		this->neighborsSet[i] = false;
	}
}

PMJMesh::PartitionOctTreeCell::~PartitionOctTreeCell()
{

}

void PMJMesh::PartitionOctTreeCell::setOriginal(Data::BoxTreeCell *original)
{
	this->original = original;
}

Data::BoxTreeCell *PMJMesh::PartitionOctTreeCell::getOriginal() const
{
	return this->original;
}

void PMJMesh::PartitionOctTreeCell::subdivide()
{
	ULInt id = 8*this->getId();

	for (UInt i = 0; i < 8; i++)
	{
		new PMJMesh::PartitionOctTreeCell(static_cast<Data::BoxTreeCell *>(this->original->child(i)), this, ++id);
	}

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)

	this->clearNeighbors();
}

bool PMJMesh::PartitionOctTreeCell::hasNeighbors(UInt side) const
{
	return (side < 6) ? this->neighborsSet[side] : false;
}

void PMJMesh::PartitionOctTreeCell::setNeighbors(UInt side, const Data::GraphNodeList &neighbors)
{
	if (side < 6)
	{
		this->neighbors[side] = neighbors;
		this->neighborsSet[side] = true;
	}
}

Data::GraphNodeList PMJMesh::PartitionOctTreeCell::getNeighbors(UInt side) const
{
	return (side < 6) ? this->neighbors[side] : Data::GraphNodeList();
}

void PMJMesh::PartitionOctTreeCell::clearNeighbors()
{
	for (UInt i = 0; i < 6; i++)
	{
		//for (UInt j = 0; j < this->neighbors[i].size(); i++)
		for (Data::GraphNodeList::iterator iter = this->neighbors[i].begin();
			 iter != neighbors[i].end(); iter++)
		{
			//PMJMesh::PartitionOctTreeCell *c = static_cast<PMJMesh::PartitionOctTreeCell *>(this->neighbors[i][j]);
			PMJMesh::PartitionOctTreeCell *c = static_cast<PMJMesh::PartitionOctTreeCell *>((*iter));

			UInt opposite = (i + 3)%6;

			c->neighborsSet[opposite] = false;
			c->neighbors[opposite].clear();
		}

		this->neighborsSet[i] = false;
		this->neighbors[i].clear();
	}
}

#if USE_ARRAY
PartitionOctTreeCell &PMJMesh::PartitionOctTreeCell::operator=(const PartitionOctTreeCell &c)
{
	if (this != &c)
	{
		Data::OctTreeCell::operator=(c);

		for (UInt i = 0; i < 6; i++)
		{
			this->neighbors[i] = c.neighbors[i];
		}

		this->neighborsSet = c.neighborsSet;

		this->original = c.original;
	}

	return *this;
}
#endif //#if USE_ARRAY

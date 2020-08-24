#include "PMJMesh/PartitionQuadTreeCell.h"

#include "PMJMesh/PartitionQuadTree.h"

using namespace PMJMesh;

PMJMesh::PartitionQuadTreeCell::PartitionQuadTreeCell(Data::BoxTreeCell *original,
	PMJMesh::PartitionQuadTreeCell *parent, ULInt id) :
	Data::QuadTreeCell(parent, id)
{
	this->setOriginal(original);

	this->neighborsSet.resize(4);

	for (UInt i = 0; i < 4; i++)
	{
		this->neighborsSet[i] = false;
	}
}

PMJMesh::PartitionQuadTreeCell::~PartitionQuadTreeCell()
{

}

void PMJMesh::PartitionQuadTreeCell::setOriginal(Data::BoxTreeCell *original)
{
	this->original = original;
}

Data::BoxTreeCell *PMJMesh::PartitionQuadTreeCell::getOriginal() const
{
	return this->original;
}

void PMJMesh::PartitionQuadTreeCell::subdivide()
{
	ULInt id = 4*this->getId();

	for (UInt i = 0; i < 4; i++)
	{
		new PMJMesh::PartitionQuadTreeCell(static_cast<Data::BoxTreeCell *>(this->original->child(i)), this, ++id);
	}

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)

	this->clearNeighbors();
}

bool PMJMesh::PartitionQuadTreeCell::hasNeighbors(UInt side) const
{
	return (side < 4) ? this->neighborsSet[side] : false;
}

void PMJMesh::PartitionQuadTreeCell::setNeighbors(UInt side, const Data::GraphNodeList &neighbors)
{
	if (side < 4)
	{
		this->neighbors[side] = neighbors;
		this->neighborsSet[side] = true;
	}
}

Data::GraphNodeList PMJMesh::PartitionQuadTreeCell::getNeighbors(UInt side) const
{
	return (side < 4) ? this->neighbors[side] : Data::GraphNodeList();
}

void PMJMesh::PartitionQuadTreeCell::clearNeighbors()
{
	for (UInt i = 0; i < 4; i++)
	{
		//for (UInt j = 0; j < this->neighbors[i].size(); j++)
		for (Data::GraphNodeList::iterator iter = this->neighbors[i].begin();
			 iter != this->neighbors[i].end(); iter++)
		{
			//PMJMesh::PartitionQuadTreeCell *c = static_cast<PMJMesh::PartitionQuadTreeCell *>(this->neighbors[i][j]);
			PMJMesh::PartitionQuadTreeCell *c = static_cast<PMJMesh::PartitionQuadTreeCell *>((*iter));

			UInt opposite = (i + 2)%4;

			c->neighborsSet[opposite] = false;
			c->neighbors[opposite].clear();
		}

		this->neighborsSet[i] = false;
		this->neighbors[i].clear();
	}
}

#if USE_ARRAY
PartitionQuadTreeCell &PMJMesh::PartitionQuadTreeCell::operator=(const PartitionQuadTreeCell &c)
{
	if (this != &c)
	{
		Data::QuadTreeCell::operator=(c);

		for (UInt i = 0; i < 4; i++)
		{
			this->neighbors[i] = c.neighbors[i];
		}

		this->neighborsSet = c.neighborsSet;

		this->original = c.original;
	}

	return *this;
}
#endif //#if USE_ARRAY

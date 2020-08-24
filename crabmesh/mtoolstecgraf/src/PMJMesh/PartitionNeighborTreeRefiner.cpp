#include "PMJMesh/PartitionNeighborTreeRefiner.h"

/*#include "PMJMesh/PartitionQuadTreeCell.h"
#include "PMJMesh/PartitionOctTreeCell.h"
#include "Data/BoxTreeCell.h"

PMJMesh::PartitionNeighborTreeRefiner::PartitionNeighborTreeRefiner(UInt dimension,
	Data::BoxTree *tree, const Data::BoxTree *original,
	Data::ULIntVector *originalNewCells) :
	MJMesh::NeighborTreeRefiner(tree)
{
	this->setDimension(dimension);

	this->setOriginal(original);

	this->originalNewCells = originalNewCells;
}

PMJMesh::PartitionNeighborTreeRefiner::~PartitionNeighborTreeRefiner()
{

}

void PMJMesh::PartitionNeighborTreeRefiner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::PartitionNeighborTreeRefiner::refine(Data::BoxTreeCell *c)
{
	if (((this->dimension == 2) && (static_cast<PMJMesh::PartitionQuadTreeCell *>(c)->getOriginal()->isLeaf())) ||
		(static_cast<PMJMesh::PartitionOctTreeCell *>(c)->getOriginal()->isLeaf()))
	{
		return;
	}

	MJMesh::NeighborTreeRefiner::refine(c);
}

void PMJMesh::PartitionNeighborTreeRefiner::subdivide(Data::BoxTreeCell *c)
{
	const Data::BoxTreeCell *original = (this->dimension == 2) ?
		static_cast<PMJMesh::PartitionQuadTreeCell *>(c)->getOriginal() :
		static_cast<PMJMesh::PartitionOctTreeCell *>(c)->getOriginal();

	if (!original->isSubdivided())
	{
		//this is only true if this refinement 2:1 is enabled and the minimum
		// level refinement has been postponed, otherwise, an error just
		// happened.
		const_cast<Data::BoxTreeCell *>(original)->subdivide();

		for (UInt i = 0; i < original->numChildren(); i++)
		{
			if (this->originalNewCells)
			{
				this->originalNewCells->push_back(original->child(i)->getId());
			}
		}

		//std::cout << "OPA, ERRO em PartitionNeighborTreeRefiner::subdivide! c->id = " << c->getId() << ", original->id = " << original->getId() << std::endl;

		//return;
	}

	c->subdivide();
}

void PMJMesh::PartitionNeighborTreeRefiner::setOriginal(const Data::BoxTree *original)
{
	this->original = original;
}*/

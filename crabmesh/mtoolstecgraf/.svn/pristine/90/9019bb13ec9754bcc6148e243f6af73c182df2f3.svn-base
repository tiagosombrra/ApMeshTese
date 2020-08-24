#include "PMJMesh/BoundaryMinNumberBSPTreePartitioner.h"

#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

PMJMesh::BoundaryMinNumberBSPTreePartitioner::BoundaryMinNumberBSPTreePartitioner(
	UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
	bool usePartitioningThreshold, UInt minNumPartitions) :
		PMJMesh::MinNumberBSPTreePartitioner::MinNumberBSPTreePartitioner(dimension,
			tree, load, skippingInner, numProcesses, loadCalculator, usePartitioningThreshold, minNumPartitions)
{

}

PMJMesh::BoundaryMinNumberBSPTreePartitioner::~BoundaryMinNumberBSPTreePartitioner()
{

}

bool PMJMesh::BoundaryMinNumberBSPTreePartitioner::shouldCount(const Data::BoxTreeCell *cell, bool increasing) const
{
	bool count = PMJMesh::MinNumberBSPTreePartitioner::shouldCount(cell, increasing);

	if (!count)
	{
		return count;
	}

	return this->hasOnCell(cell->box());
}

bool PMJMesh::BoundaryMinNumberBSPTreePartitioner::hasOnCell(const Data::Box *box) const
{
	return this->hasOnCell(box, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
}

bool PMJMesh::BoundaryMinNumberBSPTreePartitioner::hasOnCell(const Data::Box *box, const Data::BoxTreeCell *cell, bool test) const
{
	if (test)
	{
		if (box->optIn(cell->box()))
		{
			return this->hasOnCell(box, cell, false);
		}

		if (cell->outOf(box))
		{
			return false;
		}
	}

	Data::Position pos = PMJMesh::getPosition(this->dimension, cell);

	if ((pos == Data::OUT) || (pos == Data::IN))
	{
		return false;
	}

	if ((!test) && (pos == Data::ON))
	{
		return true;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		if (this->hasOnCell(box, static_cast<Data::BoxTreeCell *>(cell->child(i)), test))
		{
			return true;
		}
	}

	return false;
}

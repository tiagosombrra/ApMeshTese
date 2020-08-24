#include "PMJMesh/BoundaryNumberTreePartitioner.h"

#include "Data/BoxTreeCell.h"
#include "PMJMesh/PartitionQuadTreeCell.h"
#include "PMJMesh/PartitionOctTreeCell.h"

PMJMesh::BoundaryNumberTreePartitioner::BoundaryNumberTreePartitioner(
	UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses,
	LoadCalculator *loadCalculator, bool useMean, UInt minNumPartitions
	/*, bool refinement21*/) :
		PMJMesh::MinNumberTreePartitioner::MinNumberTreePartitioner(dimension,
			tree, load, skippingInner, numProcesses, loadCalculator, useMean,
			minNumPartitions/*, refinement21*/)
{

}

PMJMesh::BoundaryNumberTreePartitioner::~BoundaryNumberTreePartitioner()
{

}

bool PMJMesh::BoundaryNumberTreePartitioner::shouldCount(const Data::BoxTreeCell *cell, bool increasing) const
{
	bool count = PMJMesh::MinNumberTreePartitioner::shouldCount(cell, increasing);

	if (!count)
	{
		return count;
	}

	Data::BoxTreeCell *original = (this->dimension == 2) ?
		static_cast<const PMJMesh::PartitionQuadTreeCell *>(cell)->getOriginal() :
		static_cast<const PMJMesh::PartitionOctTreeCell *>(cell)->getOriginal();

	Data::Position pos = PMJMesh::getPosition(this->dimension, original);

	//if (PMJMesh::getPosition(this->dimension, original) != Data::ON)
	if ((pos == Data::IN) || (pos == Data::OUT))
	{
		return false;
	}

	return true;
}

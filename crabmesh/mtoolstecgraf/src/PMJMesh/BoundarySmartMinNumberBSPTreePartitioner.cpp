#include "PMJMesh/BoundarySmartMinNumberBSPTreePartitioner.h"

#include "Data/BoxTreeCell.h"

PMJMesh::BoundarySmartMinNumberBSPTreePartitioner::BoundarySmartMinNumberBSPTreePartitioner(
	UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator, bool usePartitioningThreshold,
	UInt minNumPartitions, UInt removeLayersInLoad, bool proportional) :
		PMJMesh::SmartMinNumberBSPTreePartitioner::SmartMinNumberBSPTreePartitioner(
			dimension, tree, load, skippingInner, numProcesses, loadCalculator, usePartitioningThreshold,
			minNumPartitions, removeLayersInLoad, proportional)
{

}

PMJMesh::BoundarySmartMinNumberBSPTreePartitioner::~BoundarySmartMinNumberBSPTreePartitioner()
{

}

bool PMJMesh::BoundarySmartMinNumberBSPTreePartitioner::shouldCount(const Data::BoxTreeCell *cell, bool increasing) const
{
	bool count = PMJMesh::MinNumberBSPTreePartitioner::shouldCount(cell, increasing);

	if (!count)
	{
		return count;
	}

#if USE_C__11
	BSPBoxHashMap::const_iterator it = this->bspMap.find(const_cast<Data::BoxTreeCell *>(cell));
#else
	BSPBoxMap::const_iterator it = this->bspMap.find(const_cast<Data::BoxTreeCell *>(cell));
#endif //#if USE_C__11

	for (Data::GraphNodeList::const_iterator iter = (*it).second.begin();
		 iter != (*it).second.end(); iter++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		if (PMJMesh::getPosition(this->dimension, c) == Data::ON)
		{
			return true;
		}
	}

	return false;
}

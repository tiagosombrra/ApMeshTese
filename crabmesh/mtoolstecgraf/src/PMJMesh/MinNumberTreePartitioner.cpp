#include "PMJMesh/MinNumberTreePartitioner.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Box.h"
#include "PMJMesh/PartitionQuadTree.h"
#include "PMJMesh/PartitionQuadTreeCell.h"
#include "PMJMesh/PartitionOctTree.h"
#include "PMJMesh/PartitionOctTreeCell.h"
#include "PMJMesh/LoadCalculator.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/PartitionNeighborTreeRefiner.h"

using namespace PMJMesh;

PMJMesh::MinNumberTreePartitioner::MinNumberTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
	bool usePartitioningThreshold, UInt minNumPartitions/*, bool refinement21*/) :
		PMJMesh::TreePartitioner(dimension, tree, load, skippingInner, numProcesses, loadCalculator)
{
	this->setUsePartitioningThreshold(usePartitioningThreshold);
	this->setMinNumPartitions(minNumPartitions);
	//this->setRefinement21(refinement21);
}

PMJMesh::MinNumberTreePartitioner::~MinNumberTreePartitioner()
{

}

void PMJMesh::MinNumberTreePartitioner::setUsePartitioningThreshold(bool usePartitioningThreshold)
{
	this->usePartitioningThreshold = usePartitioningThreshold;
}

void PMJMesh::MinNumberTreePartitioner::setMinNumPartitions(UInt minNumPartitions)
{
	this->minNumPartitions = minNumPartitions;
}

/*void PMJMesh::MinNumberTreePartitioner::setRefinement21(bool refinement21)
{
	this->refinement21 = refinement21;
}*/

UInt PMJMesh::MinNumberTreePartitioner::findMinNumPartitions() const
{
	return this->numProcesses;
}

bool PMJMesh::MinNumberTreePartitioner::shouldCount(const Data::BoxTreeCell *cell, bool increasing) const
{
	return increasing ? ((*this->load->find(cell->getId())).second > 0.0) : true;
}

void PMJMesh::MinNumberTreePartitioner::executePartitioning()
{
	if (!this->usePartitioningThreshold)
	{
		this->minNumPartitions = this->findMinNumPartitions();

		//std::cout << "usethreshold = " << this->usePartitioningThreshold << ", numprocesses = " << this->numProcesses << ", minNumPartitions = " << this->minNumPartitions << std::endl;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	std::cout << "total load = " << (*this->load->find(this->tree->getRoot()->getId())).second << std::endl;
	std::cout << "min num partitions = " << this->minNumPartitions << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->buildTree();

	/*if (this->refinement21)
	{
		PartitionNeighborTreeRefiner *refiner = new PartitionNeighborTreeRefiner(
			this->dimension, this->partitionTree, this->tree, &this->treeNewCells);

		refiner->execute();

		delete refiner;
	}*/
}

bool PMJMesh::MinNumberTreePartitioner::shouldSkip(const Data::BoxTreeCell *partitionCell) const
{
	if (!this->skippingInner)
	{
		return false;
	}

	Data::BoxTreeCell *original = (this->dimension == 2) ?
		static_cast<const PMJMesh::PartitionQuadTreeCell *>(partitionCell)->getOriginal() :
		static_cast<const PMJMesh::PartitionOctTreeCell *>(partitionCell)->getOriginal();

	Data::Position pos = PMJMesh::getPosition(this->dimension, original);

	if (pos == Data::IN)
	{
		return true;
	}

	if (original->isLeaf())
	{
		return true;
	}

	for (UInt i = 0; i < original->numChildren(); i++)
	{
		Data::Position cpos = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(original->child(i)));

		if (cpos == Data::IN)
		{
			return true;
		}
	}

	return false;
}

void PMJMesh::MinNumberTreePartitioner::buildTree()
{
	Data::BoxTreeCell *originalCell = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	Data::Box *box = originalCell->box();

	if (this->dimension == 2)
	{
		Data::Point2D min(box->getMin());
		Data::Point2D max(box->getMax());

		this->partitionTree = new PMJMesh::PartitionQuadTree(min, max);
	}
	else if (this->dimension == 3)
	{
		Data::Point3D min(box->getMin());
		Data::Point3D max(box->getMax());

		this->partitionTree = new PMJMesh::PartitionOctTree(min, max);
	}

	Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->partitionTree->getRoot());

	if (dimension == 2)
	{
		static_cast<PMJMesh::PartitionQuadTreeCell *>(cell)->setOriginal(originalCell);
	}
	else
	{
		static_cast<PMJMesh::PartitionOctTreeCell *>(cell)->setOriginal(originalCell);
	}

	UInt numPartitions = 1;

	while (numPartitions < this->minNumPartitions)
	{
		cell = NULL;
		Real maxLoad = -REAL_MAX;

		//Data::GraphNodeList leaves = this->partitionTree->getLeaves();

		//for (Data::GraphNodeList::iterator iter = leaves.begin();
		for (Data::Tree::LeafIterator iter = this->partitionTree->lBegin(), end = this->partitionTree->lEnd();
			 //iter != leaves.end(); iter++)
			 iter != end; iter++)
		{
			Real load = (*this->load->find((*iter)->getId())).second;

			if ((load <= 0.0) || (this->isSkipped(static_cast<Data::BoxTreeCell *>((*iter)))))
			{
				continue;
			}

			if ((load > maxLoad) ||
				((load >= maxLoad) &&
				 (static_cast<Data::BoxTreeCell *>((*iter))->level() < cell->level())) ||
				((load >= maxLoad) &&
				 (static_cast<Data::BoxTreeCell *>((*iter))->level() == cell->level()) &&
				 ((*iter)->getId() < cell->getId())))
			{
				maxLoad = load;

				cell = static_cast<Data::BoxTreeCell *>((*iter));
			}
		}

		if (!cell)
		{
			break;
		}

		if (this->shouldSkip(cell))
		{
			this->skip(cell);

			continue;
		}

		originalCell = static_cast<Data::BoxTreeCell *>(this->tree->node(cell->getId()));

		bool subdivided = false;

		if (!originalCell->isSubdivided())
		{
			originalCell->subdivide();

			/*if (this->loadCalculator->isAddingLoadForNodes())
			{
				maxLoad -= this->loadCalculator->loadFor(PMJMesh::getPosition(this->dimension, originalCell));
			}*/

			maxLoad /= static_cast<Real>(originalCell->numChildren());

			subdivided = true;
		}

		cell->subdivide();

		if (this->shouldCount(cell, false))
		{
			numPartitions--;
		}

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			if (this->dimension == 2)
			{
				static_cast<PartitionQuadTreeCell *>(cell->child(i))->setOriginal(static_cast<Data::BoxTreeCell *>(originalCell->child(i)));
			}
			else
			{
				static_cast<PartitionOctTreeCell *>(cell->child(i))->setOriginal(static_cast<Data::BoxTreeCell *>(originalCell->child(i)));
			}

			if (subdivided)
			{
				ULInt id = originalCell->child(i)->getId();

				this->treeNewCells.push_back(id);

#if USE_C__11
				const_cast<Data::RealHashMap *>(this->load)->insert(std::make_pair(id, maxLoad));
#else
				const_cast<Data::RealMap *>(this->load)->insert(std::make_pair(id, maxLoad));
#endif //#if USE_C__11
			}

			if (this->shouldCount(static_cast<Data::BoxTreeCell *>(cell->child(i)), true))
			{
				numPartitions++;
			}
		}
	}
}

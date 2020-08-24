#include "PMJMesh/MinNumberBSPTreePartitioner.h"

#include "Data/Box.h"
#include "Data/Point.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh/PartitionBSPTree.h"
#include "PMJMesh/PartitionBSPTreeCell.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace PMJMesh;

PMJMesh::MinNumberBSPTreePartitioner::MinNumberBSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
	bool usePartitioningThreshold, UInt minNumPartitions) :
		PMJMesh::BSPTreePartitioner::BSPTreePartitioner(dimension, tree, load,
			skippingInner, numProcesses, loadCalculator)
{
	this->setUsePartitioningThreshold(usePartitioningThreshold);
	this->setMinNumPartitions(minNumPartitions);
}

PMJMesh::MinNumberBSPTreePartitioner::~MinNumberBSPTreePartitioner()
{

}

void PMJMesh::MinNumberBSPTreePartitioner::setUsePartitioningThreshold(bool usePartitioningThreshold)
{
	this->usePartitioningThreshold = usePartitioningThreshold;
}

void PMJMesh::MinNumberBSPTreePartitioner::setMinNumPartitions(UInt minNumPartitions)
{
	this->minNumPartitions = minNumPartitions;
}

UInt PMJMesh::MinNumberBSPTreePartitioner::findMinNumPartitions() const
{
	return this->numProcesses;
}

bool PMJMesh::MinNumberBSPTreePartitioner::shouldCount(const Data::BoxTreeCell */*cell*/, bool /*increasing*/) const
{
	//return increasing ? ((*this->bspLoad.find(cell->getId())).second > 0.0) : true;
	return true;
}

void PMJMesh::MinNumberBSPTreePartitioner::executePartitioning()
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

	///TODO create a class to do this
	this->buildTree();
}

bool PMJMesh::MinNumberBSPTreePartitioner::shouldSkip(const Data::Box *box, const Data::BoxTreeCell *cell) const
{
	if (box->optIn(cell->box()))
	{
		Data::Position pos = PMJMesh::getPosition(this->dimension, cell);

		return (pos != Data::ON);
	}

	if (cell->outOf(box))
	{
		return true;
	}

	Data::Position pos = PMJMesh::getPosition(this->dimension, cell);

	if ((pos == Data::IN) || (pos == Data::OUT))
	{
		return true;
	}

	bool skip = true;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		if (!this->shouldSkip(box, static_cast<Data::BoxTreeCell *>(cell->child(i))))
		{
			skip = false;

			break;
		}
	}

	return skip;
}

bool PMJMesh::MinNumberBSPTreePartitioner::shouldSkip(const Data::Box *box) const
{
	if (!this->skippingInner)
	{
		return false;
	}

	return this->shouldSkip(box, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
}

bool PMJMesh::MinNumberBSPTreePartitioner::shouldSkip(const Data::BoxTreeCell */*partitionCell*/) const
{
	return false;
}

void PMJMesh::MinNumberBSPTreePartitioner::buildTree()
{
	Data::Point *min = static_cast<Data::Box *>(static_cast<const Data::BoxTree *>(this->tree)->getShape())->getMin();
	Data::Point *max = static_cast<Data::Box *>(static_cast<const Data::BoxTree *>(this->tree)->getShape())->getMax();

	this->partitionTree = new PartitionBSPTree(*min, *max);

	this->bspLoad[this->partitionTree->getRoot()->getId()] = (*this->load->find(this->tree->getRoot()->getId())).second;

	UInt numPartitions = 1;

	while (numPartitions < this->minNumPartitions)
	{
		Data::BoxTreeCell *cell = NULL;
		Real maxLoad = -REAL_MAX;

		//Data::GraphNodeList leaves = this->partitionTree->getLeaves();

		//for (Data::GraphNodeList::iterator iter = leaves.begin();
		for (Data::Tree::LeafIterator iter = this->partitionTree->lBegin(), end = this->partitionTree->lEnd();
			 //iter != leaves.end(); iter++)
			 iter != end; iter++)
		{
			Real load = (*this->bspLoad.find((*iter)->getId())).second;

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

		//Data::BoxTreeCell *originalCell = this->findCell(cell->box());

		//bool subdivided = false;

		/*if (!originalCell->isSubdivided())
		{
			originalCell->subdivide();

			Real load = (*this->load.find((*originalCell)->getId())).second/static_cast<Real>(originalCell->numChildren());

			//subdivided = true;

			for (UInt i = 0; i < originalCell->numChildren(); i++)
			{
				ULInt id = originalCell->child(i)->getId();

				this->treeNewCells.push_back(id);

				const_cast<Data::RealMap *>(this->load)->insert(std::make_pair(id, load));
			}
		}*/

		/*///TODO ajeitar
		Real loads[this->dimension][2];
		Data::Point mid = originalCell->mid();

		for (UInt i = 0; i < this->dimension; i++)
		{
			loads[i][0] = loads[i][1] = 0.0;

			Real coord = mid.getCoord(i);

			if ((coord > cell->box()->getMin()->getCoord(i)) &&
				(coord < cell->box()->getMax()->getCoord(i)))
			{
				for (UInt j = 0; j < originalCell->numChildren(); j++)
				{
					Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(originalCell->child(j));

					bool smaller = (child->box()->getMax()->getCoord(i) <= coord + Data::getTolerance());

					loads[i][smaller ? 0 : 1] += (*this->load->find(child->getId())).second;
				}
			}
		}*/

		Real loads[this->dimension][2];
#if USE_EXPERIMENTAL_3
		Data::Point2D min2d, max2d, mid2d;
		Data::Point3D min3d, max3d, mid3d;

		Data::Point &min = (this->dimension == 2) ?
            (min2d = cell->min2D(), static_cast<Data::Point &>(min2d)) :
            (min3d = cell->min3D(), static_cast<Data::Point &>(min3d));
        Data::Point &max = (this->dimension == 2) ?
            (max2d = cell->max2D(), static_cast<Data::Point &>(max2d)) :
            (max3d = cell->max3D(), static_cast<Data::Point &>(max3d));
        Data::Point &mid = (this->dimension == 2) ?
            (mid2d = cell->mid2D(), static_cast<Data::Point &>(mid2d)) :
            (mid3d = cell->mid3D(), static_cast<Data::Point &>(mid3d));
#else
		Data::Point min = cell->min();
		Data::Point max = cell->max();
		Data::Point mid = cell->mid();
#endif //#if USE_EXPERIMENTAL_3

		bool skip = true;

		for (UInt i = 0; i < this->dimension; i++)
		{
			loads[i][0] = loads[i][1] = 0.0;

			Real coord = mid.getCoord(i);

			Data::Box bmin(min, max), bmax(min, max);

			bmin.getMax()->setCoord(i, coord);
			bmax.getMin()->setCoord(i, coord);

			if ((this->shouldSkip(&bmin)) || (this->shouldSkip(&bmax)))
			{
				loads[i][0] = loads[i][1] = -1.0;

				continue;
			}

			skip = false;

			this->loadCalculator->setBox(&bmin);

			this->loadCalculator->execute();

			loads[i][0] = this->loadCalculator->getLoad();

			this->loadCalculator->setBox(&bmax);

			this->loadCalculator->execute();

			loads[i][1] = this->loadCalculator->getLoad();
		}

		if (skip)
		{
			this->skip(cell);

			continue;
		}

		UInt split = this->dimension;
		Real maxSpan = -REAL_MAX;
		Real minDiff = REAL_MAX;

		for (UInt i = 0; i < this->dimension; i++)
		{
			if ((loads[i][0] <= 0.0) || (loads[i][1] <= 0.0))
			{
				continue;
			}

			Real diff = std::fabs(loads[i][0] - loads[i][1]);
			Real span = cell->box()->getMax()->getCoord(i) - cell->box()->getMin()->getCoord(i);

			if ((diff < minDiff) ||
				((diff <= minDiff) && (span > maxSpan)))
			{
				split = i;
				minDiff = diff;
				maxSpan = span;
			}
		}

		if (split == this->dimension)
		{
			split = (cell->isRoot()) ? 0 : (static_cast<PartitionBSPTreeCell *>(cell->parent())->getSplitDirection() + 1)%this->dimension;
		}

		static_cast<PartitionBSPTreeCell *>(cell)->setSplit(split, mid.getCoord(split));

		cell->subdivide();

		if (this->shouldCount(cell, false))
		{
			numPartitions--;
		}

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			this->bspLoad[cell->child(i)->getId()] = loads[split][i];

			if (this->shouldCount(static_cast<Data::BoxTreeCell *>(cell->child(i)), true))
			{
				numPartitions++;
			}
		}
	}

	this->adjustBox();
}

void PMJMesh::MinNumberBSPTreePartitioner::adjustBox() const
{
#if USE_EXPERIMENTAL_3
	Data::Point2D min2d, max2d;
	Data::Point3D min3d, max3d;

	Data::Point &min = (this->dimension == 2) ?
        static_cast<Data::Point &>(min2d) : static_cast<Data::Point &>(min3d);
    Data::Point &max = (this->dimension == 2) ?
        static_cast<Data::Point &>(max2d) : static_cast<Data::Point &>(max3d);
#else
	Data::Point min(this->dimension), max(this->dimension);
#endif //#if USE_EXPERIMENTAL_3

	for (UInt i = 0; i < this->dimension; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

	this->findBox(min, max);

	if (this->setBox(static_cast<Data::BSPTreeCell *>(this->partitionTree->getRoot()), min, max))
	{
		Data::Box *box = static_cast<Data::Box *>(static_cast<Data::BSPTree *>(this->partitionTree)->getShape());

		for (UInt i = 0; i < this->dimension; i++)
		{
			if (box->getMin()->getCoord(i) < min.getCoord(i))
			{
				box->getMin()->setCoord(i, min.getCoord(i));
			}

			if (box->getMax()->getCoord(i) > max.getCoord(i))
			{
				box->getMax()->setCoord(i, max.getCoord(i));
			}
		}
	}
}

void PMJMesh::MinNumberBSPTreePartitioner::findBox(Data::Point &min, Data::Point &max) const
{
	this->findBox(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), min, max);
}

void PMJMesh::MinNumberBSPTreePartitioner::findBox(const Data::BoxTreeCell *c, Data::Point &min, Data::Point &max) const
{
	Data::Position pos = PMJMesh::getPosition(this->dimension, c);

	if ((pos == Data::UNKNOWN) || (pos == Data::OUT))
	{
		return;
	}

	if ((pos == Data::IN) || (c->isLeaf()))
	{
		Data::Box *b = c->box();

		for (UInt i = 0; i < this->dimension; i++)
		{
			if (b->getMin()->getCoord(i) < min.getCoord(i))
			{
				min.setCoord(i, b->getMin()->getCoord(i));
			}

			if (b->getMax()->getCoord(i) > max.getCoord(i))
			{
				max.setCoord(i, b->getMax()->getCoord(i));
			}
		}

		return;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->findBox(static_cast<Data::BoxTreeCell *>(c->child(i)), min, max);
	}
}

bool PMJMesh::MinNumberBSPTreePartitioner::setBox(Data::BSPTreeCell *c, const Data::Point &min, const Data::Point &max) const
{
	if ((c->getSplitCoord() <= min.getCoord(c->getSplitDirection())) ||
		(c->getSplitCoord() >= max.getCoord(c->getSplitDirection())))
	{
		return false;
	}

	bool changed = false;

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (c->box()->getMin()->getCoord(i) < min.getCoord(i))
		{
			c->box()->getMin()->setCoord(i, min.getCoord(i));

			changed = true;
		}

		if (c->box()->getMax()->getCoord(i) > max.getCoord(i))
		{
			c->box()->getMax()->setCoord(i, max.getCoord(i));

			changed = true;
		}
	}

	if (!changed)
	{
		return false;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->setBox(static_cast<Data::BSPTreeCell *>(c->child(i)), min, max);
	}

	return true;
}

Data::BoxTreeCell *PMJMesh::MinNumberBSPTreePartitioner::findCell(const Data::Box *box)
{
	Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	while (true)
	{
		UInt pos = c->numChildren();

		for (UInt i = 0; i < c->numChildren(); i++)
		{
			//if (static_cast<Data::BoxTreeCell *>(c->child(i))->box()->in(box))
			if (static_cast<Data::BoxTreeCell *>(c->child(i))->box()->optIn(box))
			{
				pos = i;

				break;
			}
		}

		if (pos == c->numChildren())
		{
			break;
		}

		c = static_cast<Data::BoxTreeCell *>(c->child(pos));
	}

	return c;
}

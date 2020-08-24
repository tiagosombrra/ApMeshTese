#include "PMJMesh/SmartMinNumberBSPTreePartitioner.h"

#include "Data/Point.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/PartitionBSPTree.h"
#include "PMJMesh/PartitionBSPTreeCell.h"
#include "PMJMesh/LoadCalculator.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace PMJMesh;

PMJMesh::SmartMinNumberBSPTreePartitioner::SmartMinNumberBSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses,
	LoadCalculator *loadCalculator, bool usePartitioningThreshold, UInt minNumPartitions,
	UInt removeLayersInLoad, bool proportional) :
		PMJMesh::MinNumberBSPTreePartitioner::MinNumberBSPTreePartitioner(dimension,
			tree, load, skippingInner, numProcesses, loadCalculator,
			usePartitioningThreshold, minNumPartitions)
{
	this->removeLayersInLoad = removeLayersInLoad;
	this->proportional = proportional;
}

PMJMesh::SmartMinNumberBSPTreePartitioner::~SmartMinNumberBSPTreePartitioner()
{

}

void PMJMesh::SmartMinNumberBSPTreePartitioner::setCurrentProportion(const Data::BoxTreeCell *cell)
{
	this->current.min = this->current.max = 1;

	if (!this->proportional)
	{
		return;
	}

	ULInt id = cell->getId();

#if USE_C__11
	ProportionHashMap::iterator iter = this->proportions.find(id);
#else
	ProportionMap::iterator iter = this->proportions.find(id);
#endif //#if USE_C__11

	if (iter == this->proportions.end())
	{
		return;
	}

	this->current = (*iter).second;
}

void PMJMesh::SmartMinNumberBSPTreePartitioner::addProportion(const Data::BoxTreeCell *cell, UInt proportion)
{
	if ((!this->proportional) || (proportion <= 1))
	{
		return;
	}

	Proportion p;

	p.min = proportion/2;
	p.max = proportion - p.min;

	this->proportions[cell->getId()] = p;
}

void PMJMesh::SmartMinNumberBSPTreePartitioner::addChildrenProportion(const Data::BoxTreeCell *cell)
{
	UInt min = (this->bspLoad[cell->child(0)->getId()] < this->bspLoad[cell->child(1)->getId()]) ? 0 : 1;

	this->addProportion(static_cast<Data::BoxTreeCell *>(cell->child(min)), this->current.min);
	this->addProportion(static_cast<Data::BoxTreeCell *>(cell->child(1 - min)), this->current.max);
}

void PMJMesh::SmartMinNumberBSPTreePartitioner::findProportion(Real v0, Real v1, Real &prop0, Real &prop1) const
{
	prop0 = prop1 = 1.0;

	if (!this->proportional)
	{
		return;
	}

	if (v0 < v1)
	{
		prop0 = static_cast<Real>(this->current.max);
		prop1 = static_cast<Real>(this->current.min);
	}
	else
	{
		prop0 = static_cast<Real>(this->current.min);
		prop1 = static_cast<Real>(this->current.max);
	}
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::shouldSkip(const Data::GraphNodeList &cells) const
{
	if (!this->skippingInner)
	{
		return false;
	}

	if (cells.empty())
	{
		return false;
	}

	for (Data::GraphNodeList::const_iterator iter = cells.begin();
		 iter != cells.end(); iter++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		Data::Position pos = PMJMesh::getPosition(this->dimension, c);

		if (pos == Data::ON)
		{
			return false;
		}
	}

	return true;
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::shouldSkip(const Data::BoxTreeCell */*partitionCell*/) const
{
	return false;
}

//static Real debugload = 0.0;

void PMJMesh::SmartMinNumberBSPTreePartitioner::buildTree()
{
	Data::Point *min = static_cast<Data::Box *>(static_cast<const Data::BoxTree *>(this->tree)->getShape())->getMin();
	Data::Point *max = static_cast<Data::Box *>(static_cast<const Data::BoxTree *>(this->tree)->getShape())->getMax();

	this->partitionTree = new PartitionBSPTree(*min, *max);

	this->bspLoad[this->partitionTree->getRoot()->getId()] = (*this->load->find(this->tree->getRoot()->getId())).second;
	this->bspMap[static_cast<Data::BoxTreeCell *>(this->partitionTree->getRoot())] = Data::GraphNodeList(1, this->tree->getRoot());

	UInt numPartitions = 1;

	this->addProportion(static_cast<Data::BoxTreeCell *>(this->partitionTree->getRoot()), this->minNumPartitions);

	while (numPartitions < this->minNumPartitions)
	{
		//std::cout << "num partitions = " << numPartitions << " of " << this->minNumPartitions << std::endl;

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

		//std::cout << "found bsp cell " << cell->getId() << std::endl;

		//debugload = maxLoad;

		//std::cout << "debugload = " << debugload << std::endl;

		this->setCurrentProportion(static_cast<Data::BSPTreeCell *>(cell));

		Data::GraphNodeList cells[2];

		SplitCoord sc = this->findSplitCoord(static_cast<Data::BSPTreeCell *>(cell), cells);

		//std::cout << "found split coord " << sc.coord << " in direction " <<
		//			 sc.split << " with loads " << sc.load[0] << " and " <<
		//			 sc.load[1] << " and cells of sizes " << cells[0].size() <<
		//			 " and " << cells[1].size() << std::endl;

		if ((sc.load[0] <= 0.0) || (sc.load[1] <= 0.0))
		{
			this->skip(cell);

			continue;
		}

		static_cast<PartitionBSPTreeCell *>(cell)->setSplit(sc.split, sc.coord);

		cell->subdivide();

		if (this->shouldCount(cell, false))
		{
			numPartitions--;
		}

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			this->bspLoad[cell->child(i)->getId()] = sc.load[i];
			this->bspMap[static_cast<Data::BoxTreeCell *>(cell->child(i))].swap(cells[i]);

			if (this->shouldCount(static_cast<Data::BoxTreeCell *>(cell->child(i)), true))
			{
				numPartitions++;
			}
		}

		this->addChildrenProportion(cell);
	}

	this->adjustBox();

	if (this->removeLayersInLoad > 0)
	{
		for (Data::RealMap::iterator iter = this->bspLoad.begin();
			 iter != this->bspLoad.end(); iter++)
		{
			ULInt id = (*iter).first;

			Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->partitionTree->node(id));

			this->loadCalculator->setBox(c->box());

			this->loadCalculator->execute();

			(*iter).second = this->loadCalculator->getLoad();
		}
	}

	/*//debug
	for (Data::GraphNodeSet::iterator iter = this->partitionTree->getLeavesSet().begin(), end = this->partitionTree->getLeavesSet().end();
		 iter != end; iter++)
	{
		std::cout << "partition for cell " << (*iter)->getId() << " with load " << this->bspLoad[(*iter)->getId()] << std::endl;
	}
	//endebug*/
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::isMin(const Data::BoxTreeCell *c, Real coord, UInt split) const
{
	return (((this->dimension == 2) ? c->mid2D().getCoord(split) : c->mid3D().getCoord(split)) < coord);
}

/*bool PMJMesh::SmartMinNumberBSPTreePartitioner::isMin(const Data::BoxTreeCell *c, const Data::Point &mid, UInt split) const
{
	return this->isMin(c, mid.getCoord(split), split);
}*/

bool PMJMesh::SmartMinNumberBSPTreePartitioner::isEqual(Real newLoad[2], Real oldLoad[2])
{
	if ((newLoad[0] <= 0.0) || (newLoad[1] <= 0.0) || (oldLoad[0] <= 0.0) || (oldLoad[1] <= 0.0))
	{
		return false;
	}

	//return (std::fabs(std::fabs(newLoad[0] - newLoad[1]) - std::fabs(oldLoad[0] - oldLoad[1])) <= Data::getTolerance());

	Real propNew[2], propOld[2];
	this->findProportion(newLoad[0], newLoad[1], propNew[0], propNew[1]);
	this->findProportion(oldLoad[0], oldLoad[1], propOld[0], propOld[1]);

	return (std::fabs(std::fabs(newLoad[0]*propNew[0] - newLoad[1]*propNew[1]) - std::fabs(oldLoad[0]*propOld[0] - oldLoad[1]*propOld[1])) <= Data::getTolerance());
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::isEqual(Real newLoad, Real oldLoad)
{
	//return (std::fabs(oldLoad - newLoad) <= Data::getTolerance());

	Real propNew, propOld;
	this->findProportion(newLoad, oldLoad, propNew, propOld);

	return (std::fabs(propOld*oldLoad - propNew*newLoad) <= Data::getTolerance());
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::isBetter(Real newLoad[2], Real oldLoad[2])
{
	if ((newLoad[0] <= 0.0) || (newLoad[0] <= 0.0))
	{
		return false;
	}

	if ((oldLoad[0] <= 0.0) || (oldLoad[1] <= 0.0))
	{
		return true;
	}

	//return (std::fabs(newLoad[0] - newLoad[1]) < std::fabs(oldLoad[0] - oldLoad[1]));

	Real propNew[2], propOld[2];
	this->findProportion(newLoad[0], newLoad[1], propNew[0], propNew[1]);
	this->findProportion(oldLoad[0], oldLoad[1], propOld[0], propOld[1]);

	return (std::fabs(newLoad[0]*propNew[0] - newLoad[1]*propNew[1]) < std::fabs(oldLoad[0]*propOld[0] - oldLoad[1]*propOld[1]));
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::isBetter(Real newLoad, Real oldLoad)
{
	//return (newLoad < oldLoad);

	Real propNew, propOld;
	this->findProportion(newLoad, oldLoad, propNew, propOld);

	return (newLoad*propNew < oldLoad*propOld);
}

bool PMJMesh::SmartMinNumberBSPTreePartitioner::split(const Data::GraphNodeList &list,
	UInt split, Real coord, Real limits[2], Data::GraphNodeList cells[2]) const
{
	//std::cout << "will split " << list.size() << " box cells in direction " << split <<
	//			 ", comparing with coord " << coord << " in limits " <<
	//			 limits[0] << " and " << limits[1] << std::endl;

	for (Data::GraphNodeList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(*iter);

		//std::cout << "will test box cell " << c->getId() << std::endl;

		Data::Position pos = PMJMesh::getPosition(this->dimension, c);

		if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
		{
			//std::cout << "cell is out or unknown, not added" << std::endl;

			continue;
		}

		if ((c->box()->getMin()->getCoord(split) > limits[0] + Data::getTolerance()) ||
			(c->box()->getMax()->getCoord(split) < limits[1] - Data::getTolerance()))
		{
			//std::cout << "cell is already in limits" << std::endl;

			if (this->isMin(c, coord, split))
			{
				cells[0].push_back(c);

				//std::cout << "cell is added to min list" << std::endl;
			}
			else
			{
				cells[1].push_back(c);

				//std::cout << "cell is added to max list" << std::endl;
			}

			continue;
		}

		//std::cout << "cell is not in limits, will test its " << c->numChildren() << " children" << std::endl;

		///TODO consider when the min refinement is postponed
		if (c->isLeaf())
		{
			cells[0].clear();
			cells[1].clear();

			//std::cout << "cell is leaf, clearing lists" << std::endl;

			break;
		}

		for (UInt j = 0; j < c->numChildren(); j++)
		{
			Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(j));

			//std::cout << "will test child box cell " << child->getId() << std::endl;

			Data::Position pos = PMJMesh::getPosition(this->dimension, child);

			if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
			{
				//std::cout << "child is out or unknown, not added" << std::endl;

				continue;
			}

			if (this->isMin(child, coord, split))
			{
				cells[0].push_back(child);

				//std::cout << "child is added to min list" << std::endl;
			}
			else
			{
				cells[1].push_back(child);

				//std::cout << "child is added to max list" << std::endl;
			}
		}
	}

	//std::cout << "lists have sizes " << cells[0].size() << " and " << cells[1].size() << std::endl;

	//return ((!cells[0].empty()) && (!cells[1].empty()));
	return ((!cells[0].empty()) || (!cells[1].empty()));
}

Real PMJMesh::SmartMinNumberBSPTreePartitioner::findInitialSplitCoord(UInt split,
	Real reference, const Data::GraphNodeList &cells, Real min, Real max) const
{
	if (cells.size() <= 1)
	{
		return reference;
		//return (min + max)*0.5;
	}

	Data::GraphNodeList cellsInMinLevel;
	UInt minLevel = UINT_MAX;

	for (Data::GraphNodeList::const_iterator iter = cells.begin();
		 iter != cells.end(); iter++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		UInt level = c->level();

		if (level < minLevel)
		{
			cellsInMinLevel.clear();

			minLevel = level;
		}

		if (level <= minLevel)
		{
			cellsInMinLevel.push_back(c);
		}
	}

	///it has to be a std::set<>, to keep it sorted!
	Data::RealSet values;

	for (Data::GraphNodeList::const_iterator iter = cellsInMinLevel.begin();
		 iter != cellsInMinLevel.end(); iter++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		Real cmin = c->box()->getMin()->getCoord(split);
		Real cmax = c->box()->getMax()->getCoord(split);

		bool minDifferent = (std::fabs(cmin - min) > Data::getTolerance());
		bool maxDifferent = (std::fabs(cmax - max) > Data::getTolerance());

		if (minDifferent)
		{
			values.insert(cmin);
		}

		if (maxDifferent)
		{
			values.insert(cmax);
		}
	}

	if (values.empty())
	{
		return reference;
		//return (min + max)*0.5;
	}

	UInt adv = static_cast<UInt>(values.size())/2;

	Data::RealSet::iterator iter = values.begin();

	std::advance(iter, adv);

	return (*iter);

	/*bool different = false;
	Real distance = REAL_MAX;
	Real closest = 0.0;

	for (Data::GraphNodeList::const_iterator iter = cells.begin();
		 iter != cells.end(); iter++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		Real coord = c->box()->getMin()->getCoord(split);

		Real dist = std::fabs(coord - reference);

		if (dist < distance)
		{
			distance = dist;
			closest = coord;
		}

		if (distance <= Data::getTolerance())
		{
			break;
		}

		coord = c->box()->getMax()->getCoord(split);

		dist = std::fabs(coord - reference);

		if (dist < distance)
		{
			distance = dist;
			closest = coord;
		}

		if (distance <= Data::getTolerance())
		{
			break;
		}

		if ((!different) &&
			((std::fabs(c->box()->getMin()->getCoord(split) - min) <= Data::getTolerance()) ||
			 (std::fabs(c->box()->getMax()->getCoord(split) - max) <= Data::getTolerance())))
		{
			different = true;
		}
	}

	return (!different) ? reference : closest;*/
}

PMJMesh::SmartMinNumberBSPTreePartitioner::SplitCoord PMJMesh::SmartMinNumberBSPTreePartitioner::findSplitCoord(
	Data::BSPTreeCell *cell, Data::GraphNodeList cells[2])
{
	SplitCoord minsc;
	minsc.split = this->dimension;
	minsc.coord = 0.0;
	minsc.load[0] = minsc.load[1] = 0.0;

#if USE_EXPERIMENTAL_3
	Data::Point2D mid2d;
	Data::Point3D mid3d;

	Data::Point &mid = (this->dimension == 2) ?
        (mid2d = cell->mid2D(), static_cast<Data::Point &>(mid2d)) :
        (mid3d = cell->mid3D(), static_cast<Data::Point &>(mid3d));
#else
	Data::Point mid = cell->mid();
#endif //#if USE_EXPERIMENTAL_3

	Data::GraphNodeList ccells[this->dimension][2];
	//Real loads[this->dimension][2];
	Real maxSpan = 0.0;

	//std::cout << "will find best split direction for bsp cell " << cell->getId() << std::endl;

#if USE_C__11
	BSPBoxHashMap::iterator it = this->bspMap.find(cell);
#else
	BSPBoxMap::iterator it = this->bspMap.find(cell);
#endif //#if USE_C__11

	//std::cout << "bsp cell " << cell->getId() << " is associated with " << (*it).second.size() << " box cells" << std::endl;

	for (UInt i = 0; i < this->dimension; i++)
	{
		//std::cout << "will try to split in direction " << i << std::endl;

		Real limits[2] = {cell->box()->getMin()->getCoord(i), cell->box()->getMax()->getCoord(i)};

		//std::cout << "limits are " << limits[0] << " and " << limits[1] << std::endl;

		Real splitCoord = this->findInitialSplitCoord(i, mid.getCoord(i), (*it).second, cell->box()->getMin()->getCoord(i), cell->box()->getMax()->getCoord(i));

		this->split((*it).second, i, splitCoord, limits, ccells[i]);

		//std::cout << "split in lists with sizes " << ccells[i][0].size() << " and " << ccells[i][1].size() << std::endl;

		SplitCoord sc = this->findSplitCoord(i, splitCoord, limits, ccells[i]);

		//std::cout << "found split coord " << sc.coord << " for direction " << i << std::endl;

		//loads[this->dimension][0] = sc.load[0];
		//loads[this->dimension][1] = sc.load[1];

		if ((sc.load[0] <= 0.0) || (sc.load[1] <= 0.0))
		{
			continue;
		}

		//std::cout << "found loads " << sc.load[0] << " and " << sc.load[1] << std::endl;

		Real span = limits[1] - limits[0];

		if ((this->isBetter(sc.load, minsc.load)) ||
			((this->isEqual(sc.load, minsc.load)) && (span > maxSpan)))
		{
			//std::cout << "changed split coord" << std::endl;

			minsc = sc;
			maxSpan = span;
		}
	}

	if (minsc.split == this->dimension)
	{
		//std::cout << "could not determine a good direction" << std::endl;

		minsc.load[0] = minsc.load[1] = -1.0;

		return minsc;

		/*minsc.split = (cell->isRoot()) ? 0 : (static_cast<PartitionBSPTreeCell *>(cell->parent())->getSplitDirection() + 1)%this->dimension;

		Data::Point min = cell->min();
		Data::Point max = cell->max();
		Data::Point mid = cell->mid();

		minsc.coord = mid.getCoord(minsc.split);

		Data::Box bmin(min, max), bmax(min, max);

		bmin.getMax()->setCoord(minsc.split, minsc.coord);
		bmax.getMin()->setCoord(minsc.split, minsc.coord);

		minsc.load[0] = loads[minsc.split][0];
		minsc.load[1] = loads[minsc.split][1];*/
	}

	cells[0].swap(ccells[minsc.split][0]);
	cells[1].swap(ccells[minsc.split][1]);

	//std::cout << "numbers of cells for min split direction are " << cells[0].size() << " and " << cells[1].size() << std::endl;

	return minsc;
}

PMJMesh::SmartMinNumberBSPTreePartitioner::SplitCoord PMJMesh::SmartMinNumberBSPTreePartitioner::findSplitCoord(UInt split,
	Real coord, Real plimits[2], Data::GraphNodeList cells[2])
{
	Data::GraphNodeList fixed[2];
	Real load[2] = {0.0, 0.0};
	bool skipFixed[2] = {false, false};

	SplitCoord sc = this->findSplitCoord(split, coord, plimits, cells, fixed, load, skipFixed);

	cells[0].swap(fixed[0]);
	cells[1].swap(fixed[1]);

	return sc;
}

PMJMesh::SmartMinNumberBSPTreePartitioner::SplitCoord PMJMesh::SmartMinNumberBSPTreePartitioner::findSplitCoord(UInt split,
	Real coord, Real plimits[2], Data::GraphNodeList cells[2],
	Data::GraphNodeList fixed[2], Real load[2], bool skipFixed[2])
{
	//std::cout << "will find split coord for direction " << split << ", coord " << coord <<
	//			 ", limits of parent " << plimits[0] << " and " << plimits[1] <<
	//			 ", lists with sizes " << cells[0].size() << " and " << cells[1].size() <<
	//			 ", fixeds with sizes " << fixed[0].size() << " and " << fixed[1].size() <<
	//			 ", and loads " << load[0] << " and " << load[1] << std::endl;

	SplitCoord sc;

	sc.split = split;
	sc.coord = coord;

	bool skip[2] = {true, true};

	for (UInt i = 0; i < 2; i++)
	{
		sc.load[i] = load[i];

		//std::cout << "setting sc.load[" << i << "] as " << sc.load[i] << std::endl;

		if (cells[i].empty())
		{
			skip[i] = false;
		}

		for (Data::GraphNodeList::iterator iter = cells[i].begin();
			 iter != cells[i].end(); iter++)
		{
			sc.load[i] += (*this->load->find((*iter)->getId())).second;

			//std::cout << "adding load of cell " << (*iter)->getId() << " to " << ((i == 0) ? "min" : "max") << ": " << sc.load[i] << std::endl;

			if ((this->skippingInner) && (skip[i]) &&
				(PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>((*iter))) != Data::IN))
			{
				skip[i] = false;
			}
		}
	}

	//std::cout << "final loads for splitting are " << sc.load[0] << " and " << sc.load[1] << std::endl;

	//std::cout << "debugload = " << debugload << ", sum = debugload: " << std::boolalpha << ((sc.load[0] + sc.load[1]) == debugload) << std::endl;

	bool sk = ((skip[0] && skipFixed[0]) || (skip[1] && skipFixed[1]));

	if (this->isEqual(sc.load[0], sc.load[1]))
	{
		//std::cout << "loads are equal" << std::endl;

		if ((!this->skippingInner) || (!sk))
		{
			fixed[0].splice(fixed[0].end(), cells[0]);
			fixed[1].splice(fixed[1].end(), cells[1]);

			return sc;
		}
	}

	//std::cout << "loads are not equal" << std::endl;

	Real limits[2], nCoord;
	Data::GraphNodeList nCells[2];

	bool splitMin = this->isBetter(sc.load[1], sc.load[0]);

	//std::cout << "will split " << (splitMin ? "min" : "max") << std::endl;

	UInt i = splitMin ? 0 : 1;

	limits[i] = plimits[i];
	limits[1 - i] = coord;

	//std::cout << "new limits are " << limits[0] << " and " << limits[1] << std::endl;

	//nCoord = (limits[0] + limits[1])*0.5;
	nCoord = this->findInitialSplitCoord(split, (limits[0] + limits[1])*0.5, cells[i], limits[0], limits[1]);

	//std::cout << "new coord is " << nCoord << std::endl;

	bool spl = this->split(cells[i], split, nCoord, limits, nCells);

	if (spl)
	{
		//std::cout << "was able to split" << std::endl;

		load[1 - i] = sc.load[1 - i];

		//std::cout << "new loads for fixeds are " << load[0] << " and " << load[1] << std::endl;

		fixed[1 - i].splice(fixed[1 - i].end(), cells[1 - i]);

		//std::cout << "new fixeds have sizes " << fixed[0].size() << " and " << fixed[1].size() << std::endl;

		Data::GraphNodeList::iterator endi = fixed[i].end();
		Data::GraphNodeList::iterator endj = fixed[1 - i].end();

		bool emptyi = fixed[i].empty();
		bool emptyj = fixed[1 - i].empty();

		if (!emptyi)
		{
			--endi;
		}

		if (!emptyj)
		{
			--endj;
		}

		if (this->skippingInner)
		{
			skipFixed[1 - i] = skip[1 - i] && skipFixed[i - 1];
		}

		SplitCoord adjusted = this->findSplitCoord(split, nCoord, limits, nCells, fixed, load, skipFixed);

		//std::cout << "debugload = " << debugload << ", sum_ch = debugload: " << std::boolalpha << ((adjusted.load[0] + adjusted.load[1]) == debugload) << std::endl;

		bool better = false;

		if ((this->skippingInner) &&
			((sk) || ((adjusted.load[0] <= 0.0) && (adjusted.load[1] <= 0.0))))
		{
			if ((adjusted.load[0] > 0.0) || (adjusted.load[1] > 0.0))
			{
				better = true;
			}
		}
		else
		{
			better = this->isBetter(adjusted.load, sc.load);
		}

		if (better)
		{
			//std::cout << "recursive call had better load distribution" << std::endl;

			sc.coord = adjusted.coord;
			sc.load[0] = adjusted.load[0];
			sc.load[1] = adjusted.load[1];
		}
		else
		{
			//std::cout << "recursive call did not have better load distribution" << std::endl;

			if (emptyj)
			{
				fixed[1 - i].clear();
			}
			else
			{
				fixed[1 - i].erase(++endj, fixed[1 - i].end());
			}

			if (emptyi)
			{
				fixed[i].clear();
			}
			else
			{
				fixed[i].erase(++endi, fixed[i].end());
			}

			fixed[i].splice(fixed[i].end(), cells[i]);

			if ((this->skippingInner) && (sk))
			{
				sc.load[0] = sc.load[1] = -1.0;
			}
		}
	}
	else
	{
		//std::cout << "was not able to split" << std::endl;

		fixed[0].splice(fixed[0].end(), cells[0]);
		fixed[1].splice(fixed[1].end(), cells[1]);

		if ((this->skippingInner) && (sk))
		{
			sc.load[0] = sc.load[1] = -1.0;
		}
	}

	//std::cout << "return cells of sizes " << cells[0].size() << " and " << cells[1].size() <<
	//			 ", and fixeds " << fixed[0].size() << " and " << fixed[1].size() <<
	//			 ", and loads " << sc.load[0] << " and " << sc.load[1] << std::endl;

	return sc;
}

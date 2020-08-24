#include "PMJMesh/LoadCalculator.h"

#include "Data/BoxTreeCell.h"
#include "Data/BoxTree.h"
#include "Data/Box.h"

using namespace PMJMesh;

PMJMesh::LoadCalculator::LoadCalculator(const Data::BoxTree *tree,
	bool considerOn, bool heightDependent, UInt dimension, UInt minLevel)
{
	this->setTree(tree);

	this->setConsiderOn(considerOn);

	this->setMinLevel(minLevel);
	this->setDimension(dimension);

	this->box = NULL;

	this->load = 0.0;

	//this->propagateLoad = false;
	//this->propagateLoad = true;

	//this->addingLoadForNodes = false;

	this->setHeightDependent(heightDependent);
	this->heightDependentMinLevel = 0;
	this->heightDependentMaxLevel = 0;
	this->heightDependentFactor = 1.0;

	/*if (this->heightDependent)
	{
		this->addingLoadForNodes = false;
	}

	if (this->addingLoadForNodes)
	{
		this->setConsiderOn(true);
	}*/
}

PMJMesh::LoadCalculator::~LoadCalculator()
{
	//this->tree = NULL;
	//this->position = NULL;
	//this->box = NULL;

	//this->treeLoad.clear();
}

/*bool PMJMesh::LoadCalculator::isConsideringOn() const
{
	return this->considerOn;
}

bool PMJMesh::LoadCalculator::isAddingLoadForNodes() const
{
	return this->addingLoadForNodes;
}*/

/*Real PMJMesh::LoadCalculator::weight(Real load) const
{
	//use with propagateload = false or propagateload = true. they are both
	//  the same in this case, so use false for less calculations
	return load;

	//use with propagateload = true
	//return (load <= 1.0) ? load : load + 1.0;

	//use with propagateload = true
	//return (load <= 1.0) ? load : load*std::log10(load)/std::log10(std::pow(this->dimension, 2.0));

	//use with propagateload = true
	//return (load <= 1.0) ? load : (load + std::log10(load)/std::log10(std::pow(this->dimension, 2.0)));
}*/

Real PMJMesh::LoadCalculator::loadFor(const Data::BoxTreeCell *cell, Real basicLoad) const
{
	if (this->heightDependent)
	{
		if (basicLoad <= 0.0)
		{
			return basicLoad;
		}

		UInt level = cell->level();

		if ((this->minLevel != 0) && (level < this->minLevel))
		{
			//Real loadChild = basicLoad*static_cast<Real>(this->minLevel)*this->heightDependentFactor;
			//Real loadChild = basicLoad*static_cast<Real>(this->minLevel - this->heightDependentMinLevel)*this->heightDependentFactor;
			//Real loadChild = basicLoad*std::pow(this->heightDependentFactor, static_cast<Real>(this->minLevel - this->heightDependentMinLevel));
			//Real loadChild = basicLoad*(static_cast<Real>(this->minLevel - this->heightDependentMinLevel)*this->heightDependentFactor + 1.0);
			Real loadChild = basicLoad*(static_cast<Real>(this->minLevel - this->heightDependentMinLevel)*this->heightDependentFactor + 1.0);
			//Real loadChild = basicLoad*(static_cast<Real>(this->minLevel)*this->heightDependentFactor + 1.0);

			UInt diff = this->minLevel - level;

			Real numChildren = std::pow(2.0, this->dimension*static_cast<Real>(diff));

			return numChildren*loadChild;
		}

		//return basicLoad*static_cast<Real>(level)*this->heightDependentFactor;
		//return basicLoad*static_cast<Real>(level - this->heightDependentMinLevel)*this->heightDependentFactor;
		//return basicLoad*std::pow(this->heightDependentFactor, static_cast<Real>(level - this->heightDependentMinLevel));
		//return basicLoad*(static_cast<Real>(level - this->heightDependentMinLevel)*this->heightDependentFactor + 1.0);
		return basicLoad*(static_cast<Real>(level - this->heightDependentMinLevel)*this->heightDependentFactor + 1.0);
		//return basicLoad*(static_cast<Real>(level)*this->heightDependentFactor + 1.0);
	}

	/*if (this->addingLoadForNodes)
	{
		if ((this->minLevel != 0) && (basicLoad > 0.0))
		{
			UInt level = cell->level();

			if (level < this->minLevel)
			{
				UInt diff = this->minLevel - level;

				Real load = 0.0;

				for (UInt i = 0; i <= diff; i++)
				{
					load += std::pow(2.0, this->dimension*static_cast<Real>(i))*basicLoad;
				}

				return load;
			}
		}

		return basicLoad;
	}*/

	//considering minLevel
	if ((this->minLevel != 0) && (basicLoad > 0.0))
	{
		UInt level = cell->level();

		if (level < this->minLevel)
		{
			UInt diff = this->minLevel - level;

			/*if (this->propagateLoad)
			{
				Real numChildren = std::pow(2.0, this->dimension);

				for (UInt i = diff; i > 0; i--)
				{
					basicLoad = numChildren*this->weight(basicLoad);
				}
			}
			else*/
			{
				basicLoad *= std::pow(2.0, this->dimension*static_cast<Real>(diff));
			}
		}
	}

	return basicLoad;
}

Real PMJMesh::LoadCalculator::loadFor(enum Data::Position pos) const
{
	switch (pos)
	{
	case Data::OUT:
		return 0.0;

	case Data::IN:
		return 1.0;

	case Data::ON:
		return (this->considerOn) ? 1.0 : 0.0;

	default:
		break;
	}

	return 0.0;
}

Real PMJMesh::LoadCalculator::setLoad(const Data::BoxTreeCell *cell, Real load)
{
	if (cell->isLeaf())
	{
		load = this->loadFor(cell, load);

		//Real w = this->weight(load);

		//this->treeLoad[cell->getId()] = w;
		this->treeLoad[cell->getId()] = load;

		return /*(this->propagateLoad) ? w : */load;
	}

	Real sumLoad = 0.0;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(cell->child(i));

		sumLoad += this->setLoad(c, load);
	}

	/*if (this->addingLoadForNodes)
	{
		sumLoad += load;
	}*/

	//Real w = this->weight(sumLoad);

	//this->treeLoad[cell->getId()] = w;
	this->treeLoad[cell->getId()] = sumLoad;

	return /*(this->propagateLoad) ? w : */sumLoad;
}

Real PMJMesh::LoadCalculator::calculateTreeLoad(const Data::BoxTreeCell *cell)
{
	enum Data::Position pos = PMJMesh::getPosition(this->dimension, cell);

	if (pos == Data::OUT)
	{
		return this->setLoad(cell, this->loadFor(pos));
	}

	if (pos == Data::IN)
	{
		return this->setLoad(cell, this->loadFor(pos));
	}

	if (cell->isLeaf())
	{
		Real load = this->loadFor(pos);

		//Real w = this->weight(load);

		//this->treeLoad[cell->getId()] = w;
		this->treeLoad[cell->getId()] = load;

		return /*(this->propagateLoad) ? w : */load;
	}

	Real load = 0.0;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		load += this->calculateTreeLoad(static_cast<Data::BoxTreeCell *>(cell->child(i)));
	}

	/*if (this->addingLoadForNodes)
	{
		load += this->loadFor(pos);
	}*/

	//Real w = this->weight(load);

	//this->treeLoad[cell->getId()] = w;
	this->treeLoad[cell->getId()] = load;

	return /*(this->propagateLoad) ? w : */load;
}

void PMJMesh::LoadCalculator::makeTreeLoad()
{
	if (this->heightDependent)
	{
		this->heightDependentMaxLevel = this->heightDependentMinLevel = 0;
		this->heightDependentFactor = 1.0;

		Data::GraphNodeList cells;

		cells.push_back(this->tree->getRoot());

		while (!cells.empty())
		{
			Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(cells.front());

			cells.pop_front();

			Data::Position pos = PMJMesh::getPosition(this->dimension, n);

			if ((pos == Data::OUT) ||
				((!this->considerOn) && (pos == Data::ON) && (n->isLeaf())))
			{
				continue;
			}

			if (n->isLeaf())
			{
				UInt l = n->level();

				if (this->heightDependentMinLevel == 0)
				{
					this->heightDependentMinLevel = l;
					this->heightDependentMaxLevel = l;
				}
				else if (l > this->heightDependentMaxLevel)
				{
					this->heightDependentMaxLevel = l;
				}

				continue;
			}

			for (UInt i = 0; i < n->numChildren(); i++)
			{
				cells.push_back(n->child(i));
			}
		}

		if (this->heightDependentMinLevel < this->minLevel)
		{
			this->heightDependentMinLevel = this->minLevel;
		}

		if ((this->heightDependentMaxLevel == 0) ||
			(this->heightDependentMaxLevel == this->heightDependentMinLevel))
		{
			this->heightDependent = false;
		}
		else
		{
			//this->heightDependentFactor = 1.0/static_cast<Real>(this->heightDependentMaxLevel);

			//this->heightDependentMinLevel -= 1;
			//this->heightDependentFactor = 1.0/static_cast<Real>(this->heightDependentMaxLevel - this->heightDependentMinLevel);

			//this->heightDependentFactor = 1.05;

			//this->heightDependentFactor = 1.0/static_cast<Real>(this->heightDependentMaxLevel - this->heightDependentMinLevel + 1);

			this->heightDependentFactor = 1.0/static_cast<Real>(this->heightDependentMaxLevel - this->heightDependentMinLevel);

			//this->heightDependentFactor = 1.0/static_cast<Real>(this->heightDependentMaxLevel);
		}
	}

	this->calculateTreeLoad(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));

	/*std::cout << "LoadCalculator::makeTreeLoad" << std::endl;

	for (Data::RealMap::iterator iter = this->treeLoad.begin();
		 iter != this->treeLoad.end(); iter++)
	{
		std::cout << (*iter).first << " = " << static_cast<int>((*iter).second) << std::endl;

		if (iter != this->treeLoad.end())
		{
			ULInt id = (*iter).first;

			iter++;

			ULInt nid = (*iter).first;

			iter--;

			for (UInt i = id + 1; i < nid; i++)
			{
				std::cout << std::endl;
			}
		}
	}*/
}

bool PMJMesh::LoadCalculator::inBox(const Data::BoxTreeCell *cell)
{
	//return this->box->in(cell->box());
	return this->box->optIn(cell->box());
}

Real PMJMesh::LoadCalculator::calculateBoxLoad(const Data::BoxTreeCell *cell)
{
	/*std::cout << "calculating box load for cell " << cell->getId() << std::endl;

	//if (!this->box->intercept(cell->box()))
	if (!this->box->optIntercept(cell->box()))
	{
		std::cout << "cell " << cell->getId() << " does not intercept box" << std::endl;
		std::cout << "returning " << 0.0 << std::endl;

		return 0.0;
	}

	std::cout << "cell " << cell->getId() << " intercepts box" << std::endl;

	if (this->inBox(cell))
	{
		std::cout << "cell " << cell->getId() << " is completely inside box" << std::endl;

		if (this->treeLoad.find(cell->getId()) == this->treeLoad.end())
		{
			std::cout << "cell " << cell->getId() << " has no load" << std::endl;
		}

		std::cout << "returning " << this->treeLoad[cell->getId()] << std::endl;

		return this->treeLoad[cell->getId()];
	}

	std::cout << "cell " << cell->getId() << " is not completely inside box" << std::endl;

	if (cell->isLeaf())
	{
		std::cout << "cell " << cell->getId() << " is leaf" << std::endl;

		std::cout << "returning " << 0.0 << std::endl;

		//these are the leaf cells that are not in box though they do intercept it

		return 0.0;
	}

	std::cout << "cell " << cell->getId() << " is not leaf" << std::endl;

	Real sum = 0.0;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		sum += this->calculateBoxLoad(static_cast<Data::BoxTreeCell *>(cell->child(i)));
	}

	std::cout << "returning sum of children loads " << sum << std::endl;

	return sum;*/

	//std::cout << "calculating box load for cell " << cell->getId() << std::endl;

	//if (this->box->in(cell->box()))
	if (this->box->optIn(cell->box()))
	{
		//std::cout << "cell " << cell->getId() << " is in box" << std::endl;
		//std::cout << "returning " << this->treeLoad[cell->getId()] << std::endl;

		return this->treeLoad[cell->getId()];
	}

	//std::cout << "cell " << cell->getId() << " is not in box" << std::endl;

	if (cell->outOf(this->box))
	{
		//std::cout << "cell " << cell->getId() << " is out of box" << std::endl;
		//std::cout << "returning " << 0.0 << std::endl;

		return 0.0;
	}

	//std::cout << "cell " << cell->getId() << " is not out of box" << std::endl;

	/*//if (this->box->out(cell->box()))
	if (this->box->optOut(cell->box()))
	{
		//std::cout << "cell " << cell->getId() << " is out of box" << std::endl;
		//std::cout << "returning " << 0.0 << std::endl;

		return 0.0;
	}

	//std::cout << "cell " << cell->getId() << " is not out of box" << std::endl;

	//if (!this->box->intercept(cell->box()))
	if (!this->box->optIntercept(cell->box()))
	{
		//std::cout << "cell " << cell->getId() << " does not intercept box box" << std::endl;
		//std::cout << "returning " << 0.0 << std::endl;

		return 0.0;
	}

	//std::cout << "cell " << cell->getId() << " intercepts box" << std::endl;

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (((cell->box()->getMin()->getCoord(i) <  this->box->getMin()->getCoord(i)) &&
			 (cell->box()->getMax()->getCoord(i) <= this->box->getMin()->getCoord(i))) ||
			((cell->box()->getMax()->getCoord(i) >  this->box->getMax()->getCoord(i)) &&
			 (cell->box()->getMin()->getCoord(i) >= this->box->getMax()->getCoord(i))))
		{
			//std::cout << "cell " << cell->getId() << " is out of box(2) " << i << std::endl;
			//std::cout << "returning " << 0.0 << std::endl;

			return 0.0;
		}
	}*/

	//std::cout << "cell " << cell->getId() << " is not out of box(2)" << std::endl;

	if (cell->isLeaf())
	{
		//std::cout << "cell " << cell->getId() << " is leaf" << std::endl;

		if (this->minLevel == 0)
		{
			//std::cout << "min level is 0" << std::endl;
			//std::cout << "returning " << 0.0 << std::endl;

			return 0.0;
		}

		//std::cout << "min level is not 0" << std::endl;

		Real load = this->treeLoad[cell->getId()];

		//std::cout << "load of cell " << cell->getId() << " is " << load << std::endl;

		if (load <= Data::getTolerance())
		{
			//std::cout << "load of cell " << cell->getId() << " is smaller than tolerance" << std::endl;
			//std::cout << "returning " << 0.0 << std::endl;

			return 0.0;
		}

		//std::cout << "load of cell " << cell->getId() << " is greater than tolerance" << std::endl;

		Real size = 1.0; //area or volume of the region of the cell inside the box

		for (UInt i = 0; i < this->dimension; i++)
		{
			size *= std::min(cell->box()->getMax()->getCoord(i), this->box->getMax()->getCoord(i)) -
					std::max(cell->box()->getMin()->getCoord(i), this->box->getMin()->getCoord(i));
		}

		//std::cout << "area/volume is " << size << std::endl;
		//std::cout << "area/volume of cell is " << std::pow(cell->box()->size(), this->dimension) << std::endl;
		//std::cout << "returning " << load*size/std::pow(cell->box()->size(), this->dimension) << std::endl;

		return load*size/std::pow(cell->box()->size(), this->dimension);
	}

	//std::cout << "cell " << cell->getId() << " is not leaf" << std::endl;

	Real sum = 0.0;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		sum += this->calculateBoxLoad(static_cast<Data::BoxTreeCell *>(cell->child(i)));
	}

	//std::cout << "cell " << cell->getId() << " has called recursively for children" << std::endl;
	//std::cout << "returning " << sum << std::endl;

	return sum;
}

void PMJMesh::LoadCalculator::makeBoxLoad()
{
	/*static int debug = 0;

	debug++;

	if (debug == 21)
	{
		std::cout << "found" << std::endl;
	}*/

	this->load = this->calculateBoxLoad(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));

	//std::cout << "load = " << this->load << std::endl;
}

void PMJMesh::LoadCalculator::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

void PMJMesh::LoadCalculator::setMinLevel(UInt minLevel)
{
	this->minLevel = minLevel;
}

void PMJMesh::LoadCalculator::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::LoadCalculator::setConsiderOn(bool considerOn)
{
	this->considerOn = considerOn;
}

bool PMJMesh::LoadCalculator::isConsideringOn() const
{
    return this->considerOn;
}

void PMJMesh::LoadCalculator::setHeightDependent(bool heightDependent)
{
	this->heightDependent = heightDependent;
}

void PMJMesh::LoadCalculator::setBox(const Data::Box *box)
{
	this->box = box;
}

Real PMJMesh::LoadCalculator::getLoad() const
{
	return this->load;
}

#if USE_C__11
const Data::RealHashMap &PMJMesh::LoadCalculator::getTreeLoad() const
#else
const Data::RealMap &PMJMesh::LoadCalculator::getTreeLoad() const
#endif //#if USE_C__11
{
	return this->treeLoad;
}

void PMJMesh::LoadCalculator::execute()
{
	if (!this->tree)
	{
		return;
	}

	if (this->treeLoad.empty())
	{
		this->makeTreeLoad();
	}

	if (this->box)
	{
		this->makeBoxLoad();
	}
}

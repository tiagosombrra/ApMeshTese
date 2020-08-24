#include "Performer/NeighborTreeRefiner.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

using namespace Performer;

Performer::NeighborTreeRefiner::NeighborTreeRefiner(Data::BoxTree *tree, UInt difference) :
	TreeRefiner(tree)
{
	this->setDifference(difference);

	//this->refined = false;

	this->cells = NULL;
}

Performer::NeighborTreeRefiner::~NeighborTreeRefiner()
{
	if (this->cells)
	{
		delete this->cells;
	}
}

void Performer::NeighborTreeRefiner::subdivide(Data::TreeNode *n)
{
	this->subdivide(static_cast<Data::BoxTreeCell *>(n));
}

void Performer::NeighborTreeRefiner::subdivide(Data::BoxTreeCell *c)
{
	c->subdivide();
}

void Performer::NeighborTreeRefiner::setDifference(UInt difference)
{
	this->difference = difference;
}

UInt Performer::NeighborTreeRefiner::getDifference() const
{
	return this->difference;
}

Data::GraphNodeList Performer::NeighborTreeRefiner::findNeighbors(const Data::BoxTreeCell *c) const
{
	return static_cast<Data::BoxTree *>(this->tree)->neighbors(c);
}

void Performer::NeighborTreeRefiner::refine(Data::TreeNode *n)
{
	this->refine(static_cast<Data::BoxTreeCell *>(n));
}

void Performer::NeighborTreeRefiner::refine(Data::BoxTreeCell *c)
{
	if (!c->isLeaf())
	{
		this->refineChildren(c);

		return;
	}

	Data::GraphNodeList neighbors = this->findNeighbors(c);

	/*
	while (!neighbors.empty())
	{
		BoxTreeCell *n = (BoxTreeCell *)neighbors.back();
		neighbors.pop_back();

		//if ((Int)n->level() - (Int)c->level() > (Int)difference)
		if (n->level() > difference + c->level())
		{
			this->subdivide(c);

			this->refined = true;

			this->refine(c);

			break;
		}
	}
	//*/

	///*
	for (Data::GraphNodeList::iterator iter = neighbors.begin();
		 iter != neighbors.end(); iter++)
	{
		Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter));

		//if ((Int)n->level() - (Int)c->level() > (Int)difference)
		if (n->level() > this->difference + c->level())
		{
			this->subdivide(c);

			while (!neighbors.empty())
			{
				n = static_cast<Data::BoxTreeCell *>(neighbors.back());

				neighbors.pop_back();

				this->refine(n);
			}

			//this->refine(c);
			this->refineChildren(c);

			break;
		}
	}
	//*/
}

void Performer::NeighborTreeRefiner::refineChildren(Data::BoxTreeCell *c)
{
	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->refine(static_cast<Data::BoxTreeCell *>(c->child(i)));
	}
}

void Performer::NeighborTreeRefiner::execute()
{
	if (!this->tree)
	{
		return;
	}

	this->executeWithSet();

	//do
	//{
		//this->refined = false;

		//this->refine(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
	//}
	//while (this->refined);
}

bool Performer::NeighborTreeRefiner::smaller(Data::BoxTreeCell *const &c1, Data::BoxTreeCell *const &c2)
{
	return c1->level() > c2->level();
}

Performer::NeighborTreeRefiner::BoxTreeCellMultiSet *Performer::NeighborTreeRefiner::makeSet() const
{
	return new BoxTreeCellMultiSet(&Performer::NeighborTreeRefiner::smaller);
}

void Performer::NeighborTreeRefiner::executeWithSet()
{
	this->cells = this->makeSet();

	this->fillSet();

	this->refineSet();

	delete this->cells;

	this->cells = NULL;
}

void Performer::NeighborTreeRefiner::fillSet()
{
	this->fillSet(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
}

void Performer::NeighborTreeRefiner::fillSet(Data::BoxTreeCell *cell)
{
	if (cell->isLeaf())
	{
        this->map[cell] = this->cells->insert(cell);

		return;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->fillSet(static_cast<Data::BoxTreeCell *>(cell->child(i)));
	}
}

void Performer::NeighborTreeRefiner::refineSet()
{
	while (!this->cells->empty())
	{
		Data::BoxTreeCell *c = *this->cells->begin();

		this->cells->erase(this->cells->begin());

		for (UInt side = 0; side < c->numSides(); side++)
		{
			while (true)
			{
				Data::GraphNodeList neighbors;

				c->neighbors(side, neighbors);

				if (neighbors.size() != 1)
				{
					break;
				}

				Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors.front());

				if (n->level() + this->difference >= c->level())
				{
					break;
				}

				BoxTreeCellMap::iterator it = this->map.find(n);

				if (it == this->map.end())
				{
					break;
				}

				this->subdivide(n);

				this->cells->erase((*it).second);

				this->map.erase(it);

				this->fillSet(n);
			}
		}
	}
}

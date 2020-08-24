#include "Data/BSPTreeCell.h"

#include "Data/BSPTree.h"
#include "Data/Box.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

#if USE_EXPERIMENTAL
Data::BSPTreeCell::BSPTreeCell(ULInt id) :
	Data::BoxTreeCell::BoxTreeCell(id)
{
	this->setSplit(0, 0.0);

#if USE_EXPERIMENTAL_5
    this->adjacents[0] = this->adjacents[1] = this->adjacents[2] = NULL;
#endif //#if USE_EXPERIMENTAL_5
}

Data::BSPTreeCell::BSPTreeCell(BSPTree *tree, ULInt id) :
	Data::BoxTreeCell::BoxTreeCell(tree, id)
{
	this->setSplit(0, 0.0);

#if USE_EXPERIMENTAL_5
    this->adjacents[0] = this->adjacents[1] = this->adjacents[2] = NULL;
#endif //#if USE_EXPERIMENTAL_5
}
#endif //#if USE_EXPERIMENTAL

Data::BSPTreeCell::BSPTreeCell(BSPTreeCell *parent, ULInt id) :
	Data::BoxTreeCell::BoxTreeCell(parent, id)
{
	this->setSplit(0, 0.0);

#if USE_EXPERIMENTAL_5
    this->adjacents[0] = parent;

    this->adjacents[1] = this->adjacents[2] = NULL;

    if (parent)
	{
		parent->add(this);
	}
#endif //#if USE_EXPERIMENTAL_5
}

Data::BSPTreeCell::~BSPTreeCell()
{
#if USE_EXPERIMENTAL_5
	if (!this->isLeaf())
	{
		this->setTree(NULL);
	}

	for (UInt i = 1; i < 3; i++)
	{
		if (this->adjacents[i])
		{
			delete this->adjacents[i];
		}
	}
#endif //#if USE_EXPERIMENTAL_5
}

#if USE_ARRAY
BSPTreeCell &Data::BSPTreeCell::operator=(const BSPTreeCell &c)
{
	if (this != &c)
	{
		RecursiveDecompositionTreeCell::operator=(c);

#if USE_EXPERIMENTAL_5
        for (UInt i = 0; i < 3; i++)
        {
            this->adjacents[i] = c.adjacents[i];
        }
#endif //#if USE_EXPERIMENTAL_5

		this->direction = c.direction;
		this->coord = c.coord;
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_EXPERIMENTAL_5
UInt Data::BSPTreeCell::numAdjacents() const
{
    UInt count = 1;

    if (this->adjacents[1])
    {
        count++;
    }

    if (this->adjacents[2])
    {
        count++;
    }

    return count;
}

void Data::BSPTreeCell::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::BSPTreeCell::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::BSPTreeCell::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 3);
}

void Data::BSPTreeCell::add(GraphNode *n)
{
    if (!this->adjacents[1])
    {
        this->adjacents[1] = n;

        return;
    }

    if (!this->adjacents[2])
    {
        this->adjacents[2] = n;
    }
}

void Data::BSPTreeCell::remove(GraphNode *n)
{
    for (UInt i = 1; i < 3; i++)
    {
        if (this->adjacents[i] == n)
        {
            this->adjacents[i] = NULL;

            break;
        }
    }
}
#endif //#if USE_EXPERIMENTAL_5

void Data::BSPTreeCell::setSplit(UInt direction, Real coord)
{
	this->direction = static_cast<UShort>(direction);
	this->coord = coord;
}

UInt Data::BSPTreeCell::getSplitDirection() const
{
	return static_cast<UInt>(this->direction);
}

Real Data::BSPTreeCell::getSplitCoord() const
{
	return this->coord;
}

///returns true if a cell with given id would be descendant of this cell,
///  false otherwise (including if id == this->id)
bool Data::BSPTreeCell::isDescendant(ULInt id) const
{
	if (id <= this->getId())
	{
		return false;
	}

	if (this->isRoot())
	{
		return true;
	}

	UInt twoDim = 2*this->dimension();
	//UInt twoDir = 2*this->getSplitDirection();

	while (id > this->getId())
	{
		//id = (id - 1 - twoDir)/twoDim;
		id = (id - 1)/twoDim;
	}

	return (id == this->getId());
}

#if USE_EXPERIMENTAL_3
Point2D Data::BSPTreeCell::min2D() const
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMin();
	}

	if (this->isRoot())
	{
		return *static_cast<BSPTree *>(this->getTree())->getShape()->getPoint(0);
	}

	UInt position = this->position();

	if (position == 0)
	{
		return static_cast<BoxTreeCell *>(this->parent())->min2D();
	}

	if (position == 1)
	{
		Point2D pmin = static_cast<BSPTreeCell *>(this->parent())->min2D();

		pmin.setCoord(static_cast<BSPTreeCell *>(this->parent())->getSplitDirection(), static_cast<BSPTreeCell *>(this->parent())->getSplitCoord());

		return pmin;
	}

	return Point2D();
}

Point2D Data::BSPTreeCell::max2D() const
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMax();
	}

	if (this->isRoot())
	{
		return *static_cast<BSPTree *>(this->getTree())->getShape()->getPoint(1);
	}

	UInt position = this->position();

	if (position == 0)
	{
		Point2D pmax = static_cast<BSPTreeCell *>(this->parent())->max2D();

		pmax.setCoord(static_cast<BSPTreeCell *>(this->parent())->getSplitDirection(), static_cast<BSPTreeCell *>(this->parent())->getSplitCoord());

		return pmax;
	}

	if (position == 1)
	{
		return static_cast<BSPTreeCell *>(this->parent())->max2D();
	}

	return Point2D();
}

Point3D Data::BSPTreeCell::min3D() const
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMin();
	}

	if (this->isRoot())
	{
		return *static_cast<BSPTree *>(this->getTree())->getShape()->getPoint(0);
	}

	UInt position = this->position();

	if (position == 0)
	{
		return static_cast<BoxTreeCell *>(this->parent())->min3D();
	}

	if (position == 1)
	{
		Point3D pmin = static_cast<BSPTreeCell *>(this->parent())->min3D();

		pmin.setCoord(static_cast<BSPTreeCell *>(this->parent())->getSplitDirection(), static_cast<BSPTreeCell *>(this->parent())->getSplitCoord());

		return pmin;
	}

	return Point3D();
}

Point3D Data::BSPTreeCell::max3D() const
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMax();
	}

	if (this->isRoot())
	{
		return *static_cast<BSPTree *>(this->getTree())->getShape()->getPoint(1);
	}

	UInt position = this->position();

	if (position == 0)
	{
		Point3D pmax = static_cast<BSPTreeCell *>(this->parent())->max3D();

		pmax.setCoord(static_cast<BSPTreeCell *>(this->parent())->getSplitDirection(), static_cast<BSPTreeCell *>(this->parent())->getSplitCoord());

		return pmax;
	}

	if (position == 1)
	{
		return static_cast<BSPTreeCell *>(this->parent())->max3D();
	}

	return Point3D();
}
#else
Point Data::BSPTreeCell::min() const
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMin();
	}

	if (this->isRoot())
	{
		return *static_cast<BSPTree *>(this->getTree())->getShape()->getPoint(0);
	}

	UInt position = this->position();

	if (position == 0)
	{
		return static_cast<BoxTreeCell *>(this->parent())->min();
	}

	if (position == 1)
	{
		Point pmin = static_cast<BSPTreeCell *>(this->parent())->min();

		pmin.setCoord(static_cast<BSPTreeCell *>(this->parent())->getSplitDirection(), static_cast<BSPTreeCell *>(this->parent())->getSplitCoord());

		return pmin;
	}

	return Point();
}

Point Data::BSPTreeCell::max() const
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMax();
	}

	if (this->isRoot())
	{
		return *static_cast<BSPTree *>(this->getTree())->getShape()->getPoint(1);
	}

	UInt position = this->position();

	if (position == 0)
	{
		Point pmax = static_cast<BSPTreeCell *>(this->parent())->max();

		pmax.setCoord(static_cast<BSPTreeCell *>(this->parent())->getSplitDirection(), static_cast<BSPTreeCell *>(this->parent())->getSplitCoord());

		return pmax;
	}

	if (position == 1)
	{
		return static_cast<BSPTreeCell *>(this->parent())->max();
	}

	return Point();
}
#endif //#if USE_EXPERIMENTAL_3

UInt Data::BSPTreeCell::position() const
{
	return this->parent() ? static_cast<UInt>((this->getId() + 1)%2) : 0;
}

Real Data::BSPTreeCell::size() const
{
	return this->box()->size();
}

void Data::BSPTreeCell::subdivide()
{
	ULInt id = 2*this->dimension()*this->getId() + 2*this->getSplitDirection();

	new BSPTreeCell(this, ++id);
	new BSPTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

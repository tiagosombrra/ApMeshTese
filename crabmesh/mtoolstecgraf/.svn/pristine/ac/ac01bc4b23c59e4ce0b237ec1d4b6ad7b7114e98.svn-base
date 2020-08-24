#include "Data/BinTreeCell.h"

#include "Data/BinTree.h"
#include "Data/Box.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

Data::BinTreeCell::BinTreeCell(BinTreeCell *parent, ULInt id) :
	BoxTreeCell(parent, id)
{
#if USE_EXPERIMENTAL_5
    this->adjacents[0] = parent;

    this->adjacents[1] = this->adjacents[2] = NULL;

    if (parent)
    {
        parent->add(this);
    }
#endif //#if USE_EXPERIMENTAL_5
}

Data::BinTreeCell::~BinTreeCell()
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

#if USE_EXPERIMENTAL_5
UInt Data::BinTreeCell::numAdjacents() const
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

void Data::BinTreeCell::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::BinTreeCell::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::BinTreeCell::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 3);
}

void Data::BinTreeCell::add(GraphNode *n)
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

void Data::BinTreeCell::remove(GraphNode *n)
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

UInt Data::BinTreeCell::dimension() const
{
	return 1;
}

Real Data::BinTreeCell::size() const
{
	//even though the level is 0 for the root, let us avoid a power and a division
	return this->isRoot() ? static_cast<BinTree *>(this->getTree())->size() :
		static_cast<BinTree *>(this->getTree())->size()/std::pow(2.0, static_cast<Real>(this->level()));
}

UInt Data::BinTreeCell::numSides() const
{
	return 2;
}

bool Data::BinTreeCell::in(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->in(p);
}

bool Data::BinTreeCell::optIn(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->optIn(p);
}

bool Data::BinTreeCell::on(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->on(p);
}

bool Data::BinTreeCell::optOn(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->optOn(p);
}

bool Data::BinTreeCell::out(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->out(p);
}

bool Data::BinTreeCell::optOut(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->optOut(p);
}

bool Data::BinTreeCell::isChild(UInt i) const
{
	return (this->position() == i);
}

UInt Data::BinTreeCell::position() const
{
	return static_cast<UInt>((this->getId() - 1)%2);
}

#if USE_EXPERIMENTAL_3
Point2D Data::BinTreeCell::mid2D() const
#else
Point Data::BinTreeCell::mid() const
#endif //#if USE_EXPERIMENTAL_3
{
#if USE_EXPERIMENTAL_3
	Point2D mid = this->min2D();
#else
	Point mid = this->min();
#endif //#if USE_EXPERIMENTAL_3

	mid.setCoord(0, mid.getCoord(0) + this->size()*0.5);

	return mid;
}

#if USE_EXPERIMENTAL_3
Point3D Data::BinTreeCell::mid3D() const
{
    return Point3D();
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
Point2D Data::BinTreeCell::min2D() const
#else
Point Data::BinTreeCell::min() const
#endif //#if USE_EXPERIMENTAL_3
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMin();
	}

	if (this->isRoot())
	{
		return *static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(0);
	}

	switch (this->position())
	{
	case 0:
		//if I am the left child
#if USE_EXPERIMENTAL_3
		return static_cast<BoxTreeCell *>(this->parent())->min2D();
#else
		return static_cast<BoxTreeCell *>(this->parent())->min();
#endif //#if USE_EXPERIMENTAL_3
		break;

	case 1:
		//if I am the right child
#if USE_EXPERIMENTAL_3
		return static_cast<BoxTreeCell *>(this->parent())->mid2D();
#else
		return static_cast<BoxTreeCell *>(this->parent())->mid();
#endif //#if USE_EXPERIMENTAL_3
		break;
	}

#if USE_EXPERIMENTAL_3
	return Point2D();
#else
	return Point();
#endif //#if USE_EXPERIMENTAL_3
}
#if USE_EXPERIMENTAL_3
Point3D Data::BinTreeCell::min3D() const
{
    return Point3D();
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
Point2D Data::BinTreeCell::max2D() const
#else
Point Data::BinTreeCell::max() const
#endif //#if USE_EXPERIMENTAL_3
{
	if (this->shp)
	{
		return *static_cast<Box *>(this->shp)->getMax();
	}

	if (this->isRoot())
	{
		return *static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(1);
	}

	switch (this->position())
	{
	case 0:
		//if I am the left child
#if USE_EXPERIMENTAL_3
		return static_cast<BoxTreeCell *>(this->parent())->mid2D();
#else
		return static_cast<BoxTreeCell *>(this->parent())->mid();
#endif //#if USE_EXPERIMENTAL_3
		break;

	case 1:
		//if I am the right child
#if USE_EXPERIMENTAL_3
		return static_cast<BoxTreeCell *>(this->parent())->max2D();
#else
		return static_cast<BoxTreeCell *>(this->parent())->max();
#endif //#if USE_EXPERIMENTAL_3
		break;
	}

#if USE_EXPERIMENTAL_3
	return Point2D();
#else
	return Point();
#endif //#if USE_EXPERIMENTAL_3
}

#if USE_EXPERIMENTAL_3
Point3D Data::BinTreeCell::max3D() const
{
    return Point3D();
}
#endif //#if USE_EXPERIMENTAL_3

GeometricShape *Data::BinTreeCell::makeShape() const
{
	if (this->isRoot())
	{
		GeometricShape *s = static_cast<BoxTree *>(this->getTree())->getShape();

		return new Box(*s->getPoint(0), *s->getPoint(1));
	}

#if USE_EXPERIMENTAL_3
	Point2D min = this->min2D();
	Point2D max;
#else
	Point min = this->min();
    Point max(1);
#endif //#if USE_EXPERIMENTAL_3
	max.setCoord(0, min.getCoord(0) + this->size());

	Box *box = new Box(min, max);

	return box;
}

void Data::BinTreeCell::subdivide()
{
	ULInt id = 2*this->getId();

	//left
	new BinTreeCell(this, ++id);
	//right
	new BinTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

#if USE_EXPERIMENTAL_5
BinTreeCell &Data::BinTreeCell::operator=(const BinTreeCell &c)
{
	if (this != &c)
	{
		BoxTreeCell::operator=(c);

        for (UInt i = 0; i < 3; i++)
        {
            this->adjacents[i] = c.adjacents[i];
        }
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_5

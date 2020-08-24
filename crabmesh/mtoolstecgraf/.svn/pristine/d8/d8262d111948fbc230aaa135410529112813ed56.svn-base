#include "Data/QuadTreeCell.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/QuadTree.h"
#include "Data/Box.h"

using namespace Data;

Data::QuadTreeCell::QuadTreeCell(QuadTreeCell *parent, ULInt id) :
#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	BoxTreeCell(id)
#else
	BoxTreeCell(parent, id)
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
{
#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
#if (!USE_EXPERIMENTAL_5)
	this->numAdjs = 1;
	this->BoxTreeCell::adjacents = this->adjacents;
#endif //#if (!USE_EXPERIMENTAL_5)

	this->adjacents[0] = parent;
	this->adjacents[1] = this->adjacents[2] = this->adjacents[3] = this->adjacents[4] = NULL;

	if (parent)
	{
		this->setTree(parent->getTree());

		parent->add(this);

		this->l = parent->level() + 1;
	}
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
}

Data::QuadTreeCell::~QuadTreeCell()
{
#if USE_EXPERIMENTAL_5
    if (!this->isLeaf())
	{
		this->setTree(NULL);
	}
#endif //#if USE_EXPERIMENTAL_5

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	for (UInt i = 1; i < 5; i++)
	{
		if (this->adjacents[i])
		{
			delete this->adjacents[i];
		}
	}

#if (!USE_EXPERIMENTAL_5)
	this->numAdjs = 0;
	this->BoxTreeCell::adjacents = NULL;
#endif //#if (!USE_EXPERIMENTAL_5)
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
}

#if USE_EXPERIMENTAL_5
UInt Data::QuadTreeCell::numAdjacents() const
{
    UInt count = 1;

    for (UInt i = 1; i < 5; i++)
    {
        if (this->adjacents[i])
        {
            count++;
        }
    }

    return count;
}

void Data::QuadTreeCell::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::QuadTreeCell::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::QuadTreeCell::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 5);
}
#endif //#if USE_EXPERIMENTAL_5

UInt Data::QuadTreeCell::dimension() const
{
	return 2;
}

TreeNode *Data::QuadTreeCell::child(enum QuadTreeCell::ChildPosition pos)
{
	switch (pos)
	{
	case QuadTreeCell::BOTTOM_LEFT:
		return TreeNode::child(0);
		break;
	case QuadTreeCell::BOTTOM_RIGHT:
		return TreeNode::child(1);
		break;
	case QuadTreeCell::TOP_RIGHT:
		return TreeNode::child(2);
		break;
	case QuadTreeCell::TOP_LEFT:
		return TreeNode::child(3);
		break;
	}

	return NULL;
}

Real Data::QuadTreeCell::size() const
{
	//even though the level is 0 for the root, let us avoid a power and a division
	return this->isRoot() ? static_cast<BoxTree *>(this->getTree())->size() :
		this->getTree() ? static_cast<BoxTree *>(this->getTree())->size()/std::pow(2.0, static_cast<Real>(this->level())) :
		static_cast<QuadTreeCell *>(this->parent())->size()*0.5;
}

UInt Data::QuadTreeCell::numSides() const
{
	return 4;
}

bool Data::QuadTreeCell::in(Real x, Real y) const
{
	return this->in(Point2D(x, y));
}

bool Data::QuadTreeCell::optIn(Real x, Real y) const
{
	return this->optIn(Point2D(x, y));
}

bool Data::QuadTreeCell::on(Real x, Real y) const
{
	return this->on(Point2D(x, y));
}

bool Data::QuadTreeCell::optOn(Real x, Real y) const
{
	return this->optOn(Point2D(x, y));
}

bool Data::QuadTreeCell::out(Real x, Real y) const
{
	return this->out(Point2D(x, y));
}

bool Data::QuadTreeCell::optOut(Real x, Real y) const
{
	return this->optOut(Point2D(x, y));
}

bool Data::QuadTreeCell::isChild(UInt i) const
{
	return (this->position() == i);
}

UInt Data::QuadTreeCell::position() const
{
	return static_cast<UInt>((this->getId() - 1)%4);
}

#if USE_EXPERIMENTAL_3
Point2D Data::QuadTreeCell::min2D() const
{
	if (this->shp)
	{
		return *static_cast<Point2D *>(static_cast<Box *>(this->shp)->getMin());
	}

	if (this->isRoot())
	{
		return *static_cast<Point2D *>(static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(0));
	}

	Point2D *pmax = static_cast<Point2D *>(static_cast<BoxTreeCell *>(this->parent())->box()->getMax());
	Point2D pmin = static_cast<BoxTreeCell *>(this->parent())->min2D();

	UInt pos = this->position();

	if ((pos == BOTTOM_RIGHT)  || (pos == TOP_RIGHT))
    {
        pmin.setCoord(0, 0.5*(pmin.getCoord(0) + pmax->getCoord(0)));
    }

    if ((pos == TOP_RIGHT)  || (pos == TOP_LEFT))
    {
        pmin.setCoord(1, 0.5*(pmin.getCoord(1) + pmax->getCoord(1)));
    }

	return pmin;

	/*switch (this->position())
	{
	case BOTTOM_LEFT:
		//if I am the bottom left child
		return static_cast<BoxTreeCell *>(this->parent())->min2D();
		break;

	case BOTTOM_RIGHT:
		//if I am the bottom right child
		{
			Point2D pmin = static_cast<BoxTreeCell *>(this->parent())->min2D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(0, pmid.getCoord(0));
			pmin.setCoord(0, pmin.getCoord(0) + this->size());

			return pmin;
		}
		break;

	case TOP_RIGHT:
		//if I am the top right child
		return static_cast<BoxTreeCell *>(this->parent())->mid2D();
		break;

	case TOP_LEFT:
		//if I am the top left child
		{
			Point2D pmin = static_cast<BoxTreeCell *>(this->parent())->min2D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(1, pmid.getCoord(1));
			pmin.setCoord(1, pmin.getCoord(1) + this->size());

			return pmin;
		}
		break;
	}

	return Point2D();*/
}

Point3D Data::QuadTreeCell::min3D() const
{
    return Point3D();
}

Point2D Data::QuadTreeCell::max2D() const
{
	if (this->shp)
	{
		return *static_cast<Point2D *>(static_cast<Box *>(this->shp)->getMax());
	}

	if (this->isRoot())
	{
		return *static_cast<Point2D *>(static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(1));
	}

	Data::Point2D pmax = static_cast<BoxTreeCell *>(this->parent())->max2D();
	Data::Point2D *pmin = static_cast<Point2D *>(static_cast<BoxTreeCell *>(this->parent())->box()->getMin());

	UInt pos = this->position();

	if ((pos == BOTTOM_LEFT)  || (pos == TOP_LEFT))
    {
        pmax.setCoord(0, 0.5*(pmax.getCoord(0) + pmin->getCoord(0)));
    }

    if ((pos == BOTTOM_LEFT)  || (pos == BOTTOM_RIGHT))
    {
        pmax.setCoord(1, 0.5*(pmax.getCoord(1) + pmin->getCoord(1)));
    }

	return pmax;

	/*switch (this->position())
	{
	case BOTTOM_LEFT:
		//if I am the bottom left child
		return static_cast<BoxTreeCell *>(this->parent())->mid2D();
		break;

	case BOTTOM_RIGHT:
		//if I am the bottom right child
		{
			Point2D pmax = static_cast<BoxTreeCell *>(this->parent())->max2D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(1, pmid.getCoord(1));
			pmax.setCoord(1, pmax.getCoord(1) - this->size());

			return pmax;
		}
		break;

	case TOP_RIGHT:
		return static_cast<BoxTreeCell *>(this->parent())->max2D();
		//if I am the top right chlid
		break;

	case TOP_LEFT:
		//if I am the top left child
		{
			Point2D pmax = static_cast<BoxTreeCell *>(this->parent())->max2D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(0, pmid.getCoord(0));
			pmax.setCoord(0, pmax.getCoord(0) - this->size());

			return pmax;
		}
		break;
	}

	return Point2D();*/
}

Point3D Data::QuadTreeCell::max3D() const
{
    return Point3D();
}
#else
Point Data::QuadTreeCell::min() const
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
	case BOTTOM_LEFT:
		//if I am the bottom left child
		return static_cast<BoxTreeCell *>(this->parent())->min();
		break;

	case BOTTOM_RIGHT:
		//if I am the bottom right child
		{
			Point pmin = static_cast<BoxTreeCell *>(this->parent())->min();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(0, pmid.getCoord(0));
			pmin.setCoord(0, pmin.getCoord(0) + this->size());

			return pmin;
		}
		break;

	case TOP_RIGHT:
		//if I am the top right child
		return static_cast<BoxTreeCell *>(this->parent())->mid();
		break;

	case TOP_LEFT:
		//if I am the top left child
		{
			Point pmin = static_cast<BoxTreeCell *>(this->parent())->min();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(1, pmid.getCoord(1));
			pmin.setCoord(1, pmin.getCoord(1) + this->size());

			return pmin;
		}
		break;
	}

	return Point();
}

Point Data::QuadTreeCell::max() const
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
	case BOTTOM_LEFT:
		//if I am the bottom left child
		return static_cast<BoxTreeCell *>(this->parent())->mid();
		break;

	case BOTTOM_RIGHT:
		//if I am the bottom right child
		{
			Point pmax = static_cast<BoxTreeCell *>(this->parent())->max();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(1, pmid.getCoord(1));
			pmax.setCoord(1, pmax.getCoord(1) - this->size());

			return pmax;
		}
		break;

	case TOP_RIGHT:
		return static_cast<BoxTreeCell *>(this->parent())->max();
		//if I am the top right chlid
		break;

	case TOP_LEFT:
		//if I am the top left child
		{
			Point pmax = static_cast<BoxTreeCell *>(this->parent())->max();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(0, pmid.getCoord(0));
			pmax.setCoord(0, pmax.getCoord(0) - this->size());

			return pmax;
		}
		break;
	}

	return Point();
}
#endif //#if USE_EXPERIMENTAL_3

void Data::QuadTreeCell::subdivide()
{
	ULInt id = 4*this->getId();

	//bottom left
	new QuadTreeCell(this, ++id);
	//bottom right
	new QuadTreeCell(this, ++id);
	//top right
	new QuadTreeCell(this, ++id);
	//top left
	new QuadTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
void Data::QuadTreeCell::add(GraphNode *n)
{
	for (UInt i = 1; i < 5; i++)
	{
		if (!this->adjacents[i])
		{
			this->adjacents[i] = n;

#if (!USE_EXPERIMENTAL_5)
			this->numAdjs++;
#endif //#if (!USE_EXPERIMENTAL_5)

			break;
		}
	}
}

void Data::QuadTreeCell::remove(GraphNode *n)
{
	for (UInt i = 1; i < 5; i++)
	{
		if (this->adjacents[i] == n)
		{
			this->adjacents[i] = NULL;

#if (!USE_EXPERIMENTAL_5)
			this->numAdjs--;
#endif //#if (!USE_EXPERIMENTAL_5)

			break;
		}
	}
}

void Data::QuadTreeCell::deleteChildren()
{
	for (UInt i = 1; i < 5; i++)
	{
		if (this->adjacents[i])
		{
			delete this->adjacents[i];

			this->adjacents[i] = NULL;
		}
	}

#if (!USE_EXPERIMENTAL_5)
	this->numAdjs = 1;
#endif //#if (!USE_EXPERIMENTAL_5)

#if (!USE_EXPERIMENTAL_TREE)
	if (this->getTree())
	{
		this->getTree()->add(this);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

QuadTreeCell &Data::QuadTreeCell::operator=(const QuadTreeCell &c)
{
	if (this != &c)
	{
		this->id = c.id;

#if (!USE_EXPERIMENTAL_5)
		this->numAdjs = c.numAdjs;
		this->BoxTreeCell::adjacents = this->adjacents;
#endif //#if (!USE_EXPERIMENTAL_5)

		for (UInt i = 0; i < 5; i++)
		{
			this->adjacents[i] = c.adjacents[i];
		}

		this->l = c.l;

		this->shp = (c.shp) ? this->makeShape() : NULL;
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))

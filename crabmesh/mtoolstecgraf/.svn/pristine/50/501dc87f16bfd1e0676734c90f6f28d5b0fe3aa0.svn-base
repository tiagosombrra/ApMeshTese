#include "Data/OctTreeCell.h"

#include "Data/BoxTree.h"
#include "Data/Box.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

Data::OctTreeCell::OctTreeCell(OctTreeCell *parent, ULInt id) :
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
	this->adjacents[1] = this->adjacents[2] = this->adjacents[3] = this->adjacents[4] =
		this->adjacents[5] = this->adjacents[6] = this->adjacents[7] = this->adjacents[8] = NULL;

	if (parent)
	{
		this->setTree(parent->getTree());

		parent->add(this);

		this->l = parent->level() + 1;
	}
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))

}

Data::OctTreeCell::~OctTreeCell()
{
#if USE_EXPERIMENTAL_5
    if (!this->isLeaf())
	{
		this->setTree(NULL);
	}
#endif //#if USE_EXPERIMENTAL_5

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	for (UInt i = 1; i < 9; i++)
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
UInt Data::OctTreeCell::numAdjacents() const
{
    UInt count = 1;

    for (UInt i = 1; i < 9; i++)
    {
        if (this->adjacents[i])
        {
            count++;
        }
    }

    return count;
}

void Data::OctTreeCell::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::OctTreeCell::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::OctTreeCell::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 9);
}
#endif //#if USE_EXPERIMENTAL_5

UInt Data::OctTreeCell::dimension() const
{
	return 3;
}

TreeNode *Data::OctTreeCell::child(enum OctTreeCell::ChildPosition pos)
{
	switch (pos)
	{
	case OctTreeCell::BOTTOM_LEFT_FAR:
		return TreeNode::child(0);
		break;
	case OctTreeCell::BOTTOM_RIGHT_FAR:
		return TreeNode::child(1);
		break;
	case OctTreeCell::TOP_RIGHT_FAR:
		return TreeNode::child(2);
		break;
	case OctTreeCell::TOP_LEFT_FAR:
		return TreeNode::child(3);
		break;
	case OctTreeCell::BOTTOM_LEFT_NEAR:
		return TreeNode::child(4);
		break;
	case OctTreeCell::BOTTOM_RIGHT_NEAR:
		return TreeNode::child(5);
		break;
	case OctTreeCell::TOP_RIGHT_NEAR:
		return TreeNode::child(6);
		break;
	case OctTreeCell::TOP_LEFT_NEAR:
		return TreeNode::child(7);
		break;
	}

	return NULL;
}

Real Data::OctTreeCell::size() const
{
	//even though the level is 0 for the root, let us avoid a power and a division
	return this->isRoot() ? static_cast<BoxTree *>(this->getTree())->size() :
		this->getTree() ? static_cast<BoxTree *>(this->getTree())->size()/std::pow(2.0, static_cast<Real>(this->level())) :
		static_cast<OctTreeCell *>(this->parent())->size()*0.5;
}

UInt Data::OctTreeCell::numSides() const
{
	return 6;
}

bool Data::OctTreeCell::in(Real x, Real y, Real z) const
{
	return this->in(Point3D(x, y, z));
}

bool Data::OctTreeCell::optIn(Real x, Real y, Real z) const
{
	return this->optIn(Point3D(x, y, z));
}

bool Data::OctTreeCell::on(Real x, Real y, Real z) const
{
	return this->on(Point3D(x, y, z));
}

bool Data::OctTreeCell::optOn(Real x, Real y, Real z) const
{
	return this->optOn(Point3D(x, y, z));
}

bool Data::OctTreeCell::out(Real x, Real y, Real z) const
{
	return this->out(Point3D(x, y, z));
}

bool Data::OctTreeCell::optOut(Real x, Real y, Real z) const
{
	return this->optOut(Point3D(x, y, z));
}

bool Data::OctTreeCell::isChild(UInt i) const
{
	return (this->position() == i);
}

UInt Data::OctTreeCell::position() const
{
	return static_cast<UInt>((this->getId() - 1)%8);
}

#if USE_EXPERIMENTAL_3
Point3D Data::OctTreeCell::min3D() const
{
	if (this->shp)
	{
		return *static_cast<Point3D *>(static_cast<Box *>(this->shp)->getMin());
	}

	if (this->isRoot())
	{
		return *static_cast<Point3D *>(static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(0));
	}

	Point3D *pmax = static_cast<Point3D *>(static_cast<BoxTreeCell *>(this->parent())->box()->getMax());
	Point3D pmin = static_cast<BoxTreeCell *>(this->parent())->min3D();

	UInt pos = this->position();

	if ((pos == BOTTOM_RIGHT_FAR)  || (pos == TOP_RIGHT_FAR) ||
        (pos == BOTTOM_RIGHT_NEAR) || (pos == TOP_RIGHT_NEAR))
    {
        pmin.setCoord(0, 0.5*(pmin.getCoord(0) + pmax->getCoord(0)));
    }

    if ((pos == TOP_LEFT_FAR)  || (pos == TOP_RIGHT_FAR) ||
        (pos == TOP_LEFT_NEAR) || (pos == TOP_RIGHT_NEAR))
    {
        pmin.setCoord(1, 0.5*(pmin.getCoord(1) + pmax->getCoord(1)));
    }

	if ((pos == BOTTOM_LEFT_NEAR) || (pos == BOTTOM_RIGHT_NEAR) ||
        (pos ==   TOP_RIGHT_NEAR) || (pos ==     TOP_LEFT_NEAR))
    {
        pmin.setCoord(2, 0.5*(pmin.getCoord(2) + pmax->getCoord(2)));
    }

	return pmin;

	/*switch (this->position())
	{
	case BOTTOM_LEFT_FAR:
		//if I am the bottom left far child
		return static_cast<BoxTreeCell *>(this->parent())->min3D();
		break;

	case BOTTOM_RIGHT_FAR:
		//if I am the bottom right far child
		{
			Point3D pmin = static_cast<BoxTreeCell *>(this->parent())->min3D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(0, pmid.getCoord(0));
			pmin.setCoord(0, pmin.getCoord(0) + this->size());

			return pmin;
		}
		break;

	case TOP_RIGHT_FAR:
		//if I am the top right far child
		{
			//Point pmin = ((BoxTreeCell *)this->parent())->min();
			Point3D pmid = static_cast<BoxTreeCell *>(this->parent())->mid3D();

			//pmid.setCoord(2, pmin.getCoord(2));
			pmid.setCoord(2, pmid.getCoord(2) - this->size());

			return pmid;
		}
		break;

	case TOP_LEFT_FAR:
		//if I am the top left far child
		{
			Point3D pmin = static_cast<BoxTreeCell *>(this->parent())->min3D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(1, pmid.getCoord(1));
			pmin.setCoord(1, pmin.getCoord(1) + this->size());

			return pmin;
		}
		break;

	case BOTTOM_LEFT_NEAR:
		//if I am the bottom left near child
		{
			Point3D pmin = static_cast<BoxTreeCell *>(this->parent())->min3D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(2, pmid.getCoord(2));
			pmin.setCoord(2, pmin.getCoord(2) + this->size());

			return pmin;
		}
		break;

	case BOTTOM_RIGHT_NEAR:
		//if I am the bottom right near child
		{
			//Point pmin = ((BoxTreeCell *)this->parent())->min();
			Point3D pmid = static_cast<BoxTreeCell *>(this->parent())->mid3D();

			//pmid.setCoord(1, pmin.getCoord(1));
			pmid.setCoord(1, pmid.getCoord(1) - this->size());

			return pmid;
		}
		break;

	case TOP_RIGHT_NEAR:
		//if I am the top right near child
		return static_cast<BoxTreeCell *>(this->parent())->mid3D();
		break;

	case TOP_LEFT_NEAR:
		//if I am the top left near child
		{
			//Point pmin = ((BoxTreeCell *)this->parent())->min();
			Point3D pmid = static_cast<BoxTreeCell *>(this->parent())->mid3D();

			//pmid.setCoord(0, pmin.getCoord(0));
			pmid.setCoord(0, pmid.getCoord(0) - this->size());

			return pmid;
		}
		break;
	}

	return Point3D();*/
}

Point2D Data::OctTreeCell::min2D() const
{
    return Point2D();
}

Point3D Data::OctTreeCell::max3D() const
{
	if (this->shp)
	{
		return *static_cast<Point3D *>(static_cast<Box *>(this->shp)->getMax());
	}

	if (this->isRoot())
	{
		return *static_cast<Point3D *>(static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(1));
	}

	Data::Point3D pmax = static_cast<BoxTreeCell *>(this->parent())->max3D();
	Data::Point3D *pmin = static_cast<Point3D *>(static_cast<BoxTreeCell *>(this->parent())->box()->getMin());

	UInt pos = this->position();

	if ((pos == BOTTOM_LEFT_FAR)  || (pos == TOP_LEFT_FAR) ||
        (pos == BOTTOM_LEFT_NEAR) || (pos == TOP_LEFT_NEAR))
    {
        pmax.setCoord(0, 0.5*(pmax.getCoord(0) + pmin->getCoord(0)));
    }

    if ((pos == BOTTOM_LEFT_FAR)  || (pos == BOTTOM_RIGHT_FAR) ||
        (pos == BOTTOM_LEFT_NEAR) || (pos == BOTTOM_RIGHT_NEAR))
    {
        pmax.setCoord(1, 0.5*(pmax.getCoord(1) + pmin->getCoord(1)));
    }

	if ((pos == BOTTOM_LEFT_FAR) || (pos == BOTTOM_RIGHT_FAR) ||
        (pos ==   TOP_RIGHT_FAR) || (pos ==     TOP_LEFT_FAR))
    {
        pmax.setCoord(2, 0.5*(pmax.getCoord(2) + pmin->getCoord(2)));
    }

	return pmax;

	/*switch (this->position())
	{
	case BOTTOM_LEFT_FAR:
		//if I am the bottom left far child
		return static_cast<BoxTreeCell *>(this->parent())->mid3D();
		break;

	case BOTTOM_RIGHT_FAR:
		//if I am the bottom right far child
		{
			//Point pmax = ((BoxTreeCell *)this->parent())->max();
			Point3D pmid = static_cast<BoxTreeCell *>(this->parent())->mid3D();

			//pmid.setCoord(0, pmax.getCoord(0));
			pmid.setCoord(0, pmid.getCoord(0) + this->size());

			return pmid;
		}
		break;

	case TOP_RIGHT_FAR:
		//if I am the top right far child
		{
			Point3D pmax = static_cast<BoxTreeCell *>(this->parent())->max3D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(2, pmid.getCoord(2));
			pmax.setCoord(2, pmax.getCoord(2) - this->size());

			return pmax;
		}
		break;

	case TOP_LEFT_FAR:
		//if I am the top left far child
		{
			//Point pmax = ((BoxTreeCell *)this->parent())->max();
			Point3D pmid = static_cast<BoxTreeCell *>(this->parent())->mid3D();

			//pmid.setCoord(1, pmax.getCoord(1));
			pmid.setCoord(1, pmid.getCoord(1) + this->size());

			return pmid;
		}
		break;

	case BOTTOM_LEFT_NEAR:
		//if I am the bottom left near child
		{
			//Point pmax = ((BoxTreeCell *)this->parent())->max();
			Point3D pmid = static_cast<BoxTreeCell *>(this->parent())->mid3D();

			//pmid.setCoord(2, pmax.getCoord(2));
			pmid.setCoord(2, pmid.getCoord(2) + this->size());

			return pmid;
		}
		break;

	case BOTTOM_RIGHT_NEAR:
		//if I am the bottom right near child
		{
			Point3D pmax = static_cast<BoxTreeCell *>(this->parent())->max3D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(1, pmid.getCoord(1));
			pmax.setCoord(1, pmax.getCoord(1) - this->size());

			return pmax;
		}
		break;

	case TOP_RIGHT_NEAR:
		//if I am the top right near child
		return static_cast<BoxTreeCell *>(this->parent())->max3D();
		break;

	case TOP_LEFT_NEAR:
		//if I am the top left near child
		{
			Point3D pmax = static_cast<BoxTreeCell *>(this->parent())->max3D();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(0, pmid.getCoord(0));
			pmax.setCoord(0, pmax.getCoord(0) - this->size());

			return pmax;
		}
		break;
	}

	return Point3D();*/
}

Point2D Data::OctTreeCell::max2D() const
{
    return Point2D();
}
#else
Point Data::OctTreeCell::min() const
{
	if (this->shp)
	{
		return static_cast<Box *>(this->shp)->getMin();
	}

	if (this->isRoot())
	{
		return *static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(0);
	}

	switch (this->position())
	{
	case BOTTOM_LEFT_FAR:
		//if I am the bottom left far child
		return static_cast<BoxTreeCell *>(this->parent())->min();
		break;

	case BOTTOM_RIGHT_FAR:
		//if I am the bottom right far child
		{
			Point pmin = static_cast<BoxTreeCell *>(this->parent())->min();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(0, pmid.getCoord(0));
			pmin.setCoord(0, pmin.getCoord(0) + this->size());

			return pmin;
		}
		break;

	case TOP_RIGHT_FAR:
		//if I am the top right far child
		{
			//Point pmin = ((BoxTreeCell *)this->parent())->min();
			Point pmid = static_cast<BoxTreeCell *>(this->parent())->mid();

			//pmid.setCoord(2, pmin.getCoord(2));
			pmid.setCoord(2, pmid.getCoord(2) - this->size());

			return pmid;
		}
		break;

	case TOP_LEFT_FAR:
		//if I am the top left far child
		{
			Point pmin = static_cast<BoxTreeCell *>(this->parent())->min();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(1, pmid.getCoord(1));
			pmin.setCoord(1, pmin.getCoord(1) + this->size());

			return pmin;
		}
		break;

	case BOTTOM_LEFT_NEAR:
		//if I am the bottom left near child
		{
			Point pmin = static_cast<BoxTreeCell *>(this->parent())->min();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmin.setCoord(2, pmid.getCoord(2));
			pmin.setCoord(2, pmin.getCoord(2) + this->size());

			return pmin;
		}
		break;

	case BOTTOM_RIGHT_NEAR:
		//if I am the bottom right near child
		{
			//Point pmin = ((BoxTreeCell *)this->parent())->min();
			Point pmid = static_cast<BoxTreeCell *>(this->parent())->mid();

			//pmid.setCoord(1, pmin.getCoord(1));
			pmid.setCoord(1, pmid.getCoord(1) - this->size());

			return pmid;
		}
		break;

	case TOP_RIGHT_NEAR:
		//if I am the top right near child
		return static_cast<BoxTreeCell *>(this->parent())->mid();
		break;

	case TOP_LEFT_NEAR:
		//if I am the top left near child
		{
			//Point pmin = ((BoxTreeCell *)this->parent())->min();
			Point pmid = static_cast<BoxTreeCell *>(this->parent())->mid();

			//pmid.setCoord(0, pmin.getCoord(0));
			pmid.setCoord(0, pmid.getCoord(0) - this->size());

			return pmid;
		}
		break;
	}

	return Point();
}

Point Data::OctTreeCell::max() const
{
	if (this->shp)
	{
		return static_cast<Box *>(this->shp)->getMax();
	}

	if (this->isRoot())
	{
		return *static_cast<BoxTree *>(this->getTree())->getShape()->getPoint(1);
	}

	switch (this->position())
	{
	case BOTTOM_LEFT_FAR:
		//if I am the bottom left far child
		return static_cast<BoxTreeCell *>(this->parent())->mid();
		break;

	case BOTTOM_RIGHT_FAR:
		//if I am the bottom right far child
		{
			//Point pmax = ((BoxTreeCell *)this->parent())->max();
			Point pmid = static_cast<BoxTreeCell *>(this->parent())->mid();

			//pmid.setCoord(0, pmax.getCoord(0));
			pmid.setCoord(0, pmid.getCoord(0) + this->size());

			return pmid;
		}
		break;

	case TOP_RIGHT_FAR:
		//if I am the top right far child
		{
			Point pmax = static_cast<BoxTreeCell *>(this->parent())->max();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(2, pmid.getCoord(2));
			pmax.setCoord(2, pmax.getCoord(2) - this->size());

			return pmax;
		}
		break;

	case TOP_LEFT_FAR:
		//if I am the top left far child
		{
			//Point pmax = ((BoxTreeCell *)this->parent())->max();
			Point pmid = static_cast<BoxTreeCell *>(this->parent())->mid();

			//pmid.setCoord(1, pmax.getCoord(1));
			pmid.setCoord(1, pmid.getCoord(1) + this->size());

			return pmid;
		}
		break;

	case BOTTOM_LEFT_NEAR:
		//if I am the bottom left near child
		{
			//Point pmax = ((BoxTreeCell *)this->parent())->max();
			Point pmid = static_cast<BoxTreeCell *>(this->parent())->mid();

			//pmid.setCoord(2, pmax.getCoord(2));
			pmid.setCoord(2, pmid.getCoord(2) + this->size());

			return pmid;
		}
		break;

	case BOTTOM_RIGHT_NEAR:
		//if I am the bottom right near child
		{
			Point pmax = static_cast<BoxTreeCell *>(this->parent())->max();
			//Point pmid = ((BoxTreeCell *)this->parent())->mid();

			//pmax.setCoord(1, pmid.getCoord(1));
			pmax.setCoord(1, pmax.getCoord(1) - this->size());

			return pmax;
		}
		break;

	case TOP_RIGHT_NEAR:
		//if I am the top right near child
		return static_cast<BoxTreeCell *>(this->parent())->max();
		break;

	case TOP_LEFT_NEAR:
		//if I am the top left near child
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

void Data::OctTreeCell::subdivide()
{
	ULInt id = 8*this->getId();

	//bottom left far
	new OctTreeCell(this, ++id);
	//bottom right far
	new OctTreeCell(this, ++id);
	//top right far
	new OctTreeCell(this, ++id);
	//top left far
	new OctTreeCell(this, ++id);
	//bottom left near
	new OctTreeCell(this, ++id);
	//bottom right near
	new OctTreeCell(this, ++id);
	//top right near
	new OctTreeCell(this, ++id);
	//top left near
	new OctTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	this->getTree()->addChildren(this);
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
void Data::OctTreeCell::add(GraphNode *n)
{
	for (UInt i = 1; i < 9; i++)
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

void Data::OctTreeCell::remove(GraphNode *n)
{
	for (UInt i = 1; i < 9; i++)
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

void Data::OctTreeCell::deleteChildren()
{
	for (UInt i = 1; i < 9; i++)
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

OctTreeCell &Data::OctTreeCell::operator=(const OctTreeCell &c)
{
	if (this != &c)
	{
		this->id = c.id;

#if (!USE_EXPERIMENTAL_5)
		this->numAdjs = c.numAdjs;
		this->BoxTreeCell::adjacents = this->adjacents;
#endif //#if (!USE_EXPERIMENTAL_5)

		for (UInt i = 0; i < 9; i++)
		{
			this->adjacents[i] = c.adjacents[i];
		}

		this->l = c.l;

		this->shp = (c.shp) ? this->makeShape() : NULL;
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))

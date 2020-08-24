#include "Data/BoxTreeCell.h"

#include "Data/BoxTree.h"
#include "Data/Point.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"

#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

#if USE_GUI
#include "Data/Point3D.h"
#include "Data/State.h"
#include "Data/Edge.h"
#endif //#if USE_GUI

using namespace Data;

#if USE_EXPERIMENTAL
Data::BoxTreeCell::BoxTreeCell(ULInt id) :
	RecursiveDecompositionTreeCell(id)
{

}

Data::BoxTreeCell::BoxTreeCell(BoxTree *tree, ULInt id) :
	RecursiveDecompositionTreeCell(tree, id)
{

}
#endif //#if USE_EXPERIMENTAL

Data::BoxTreeCell::BoxTreeCell(BoxTreeCell *parent, ULInt id) :
	RecursiveDecompositionTreeCell(parent, id)
{

}

Data::BoxTreeCell::~BoxTreeCell()
{

}

UInt Data::BoxTreeCell::dimension() const
{
	//return ((BoxTree *)this->getTree())->getShape()->dimension();
	return this->box()->dimension();
}

bool Data::BoxTreeCell::isChild(UInt i) const
{
	return (this->isRoot()) ? false : (this->parent()->child(i) == this);
}

UInt Data::BoxTreeCell::position() const
{
	return this->parent() ? static_cast<UInt>((static_cast<LInt>(this->getId()) - 1)%this->parent()->numChildren()) : 0;
}

bool Data::BoxTreeCell::isDescendant(ULInt id) const
{
	if (id <= this->getId())
	{
		return false;
	}

	if (this->isRoot())
	{
		return true;
	}

	UInt numChildren = (this->isLeaf()) ? this->parent()->numChildren() : this->numChildren();

	while (id > this->getId())
	{
		ULInt child = (id - 1)%numChildren;
		id = (id - 1 - child)/numChildren;
	}

	return (id == this->getId());
}

bool Data::BoxTreeCell::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::BoxTreeCell::in(const Point *p) const
{
	return this->box()->in(p);
}

bool Data::BoxTreeCell::optIn(const Point &p) const
{
	return this->optIn(&p);
}

bool Data::BoxTreeCell::optIn(const Point *p) const
{
	return this->box()->optIn(p);
}

bool Data::BoxTreeCell::in(const Box &box) const
{
	return this->in(&box);
}

bool Data::BoxTreeCell::in(const Box *box) const
{
	return this->box()->in(box);
}

bool Data::BoxTreeCell::optIn(const Box &box) const
{
	return this->optIn(&box);
}

bool Data::BoxTreeCell::optIn(const Box *box) const
{
	return this->box()->optIn(box);
}

bool Data::BoxTreeCell::in(const Box &box, UInt side) const
{
	return this->in(&box, side);
}

bool Data::BoxTreeCell::in(const Box *box, UInt side) const
{
	UInt dimension = this->dimension();

	//return ((side < dimension) ?
	//		(this->box()->getMax()->getCoord(side)             < box->getMax()->getCoord(side))             :
	//		(this->box()->getMin()->getCoord(side - dimension) > box->getMin()->getCoord(side - dimension)));
	return ((side < dimension) ?
			(this->box()->getMax()->getCoord(side)             < box->getMax()->getCoord(side)             - Data::getTolerance()) :
			(this->box()->getMin()->getCoord(side - dimension) > box->getMin()->getCoord(side - dimension) + Data::getTolerance()));
}

bool Data::BoxTreeCell::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::BoxTreeCell::on(const Point *p) const
{
	return this->box()->on(p);
}

bool Data::BoxTreeCell::optOn(const Point &p) const
{
	return this->optOn(&p);
}

bool Data::BoxTreeCell::optOn(const Point *p) const
{
	return this->box()->optOn(p);
}

bool Data::BoxTreeCell::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::BoxTreeCell::out(const Point *p) const
{
	return this->box()->out(p);
}

bool Data::BoxTreeCell::optOut(const Point &p) const
{
	return this->optOut(&p);
}

bool Data::BoxTreeCell::optOut(const Point *p) const
{
	return this->box()->optOut(p);
}

bool Data::BoxTreeCell::intercept(const Box &box) const
{
	return this->intercept(&box);
}

bool Data::BoxTreeCell::intercept(const Box *box) const
{
	return this->box()->intercept(box);
}

bool Data::BoxTreeCell::optIntercept(const Box &box) const
{
	return this->optIntercept(&box);
}

bool Data::BoxTreeCell::optIntercept(const Box *box) const
{
	return this->box()->optIntercept(box);
}

bool Data::BoxTreeCell::outOf(const Box &box) const
{
	return this->outOf(&box);
}

bool Data::BoxTreeCell::outOf(const Box *box) const
{
	return this->box()->outOf(box);
}

Box *Data::BoxTreeCell::box() const
{
	return static_cast<Box *>(this->shape());
}

RecursiveDecompositionTreeCell *Data::BoxTreeCell::find(const Point &p) const
{
	return this->find(&p);
}

RecursiveDecompositionTreeCell *Data::BoxTreeCell::find(const Point *p) const
{
	//if (this->out(p))
	if (this->optOut(p))
	{
		return NULL;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		RecursiveDecompositionTreeCell *c = static_cast<RecursiveDecompositionTreeCell *>(this->child(i))->find(p);

		if (c)
		{
			return c;
		}
	}

	const RecursiveDecompositionTreeCell *c = this;

	return const_cast<RecursiveDecompositionTreeCell *>(c);
}

Real Data::BoxTreeCell::size() const
{
	//even though the level is 0 for the root, let us avoid a power and a division
	return this->isRoot() ? static_cast<BoxTree *>(this->getTree())->size() :
		static_cast<BoxTree *>(this->getTree())->size()/std::pow(2.0, static_cast<Real>(this->level()));
}

UInt Data::BoxTreeCell::numSides() const
{
	return 2*this->dimension();
}

GraphNodeList Data::BoxTreeCell::neighbors() const
{
	return static_cast<BoxTree *>(this->getTree())->neighbors(this);
}

void Data::BoxTreeCell::neighbors(GraphNodeList &neighs) const
{
	return static_cast<BoxTree *>(this->getTree())->neighbors(this, neighs);
}

GraphNodeList Data::BoxTreeCell::neighbors(UInt side) const
{
	return static_cast<BoxTree *>(this->getTree())->neighbors(this, side);
}

void Data::BoxTreeCell::neighbors(UInt side, GraphNodeList &neighs) const
{
	return static_cast<BoxTree *>(this->getTree())->neighbors(this, side, neighs);
}

/*bool Data::BoxTreeCell::isSubdivided() const
{
	return (!this->isLeaf());
}*/

#if USE_EXPERIMENTAL_3
Point2D Data::BoxTreeCell::mid2D() const
{
	if (this->shp)
	{
		return static_cast<Box *>(this->shp)->mid2D();
	}

	Point2D min = this->min2D();
	Point2D max = this->max2D();

	Point2D mid;

	for (UInt i = 0; i < 2; i++)
	{
		mid.setCoord(i, (min.getCoord(i) + max.getCoord(i))*0.5);
	}

	return mid;
}

Point3D Data::BoxTreeCell::mid3D() const
{
	if (this->shp)
	{
		return static_cast<Box *>(this->shp)->mid3D();
	}

	Point3D min = this->min3D();
	Point3D max = this->max3D();

	Point3D mid;

	for (UInt i = 0; i < 3; i++)
	{
		mid.setCoord(i, (min.getCoord(i) + max.getCoord(i))*0.5);
	}

	return mid;
}
#else
Point Data::BoxTreeCell::mid() const
{
	if (this->shp)
	{
		return static_cast<Box *>(this->shp)->mid();
	}

	Point min = this->min();
	Point max = this->max();

	UInt dim = std::max(min.dimension(), max.dimension());

	Point mid(dim);

	for (UInt i = 0; i < dim; i++)
	{
		mid.setCoord(i, (min.getCoord(i) + max.getCoord(i))*0.5);
	}

	return mid;
}
#endif //#if USE_EXPERIMENTAL_3

GeometricShape *Data::BoxTreeCell::makeShape() const
{
	if (this->isRoot())
	{
		GeometricShape *s = static_cast<BoxTree *>(this->getTree())->getShape();

#if USE_GUI
        Real r, g, b;
        Data::getState().getTypeColor(Data::State::TREE_CELL_DEFAULT, r, g, b);

        Box *box = new Box(*s->getPoint(0), *s->getPoint(1));

        box->setColor(r, g, b);

        return box;
#else
		return new Box(*s->getPoint(0), *s->getPoint(1));
#endif //#endif //#if USE_GUI
	}

#if USE_EXPERIMENTAL_3
	UInt dim = this->getTree() ?
        static_cast<BoxTree *>(this->getTree())->getShape()->dimension() :
        static_cast<BoxTreeCell *>(this->parent())->dimension();

#if USE_GUI
    Real r, g, b;
    Data::getState().getTypeColor(Data::State::TREE_CELL_DEFAULT, r, g, b);

    Box *box = (dim == 2) ?
        new Box(this->min2D(), this->max2D()) :
        new Box(this->min3D(), this->max3D());

    box->setColor(r, g, b);

    return box;
#else
	return (dim == 2) ?
        new Box(this->min2D(), this->max2D()) :
        new Box(this->min3D(), this->max3D());
#endif //#if USE_GUI
#else
#if USE_GUI
    Real r, g, b;
    Data::getState().getTypeColor(Data::State::TREE_CELL_DEFAULT, r, g, b);

    Box *box = new Box(this->min(), this->max());

    box->setColor(r, g, b);

    return box;
#else
	return new Box(this->min(), this->max());
#endif //#if USE_GUI
#endif //#if USE_EXPERIMENTAL_3
}

#if USE_GUI
void Data::BoxTreeCell::draw() const
{
	if ((this->isLeaf()) ||
		(!this->getTree())) //workaround
	{
		/*if (!Data::getState().isInVisibleSpace(this->box()))
		{
			return;
		}*/

		if (!this->shp) //to avoid the duplicated creation of the shape
		{
			return;
		}

		this->box()->draw();

		return;
	}

	if (this->isSubdivided())
	{
		for (UInt i = 0; i < this->numChildren(); i++)
		{
			if (this->child(i))
			{
				this->child(i)->draw();
			}
		}
	}

	/*if (this->isRoot())
	{
		this->box()->draw();
	}

	if ((this->isLeaf()) && (this->box()->isHighlighted()))
	{
		if ((this->isLeaf()) ||
			(!this->getTree())) //workaround
		{
			this->box()->draw();

			return;
		}
	}

	if (this->isSubdivided())
	{
		UInt d = this->dimension();

		for (UInt i = 0; i < d; i++)
		{
			Point3D min, max;

			Edge e(&min, &max);

			for (UInt j = 0; j < d; j++)
			{
				if (j == i)
				{
					min.setCoord(j, this->box()->getMin()->getCoord(j));
					max.setCoord(j, this->box()->getMax()->getCoord(j));
				}
				else
				{
					min.setCoord(j, (this->box()->getMin()->getCoord(j) + this->box()->getMax()->getCoord(j))*0.5);
					max.setCoord(j, min.getCoord(j));
				}
			}

			e.draw();
		}

		for (UInt i = 0; i < this->numChildren(); i++)
		{
			if (this->child(i))
			{
				this->child(i)->draw();
			}
		}
	}*/

	/*//debug
	if ((this->isLeaf()) && (this->box()->isHighlighted()))
	{
		GraphNodeVector neighbors = ((BoxTree *)this->getTree())->neighbors(this);

		for (UInt i = 0; i < neighbors.size(); i++)
		{
			Box *box = ((BoxTreeCell *)neighbors[i])->box();

			glColor(1.0, 1.0, 1.0);

			glBegin(GL_QUADS);
				glVertex(box->getMin()->getCoord(0), box->getMin()->getCoord(1));
				glVertex(box->getMax()->getCoord(0), box->getMin()->getCoord(1));
				glVertex(box->getMax()->getCoord(0), box->getMax()->getCoord(1));
				glVertex(box->getMin()->getCoord(0), box->getMax()->getCoord(1));
			glEnd();
		}
	}
	//endebug*/
}

std::string Data::BoxTreeCell::text() const
{
	std::stringstream str;

	str << this->getId();

	if (!this->isLeaf())
	{
		str << ": (";

		for (UInt i = 0; i < this->numChildren(); i++)
		{
			str << this->child(i)->text();

			if (i < this->numChildren() - 1)
			{
				str << " ";
			}
		}

		str << ")";
	}
	else if (this->getTree())
	{
		str << ", neighs:";

		Data::GraphNodeList neighs = static_cast<Data::BoxTree *>(this->getTree())->neighbors(this);

		//for (UInt i = 0; i < neighs.size(); i++)
		for (Data::GraphNodeList::iterator iter = neighs.begin();
			 iter != neighs.end(); iter++)
		{
			//str << " " << neighs[i]->getId();
			str << " " << (*iter)->getId();
		}
	}

	return str.str();
}
#endif //#if USE_GUI

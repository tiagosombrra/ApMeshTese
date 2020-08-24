#include "MJMesh/Edge2D.h"

#include "MJMesh/Vertex.h"
#include "Data/Vector2D.h"

using namespace MJMesh;

MJMesh::Edge2D::Edge2D(MJMesh::Vertex *v1, MJMesh::Vertex *v2) :
	Data::Edge2D(static_cast<Data::Point2D *>(v1->getPoint()), static_cast<Data::Point2D *>(v2->getPoint()))
{
	//v1->add(this);
	//v2->add(this);

#if USE_ATTRIBS
#if USE_RECALC_ATTRIBS
	this->v = new Data::Vector2D(Data::Edge2D::vector());

	this->s = Data::Edge2D::size();

	this->n = new Data::Vector2D(Data::Edge2D::normal());

	this->m = new Data::Point2D(Data::Edge2D::mid());

	this->bbox = new Data::Box(Data::Edge2D::box());
#else
	this->s = -1.0;
	this->v = NULL;
	this->n = NULL;
	this->m = NULL;
	this->bbox = NULL;
#endif //#if USE_RECALC_ATTRIBS
#endif //#if USE_ATTRIBS
}

MJMesh::Edge2D::~Edge2D()
{
#if USE_ATTRIBS
	if (this->v)
	{
		delete this->v;

#if ((USE_POOL) && (USE_GUI))
		this->v = NULL;
#endif //#if ((USE_POOL) && (USE_GUI))
	}

	if (this->n)
	{
		delete this->n;

#if ((USE_POOL) && (USE_GUI))
		this->n = NULL;
#endif //#if ((USE_POOL) && (USE_GUI))
	}

	if (this->m)
	{
		delete this->m;

#if ((USE_POOL) && (USE_GUI))
		this->m = NULL;
#endif //#if ((USE_POOL) && (USE_GUI))
	}

	if (this->bbox)
	{
		delete this->bbox;

#if ((USE_POOL) && (USE_GUI))
		this->bbox = NULL;
#endif //#if ((USE_POOL) && (USE_GUI))
	}
#endif //#if USE_ATTRIBS
}

#if USE_ATTRIBS
Real MJMesh::Edge2D::size() const
{
#if USE_RECALC_ATTRIBS
	return this->s;
#else
	return (this->s > 0.0) ? this->s : (this->s = Data::Edge2D::size(), this->s);
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
const Data::Vector2D *MJMesh::Edge2D::getVector() const
#else
const Data::Vector *MJMesh::Edge2D::getVector() const
#endif //#if USE_EXPERIMENTAL_3
{
#if USE_RECALC_ATTRIBS
	return this->v;
#else
#if USE_EXPERIMENTAL_3
	return (this->v) ? this->v : (this->v = new Data::Vector2D(Data::Edge2D::vector2D()), this->v);
#else
	return (this->v) ? this->v : (this->v = new Data::Vector2D(Data::Edge2D::vector()), this->v);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
Data::Vector2D MJMesh::Edge2D::vector2D() const
#else
Data::Vector MJMesh::Edge2D::vector() const
#endif //#if USE_EXPERIMENTAL_3
{
	return *this->getVector();
}

const Data::Vector2D *MJMesh::Edge2D::getNormal() const
{
#if USE_RECALC_ATTRIBS
	return this->n;
#else
	return (this->n) ? this->n : (this->n = new Data::Vector2D(Data::Edge2D::normal()), this->n);
#endif //#if USE_RECALC_ATTRIBS
}

Data::Vector2D MJMesh::Edge2D::normal() const
{
	return *this->getNormal();
}

#if USE_EXPERIMENTAL_3
const Data::Point2D *MJMesh::Edge2D::getMid() const
#else
const Data::Point *MJMesh::Edge2D::getMid() const
#endif //#if USE_EXPERIMENTAL_3
{
#if USE_RECALC_ATTRIBS
	return this->m;
#else
#if USE_EXPERIMENTAL_3
	return (this->m) ? this->m : (this->m = new Data::Point2D(Data::Edge2D::mid2D()), this->m);
#else
	return (this->m) ? this->m : (this->m = new Data::Point2D(Data::Edge2D::mid()), this->m);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
Data::Point2D MJMesh::Edge2D::mid2D() const
#else
Data::Point MJMesh::Edge2D::mid() const
#endif //#if USE_EXPERIMENTAL_3
{
	return *this->getMid();
}

const Data::Box *MJMesh::Edge2D::getBox() const
{
#if USE_RECALC_ATTRIBS
	return this->bbox;
#else
	if (!this->bbox)
	{
		Data::Point2D *min = new Data::Point2D(this->points[0]);
		Data::Point2D *max = new Data::Point2D(this->points[0]);

		Real c = this->points[1]->getCoord(0);

		if (c < min->getX())
		{
			min->setX(c);
		}

		if (c > max->getX())
		{
			max->setX(c);
		}

		c = this->points[1]->getCoord(1);

		if (c < min->getY())
		{
			min->setY(c);
		}

		if (c > max->getY())
		{
			max->setY(c);
		}

		this->bbox = new Data::Box(min, max);
	}

	return this->bbox;
#endif //#if USE_RECALC_ATTRIBS
}

Data::Box MJMesh::Edge2D::box() const
{
	return *this->getBox();
}

void MJMesh::Edge2D::invertNormal() const
{
	if (this->v)
	{
		this->v->invert();
	}

	if (this->n)
	{
		this->n->invert();
	}
}

void MJMesh::Edge2D::clearAttribs() const
{
	if (this->v)
	{
		delete this->v;

#if USE_RECALC_ATTRIBS
		this->v = new Data::Vector2D(Data::Edge2D::vector());
#else
		this->v = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

#if USE_RECALC_ATTRIBS
		this->s = Data::Edge2D::size();
#else
		this->s = -1.0;
#endif //#if USE_RECALC_ATTRIBS

	if (this->n)
	{
		delete this->n;

#if USE_RECALC_ATTRIBS
		this->n = new Data::Vector2D(Data::Edge2D::normal());
#else
		this->n = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

	if (this->m)
	{
		delete this->m;

#if USE_RECALC_ATTRIBS
		this->m = new Data::Point2D(Data::Edge2D::mid());
#else
		this->m = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

	if (this->bbox)
	{
		delete this->bbox;

#if USE_RECALC_ATTRIBS
		Data::Box box = Data::Edge2D::box();
		this->bbox = new Data::Box(*box.getMin(), *box.getMax());
#else
		this->bbox = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

	/*this->s = Data::Edge2D::size();

	if (!this->v)
	{
		this->v = new Data::Vector(Data::Edge2D::vector());
	}

	if (!this->n)
	{
		this->n = new Data::Vector2D(Data::Edge2D::normal());
	}

	if (!this->m)
	{
		this->m = new Data::Point2D(Data::Edge2D::mid());
	}

	if (!this->bbox)
	{
		Data::Box b = Data::Edge2D::box();

		this->bbox = new Data::Box(*b.getMin(), *b.getMax());
	}*/
}

void MJMesh::Edge2D::recalcAttribs() const
{
	if (this->v)
	{
		this->v->setCoord(0, this->getP2()->getCoord(0) - this->getP1()->getCoord(0));
		this->v->setCoord(1, this->getP2()->getCoord(1) - this->getP1()->getCoord(1));
	}

	if (this->s > 0.0)
	{
		this->s = Data::Edge2D::size();
	}

	if (this->n)
	{
#if USE_EXPERIMENTAL_3
		Data::Vector2D n = Data::Edge2D::normal();
#else
		Data::Vector n = Data::Edge2D::normal();
#endif //#if USE_EXPERIMENTAL_3

		this->n->setCoord(0, n.getCoord(0));
		this->n->setCoord(1, n.getCoord(1));
	}

	if (this->m)
	{
		this->m->setCoord(0, 0.5*(this->getP1()->getCoord(0) + this->getP2()->getCoord(0)));
		this->m->setCoord(1, 0.5*(this->getP1()->getCoord(1) + this->getP2()->getCoord(1)));
	}

	if (this->bbox)
	{
		this->bbox->getMin()->setCoord(0, std::min(this->getP1()->getCoord(0), this->getP2()->getCoord(0)));
		this->bbox->getMin()->setCoord(1, std::min(this->getP1()->getCoord(1), this->getP2()->getCoord(1)));
		this->bbox->getMax()->setCoord(0, std::max(this->getP1()->getCoord(0), this->getP2()->getCoord(0)));
		this->bbox->getMax()->setCoord(1, std::max(this->getP1()->getCoord(1), this->getP2()->getCoord(1)));
	}
}

#if USE_ARRAY
MJMesh::Edge2D &MJMesh::Edge2D::operator=(const MJMesh::Edge2D &e)
{
	if (this != &e)
	{
		Data::Edge2D::operator=(e);

		this->clearAttribs();

/*#if USE_RECALC_ATTRIBS
		Data::Vector v = Data::Edge2D::vector();
		this->v = new Data::Vector(v);

		this->s = Data::Edge2D::size();

		Data::Vector2D n = Data::Edge2D::normal();
		this->n = new Data::Vector2D(n);

		Data::Point mid = Data::Edge2D::mid();
		this->m = new Data::Point2D(mid);

		Data::Box box = Data::Edge2D::box();
		this->bbox = new Data::Box(*box.getMin(), *box.getMax());
#else
		this->v = NULL;
		this->s = -1.0;
		this->n = NULL;
		this->m = NULL;
		this->bbox = NULL;
#endif //#if USE_RECALC_ATTRIBS*/
	}

	return *this;
}
#endif //#if USE_ARRAY
#endif //#if USE_ATTRIBS

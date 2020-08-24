#include "MJMesh/Edge3D.h"

#include "MJMesh/Vertex.h"
#include "Data/Vector3D.h"

using namespace MJMesh;

MJMesh::Edge3D::Edge3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2) :
	Data::Edge3D(static_cast<Data::Point3D *>(v1->getPoint()), static_cast<Data::Point3D *>(v2->getPoint()))
{
	//v1->add(this);
	//v2->add(this);

#if USE_ATTRIBS
#if USE_RECALC_ATTRIBS
	this->v = new Data::Vector3D(Data::Edge::vector());

	this->s = Data::Edge::size();

	this->m = new Data::Point3D(Data::Edge::mid());

	this->bbox = new Data::Box(Data::Edge::box());
#else
	this->s = -1.0;
	this->v = NULL;
	this->m = NULL;
	this->bbox = NULL;
#endif //#if USE_RECALC_ATTRIBS
#endif //#if USE_ATTRIBS
}

MJMesh::Edge3D::~Edge3D()
{
#if USE_ATTRIBS
	if (this->v)
	{
		delete this->v;

#if ((USE_POOL) && (USE_GUI))
		this->v = NULL;
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

UInt MJMesh::Edge3D::dimension() const
{
	return 3;
}

#if USE_ATTRIBS
Real MJMesh::Edge3D::size() const
{
#if USE_RECALC_ATTRIBS
	return this->s;
#else
	return (this->s > 0.0) ? this->s : (this->s = Data::Edge::size(), this->s);
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
const Data::Vector3D *MJMesh::Edge3D::getVector() const
#else
const Data::Vector *MJMesh::Edge3D::getVector() const
#endif //#if USE_EXPERIMENTAL_3
{
#if USE_RECALC_ATTRIBS
	return this->v;
#else
#if USE_EXPERIMENTAL_3
	return (this->v) ? this->v : (this->v = new Data::Vector3D(Data::Edge::vector3D()), this->v);
#else
	return (this->v) ? this->v : (this->v = new Data::Vector3D(Data::Edge::vector()), this->v);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
Data::Vector3D MJMesh::Edge3D::vector3D() const
#else
Data::Vector MJMesh::Edge3D::vector() const
#endif //#if USE_EXPERIMENTAL_3
{
	return *this->getVector();
}

#if USE_EXPERIMENTAL_3
const Data::Point3D *MJMesh::Edge3D::getMid() const
#else
const Data::Point *MJMesh::Edge3D::getMid() const
#endif //#if USE_EXPERIMENTAL_3
{
#if USE_RECALC_ATTRIBS
	return this->m;
#else
#if USE_EXPERIMENTAL_3
	return (this->m) ? this->m : (this->m = new Data::Point3D(Data::Edge::mid3D()), this->m);
#else
	return (this->m) ? this->m : (this->m = new Data::Point3D(Data::Edge::mid()), this->m);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
Data::Point3D MJMesh::Edge3D::mid3D() const
#else
Data::Point MJMesh::Edge3D::mid() const
#endif //#if USE_EXPERIMENTAL_3
{
	return *this->getMid();
}

const Data::Box *MJMesh::Edge3D::getBox() const
{
#if USE_RECALC_ATTRIBS
	return this->bbox;
#else
	if (!this->bbox)
	{
		Data::Point3D *min = new Data::Point3D(this->points[0]);
		Data::Point3D *max = new Data::Point3D(this->points[0]);

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

		c = this->points[1]->getCoord(2);

		if (c < min->getZ())
		{
			min->setZ(c);
		}

		if (c > max->getZ())
		{
			max->setZ(c);
		}

		this->bbox = new Data::Box(min, max);
	}

	return this->bbox;
#endif //#if USE_RECALC_ATTRIBS
}

Data::Box MJMesh::Edge3D::box() const
{
	return *this->getBox();
}

void MJMesh::Edge3D::invertNormal() const
{
	if (this->v)
	{
		this->v->invert();
	}
}

void MJMesh::Edge3D::clearAttribs() const
{
	if (this->v)
	{
		delete this->v;

#if USE_RECALC_ATTRIBS
		this->v = new Data::Vector3D(Data::Edge::vector());
#else
		this->v = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

#if USE_RECALC_ATTRIBS
		this->s = Data::Edge::size();
#else
		this->s = -1.0;
#endif //#if USE_RECALC_ATTRIBS

	if (this->m)
	{
		delete this->m;

#if USE_RECALC_ATTRIBS
		this->m = new Data::Point3D(Data::Edge::mid());
#else
		this->m = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

	if (this->bbox)
	{
		delete this->bbox;

#if USE_RECALC_ATTRIBS
		Data::Box box = Data::Edge::box();
		this->bbox = new Data::Box(*box.getMin(), *box.getMax());
#else
		this->bbox = NULL;
#endif //#if USE_RECALC_ATTRIBS
	}

	/*this->s = Data::Edge::size();

	if (!this->v)
	{
		this->v = new Data::Vector(Data::Edge::vector());
	}

	if (!this->m)
	{
		this->m = new Data::Point3D(Data::Edge::mid());
	}

	if (!this->bbox)
	{
		Data::Box b = Data::Edge::box();

		this->bbox = new Data::Box(*b.getMin(), *b.getMax());
	}*/
}

void MJMesh::Edge3D::recalcAttribs() const
{
	if (this->v)
	{
		this->v->setCoord(0, this->getP2()->getCoord(0) - this->getP1()->getCoord(0));
		this->v->setCoord(1, this->getP2()->getCoord(1) - this->getP1()->getCoord(1));
	}

	if (this->s > 0.0)
	{
		this->s = Data::Edge::size();
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
		this->bbox->getMin()->setCoord(2, std::min(this->getP1()->getCoord(2), this->getP2()->getCoord(2)));
		this->bbox->getMax()->setCoord(0, std::max(this->getP1()->getCoord(0), this->getP2()->getCoord(0)));
		this->bbox->getMax()->setCoord(1, std::max(this->getP1()->getCoord(1), this->getP2()->getCoord(1)));
		this->bbox->getMax()->setCoord(2, std::max(this->getP1()->getCoord(2), this->getP2()->getCoord(2)));
	}
}

#if USE_ARRAY
MJMesh::Edge3D &MJMesh::Edge3D::operator=(const MJMesh::Edge3D &e)
{
	if (this != &e)
	{
		Data::Edge::operator=(e);

		this->clearAttribs();

/*#if USE_RECALC_ATTRIBS
		Data::Vector v = Data::Edge::vector();
		this->v = new Data::Vector(v);

		this->s = Data::Edge::size();

		Data::Point mid = Data::Edge::mid();
		this->m = new Data::Point3D(mid);

		Data::Box box = Data::Edge::box();
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

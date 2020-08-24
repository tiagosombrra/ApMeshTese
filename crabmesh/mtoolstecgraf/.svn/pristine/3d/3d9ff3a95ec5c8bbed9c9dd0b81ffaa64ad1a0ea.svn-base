#include "MJMesh/Triangle3D.h"

#include "Data/Vector3D.h"
#include "Data/Box.h"

using namespace MJMesh;

MJMesh::Triangle3D::Triangle3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3) :
	Data::Triangle3D(static_cast<Data::Point3D *>(v1->getPoint()), static_cast<Data::Point3D *>(v2->getPoint()), static_cast<Data::Point3D *>(v3->getPoint()))
{
#if USE_ATTRIBS
#if USE_RECALC_ATTRIBS
	this->s = Data::Triangle3D::size();

	this->n = new Data::Vector3D(Data::Triangle3D::normal());

	this->m = new Data::Point3D(Data::Triangle3D::mid());

	Data::Box b = Data::Triangle3D::box();
	this->bbox = new Data::Box(*b.getMin(), *b.getMax());
#else
	this->s = -1.0;
	this->n = NULL;
	this->m = NULL;
	this->bbox = NULL;
#endif //#if USE_RECALC_ATTRIBS
#endif //#if USE_ATTRIBS
}

MJMesh::Triangle3D::~Triangle3D()
{
#if USE_ATTRIBS
	if (this->n)
	{
		delete this->n;
	}

	if (this->m)
	{
		delete this->m;
	}

	if (this->bbox)
	{
		delete this->bbox;
	}
#endif //#if USE_ATTRIBS
}

#if USE_ATTRIBS
Real MJMesh::Triangle3D::size() const
{
#if USE_RECALC_ATTRIBS
	return this->s;
#else
	return (this->s > 0.0) ? this->s : (this->s = Data::Triangle3D::size(), this->s);
#endif //#if USE_RECALC_ATTRIBS
}

const Data::Vector3D *MJMesh::Triangle3D::getNormal() const
{
#if USE_RECALC_ATTRIBS
	return this->n;
#else
	return (this->n) ? this->n : (this->n = new Data::Vector3D(Data::Triangle3D::normal()), this->n);
#endif //#if USE_RECALC_ATTRIBS
}

Data::Vector3D MJMesh::Triangle3D::normal() const
{
	return *this->getNormal();
}

#if USE_EXPERIMENTAL_3
const Data::Point3D *MJMesh::Triangle3D::getMid() const
#else
const Data::Point *MJMesh::Triangle3D::getMid() const
#endif //#if USE_EXPERIMENTAL_3
{
#if USE_RECALC_ATTRIBS
	return this->m;
#else
#if USE_EXPERIMENTAL_3
	return (this->m) ? this->m : (this->m = new Data::Point3D(Data::Triangle3D::mid3D()), this->m);
#else
	return (this->m) ? this->m : (this->m = new Data::Point3D(Data::Triangle3D::mid()), this->m);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_RECALC_ATTRIBS
}

#if USE_EXPERIMENTAL_3
Data::Point3D MJMesh::Triangle3D::mid3D() const
#else
Data::Point MJMesh::Triangle3D::mid() const
#endif //#if USE_EXPERIMENTAL_3
{
	return *this->getMid();
}

const Data::Box *MJMesh::Triangle3D::getBox() const
{
#if USE_RECALC_ATTRIBS
	return this->bbox;
#else
	if (!this->bbox)
	{
		Data::Point3D *min = new Data::Point3D(this->points[0]);
		Data::Point3D *max = new Data::Point3D(this->points[0]);

		for (UInt i = 1; i < 3; i++)
		{
			for (UInt j = 0; j < 3; j++)
			{
				Real coord = this->points[i]->getCoord(j);

				if (coord < min->getCoord(j))
				{
					min->setCoord(j, coord);
				}

				if (coord > max->getCoord(j))
				{
					max->setCoord(j, coord);
				}
			}
		}

		this->bbox = new Data::Box(min, max);
	}

	return this->bbox;
#endif //#if USE_RECALC_ATTRIBS
}

Data::Box MJMesh::Triangle3D::box() const
{
	return *this->getBox();
}

void MJMesh::Triangle3D::invertNormal() const
{
	if (this->n)
	{
		this->n->invert();
	}
}

void MJMesh::Triangle3D::clearAttribs() const
{
#if USE_RECALC_ATTRIBS
	this->s = Data::Triangle3D::size();

	if (this->n)
	{
		delete this->n;

		this->n = new Data::Vector3D(Data::Triangle3D::normal());
	}

	if (this->m)
	{
		delete this->m;

		this->m = new Data::Point3D(Data::Triangle3D::mid());
	}

	if (this->bbox)
	{
		delete this->bbox;

		Data::Box b = Data::Triangle3D::box();
		this->bbox = new Data::Box(*b.getMin(), *b.getMax());
	}
#else
	this->s = -1.0;

	if (this->n)
	{
		delete this->n;

		this->n = NULL;
	}

	if (this->m)
	{
		delete this->m;

		this->m = NULL;
	}

	if (this->bbox)
	{
		delete this->bbox;

		this->bbox = NULL;
	}
#endif //#if USE_RECALC_ATTRIBS

	/*this->s = Data::Triangle3D::size();

	if (!this->n)
	{
		this->n = new Data::Vector3D(Data::Triangle3D::normal());
	}

	if (!this->m)
	{
		this->m = new Data::Point3D(Data::Triangle3D::mid());
	}

	if (!this->bbox)
	{
		Data::Box b = Data::Triangle3D::box();

		this->bbox = new Data::Box(*b.getMin(), *b.getMax());
	}*/
}

void MJMesh::Triangle3D::recalcAttribs() const
{
	if (this->s > 0.0)
	{
		this->s = Data::Triangle3D::size();
	}

	if (this->n)
	{
		Data::Vector3D n = Data::Triangle3D::normal();

		this->n->setCoord(0, n.getCoord(0));
		this->n->setCoord(1, n.getCoord(1));
		this->n->setCoord(2, n.getCoord(2));
	}

	if (this->m)
	{
		this->m->setCoord(0, 0.5*(this->getP1()->getCoord(0) + this->getP2()->getCoord(0)));
		this->m->setCoord(1, 0.5*(this->getP1()->getCoord(1) + this->getP2()->getCoord(1)));
		this->m->setCoord(2, 0.5*(this->getP1()->getCoord(2) + this->getP2()->getCoord(2)));
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
MJMesh::Triangle3D &MJMesh::Triangle3D::operator=(const MJMesh::Triangle3D &t)
{
	if (this != &t)
	{
		Data::Triangle3D::operator=(t);

		this->clearAttribs();

/*#if USE_RECALC_ATTRIBS
		this->s = Data::Triangle3D::size();

		this->n = new Data::Vector3D(Data::Triangle3D::normal());

		this->m = new Data::Point3D(Data::Triangle3D::mid());

		Data::Box b = Data::Triangle3D::box();
		this->bbox = new Data::Box(*b.getMin(), *b.getMax());
#else
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

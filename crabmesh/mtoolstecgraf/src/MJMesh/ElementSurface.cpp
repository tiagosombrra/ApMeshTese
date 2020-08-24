#include "MJMesh/ElementSurface.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Edge3D.h"
#include "Data/Point3D.h"
#include "Data/Box.h"
#include "Data/Triangle3D.h"
#include "Data/Quadrilateral.h"

using namespace MJMesh;

MJMesh::ElementSurface::ElementSurface(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
	ULInt id) :
	MJMesh::Element(
			new Data::Triangle3D(static_cast<Data::Point3D *>(v1->getPoint()), static_cast<Data::Point3D *>(v2->getPoint()), static_cast<Data::Point3D *>(v3->getPoint())),
		id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 3;
	this->MJMesh::Element::vertices = this->vertices;
#else
	this->resize(3);
#endif //#if USE_EXPERIMENTAL

	this->setVertices(v1, v2, v3);
}

#if USE_QUADS
MJMesh::ElementSurface::ElementSurface(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
	ULInt id) :
	MJMesh::Element(
			new Data::Quadrilateral(
				static_cast<Data::Point3D *>(v1->getPoint()), static_cast<Data::Point3D *>(v2->getPoint()),
				static_cast<Data::Point3D *>(v3->getPoint()), static_cast<Data::Point3D *>(v4->getPoint())),
		id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 4;
	this->MJMesh::Element::vertices = this->vertices;
#else
	this->resize(4);
#endif //#if USE_EXPERIMENTAL

	this->setVertices(v1, v2, v3, v4);
}
#endif //#if USE_QUADS

MJMesh::ElementSurface::~ElementSurface()
{
#if USE_EXPERIMENTAL
	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i])
		{
			static_cast<MJMesh::Vertex *>(this->vertices[i])->remove(this);
		}
	}

	this->numVerts = 0;
	this->MJMesh::Element::vertices = NULL;
#endif //#if USE_EXPERIMENTAL
}

void MJMesh::ElementSurface::setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3)
{
	this->setVertex(0, v1);
	this->setVertex(1, v2);
	this->setVertex(2, v3);
#if USE_QUADS
	this->vertices[3] = NULL;
#endif //#if USE_QUADS

	v1->add(this);
	v2->add(this);
	v3->add(this);
}

#if USE_QUADS
void MJMesh::ElementSurface::setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4)
{
	this->setVertex(0, v1);
	this->setVertex(1, v2);
	this->setVertex(2, v3);
	this->setVertex(3, v4);

	v1->add(this);
	v2->add(this);
	v3->add(this);
	v4->add(this);
}
#endif //#if USE_QUADS

UInt MJMesh::ElementSurface::numSides() const
{
    return static_cast<Data::Polygon *>(this->getShape())->numSides();
}

UInt MJMesh::ElementSurface::numVerticesInSide(UInt /*side*/) const
{
    return 2;
}

Data::Vertex *MJMesh::ElementSurface::getVertexInSide(UInt vertex, UInt side) const
{
    /*UInt nv = this->numVerticesInSide(side);

    if (vertex >= nv)
    {
        return NULL;
    }*/

    return this->getVertex((side + vertex)%this->numVertices());
}

void MJMesh::ElementSurface::invert()
{
    Data::Vertex *v = this->getVertex(0);
	Data::Point *p = this->getShape()->getPoint(0);

	this->vertices[0] = this->getVertex(1);
	this->getShape()->setPoint(0, this->getShape()->getPoint(1));

	this->vertices[1] = v;
	this->getShape()->setPoint(1, p);

/*#if USE_QUADS
	if (this->numVertices() == 3)
#endif //#if USE_QUADS
    {
        static_cast<MJMesh::Triangle3D *>(this->getShape())->invertNormal();
    }
#if USE_QUADS
	else
    {
        static_cast<MJMesh::Quadrilateral *>(this->getShape())->invertNormal();
    }
#endif //#if USE_QUADS*/
}

Real MJMesh::ElementSurface::gamma() const
{
#if USE_QUADS
	if (this->getVertex(3))
	{
		Data::Triangle3D t1(static_cast<Data::Point3D *>(this->getVertex(0)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(1)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(2)->getPoint()));
		Data::Triangle3D t2(static_cast<Data::Point3D *>(this->getVertex(0)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(2)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(3)->getPoint()));

		return 0.5*(t1.gamma() + t2.gamma());
	}
#endif //#if USE_QUADS

	return static_cast<Data::Triangle3D *>(this->getShape())->gamma();
}

Real MJMesh::ElementSurface::quality() const
{
#if USE_QUADS
	if (this->getVertex(3))
	{
		Data::Triangle3D t1(
			static_cast<Data::Point3D *>(this->getVertex(0)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(1)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(2)->getPoint()));

		Real a = t1.getP1()->distance(t1.getP2());
		Real b = t1.getP2()->distance(t1.getP3());
		Real c = t1.getP3()->distance(t1.getP1());

		Real s = 0.5*(a + b + c);
		Real d = (s - a)*(s - b)*(s - c);

		Real rInsc = std::sqrt(d/s);
		Real rCirc = 0.25*a*b*c/sqrt(s*d);

		Real q1 = 2.0*rInsc/rCirc;

		Data::Triangle3D t2(
			static_cast<Data::Point3D *>(this->getVertex(0)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(2)->getPoint()),
			static_cast<Data::Point3D *>(this->getVertex(3)->getPoint()));

		a = t2.getP1()->distance(t2.getP2());
		b = t2.getP2()->distance(t2.getP3());
		c = t2.getP3()->distance(t2.getP1());

		s = 0.5*(a + b + c);
		d = (s - a)*(s - b)*(s - c);

		rInsc = std::sqrt(d/s);
		rCirc = 0.25*a*b*c/sqrt(s*d);

		Real q2 = 2.0*rInsc/rCirc;

		return 0.5*(q1 + q2);
	}
#endif //#if USE_QUADS

	Data::Triangle3D *t = static_cast<Data::Triangle3D *>(this->getShape());

	Real a = t->getP1()->distance(t->getP2());
	Real b = t->getP2()->distance(t->getP3());
	Real c = t->getP3()->distance(t->getP1());

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	Real rInsc = std::sqrt(d/s);
	Real rCirc = 0.25*a*b*c/sqrt(s*d);

	return 2.0*rInsc/rCirc;

	/*Real rInsc = REAL_MAX;
    Real rCirc = -REAL_MAX;

    Data::Triangle3D *t = static_cast<Data::Triangle3D *>(this->getShape());

    Point3D insc(t->incenter());
    Point3D circ(t->circumcenter());

    for (UInt i = 0; i < 3; i++)
    {
    	Data::Edge3D e(static_cast<Data::Point3D *>(t->getPoint(i)), static_cast<Data::Point3D *>(t->getPoint((i+1)%3)));

    	Real d = e.straightDistance(insc);

    	//std::cout << "rinsc = " << d << std::endl;

    	e.setPoints(NULL, NULL);

		rInsc = (d < rInsc) ? d : rInsc;
    }

    for (UInt i = 0; i < 3; i++)
    {
		Real d = circ.distance(t->getPoint(i));

		//std::cout << "rcirc = " << d << std::endl;

		rCirc = (d > rCirc) ? d : rCirc;
	}

    return 2.0*rInsc/rCirc;*/
}


bool MJMesh::ElementSurface::isBad() const
{
	static const Real gammaStar = 4.0/std::sqrt(3.0);

	//gamma* = gamma of an equilateral triangle with side 1

#if USE_QUADS
	if (this->getVertex(3))
	{
		return (this->gamma()/(2.0*gammaStar) > 1.5);
	}
#endif //#if USE_QUADS

	return (this->gamma()/gammaStar > 1.5);
}

bool MJMesh::ElementSurface::intercept(const Data::Element &e) const
{
	return this->intercept(&e);
}

bool MJMesh::ElementSurface::intercept(const Data::Element *e) const
{
	//if (!this->getShape()->box().intercept(e->getShape()->box()))
	if (!this->getShape()->box().optIntercept(e->getShape()->box()))
	{
		return false;
	}

	Data::GeometricShape *s = e->getShape();

	//two triangles intercept if
	//  one edge of one triangle intercepts one edge of the other triangle or
	//  one point of one triangle is inside the other triangle

	bool intercepted = false;

	for (UInt i = 0; i < s->numPoints(); i++)
	{
		Data::Point3D *p11 = static_cast<Data::Point3D *>(s->getPoint(i));
		Data::Point3D *p12 = static_cast<Data::Point3D *>(s->getPoint((i+1)%s->numPoints()));

		Data::Edge3D edge(p11, p12);

		if (this->intercept(edge))
		{
			intercepted = true;
		}

		edge.setPoints(NULL, NULL);

		if (intercepted)
		{
			break;
		}

		if (e->intercept(static_cast<Data::Point3D *>(this->getShape()->getPoint(i))))
		{
			intercepted = true;

			break;
		}
	}

	return intercepted;
}

bool MJMesh::ElementSurface::intercept(const Data::Edge &e) const
{
	return this->intercept(&e);
}

bool MJMesh::ElementSurface::intercept(const Data::Edge *e) const
{
	//if (!this->getShape()->box().intercept(e->box()))
	if (!this->getShape()->box().optIntercept(e->box()))
	{
		return false;
	}

	if ((this->in(e->getP1())) ||
		(this->in(e->getP2())))
	{
		return true;
	}

	Data::GeometricShape *s = this->getShape();

	for (UInt i = 0; i < s->numPoints(); i++)
	{
		Data::Point3D *p1 = static_cast<Data::Point3D *>(s->getPoint(i));
		Data::Point3D *p2 = static_cast<Data::Point3D *>(s->getPoint((i+1)%s->numPoints()));

		if (static_cast<const Data::Edge3D *>(e)->intercept(p1, p2))
		{
			return true;
		}
	}

	return false;
}

bool MJMesh::ElementSurface::intercept(const Data::Point &p) const
{
	return this->intercept(&p);
}

bool MJMesh::ElementSurface::intercept(const Data::Point *p) const
{
	return !this->getShape()->out(p);
}

bool MJMesh::ElementSurface::have(const Data::Edge &e) const
{
	return this->have(&e);
}

bool MJMesh::ElementSurface::have(const Data::Edge *e) const
{
	Data::GeometricShape *s = this->getShape();

	return ((s->have(e->getP1())) && (s->have(e->getP2())));
}

bool MJMesh::ElementSurface::have(const Data::FrontElement &e) const
{
	return this->have(&e);
}

bool MJMesh::ElementSurface::have(const Data::FrontElement *e) const
{
	return this->have(static_cast<MJMesh::Edge3D *>(e->getShape()));
}

bool MJMesh::ElementSurface::isAdjacent(const Data::Element &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::ElementSurface::isAdjacent(const Data::Element *e) const
{
	for (UInt i = 0; i < e->numVertices(); i++)
    {
        Data::Vertex *v1 = e->getVertex(i);
        Data::Vertex *v2 = e->getVertex((i + 1)%e->numVertices());

        if ((this->have(v1)) && (this->have(v2)))
        {
            return true;
        }
    }

    return false;

	/*Data::Polygon *p = static_cast<Data::Polygon *>(e->getShape());

	bool adjacent = false;

	for (UInt i = 0; i < p->numPoints(); i++)
	{
		Data::Point *p1 = p->getPoint(i);
		Data::Point *p2 = p->getPoint((i+1)%p->numPoints());

		Data::Edge3D e(p1, p2);

		if (this->have(e))
		{
			adjacent = true;
		}

		e.setPoints(NULL, NULL);

		if (adjacent)
		{
			break;
		}
	}

	return adjacent;*/
}

#if USE_ARRAY
#if USE_EXPERIMENTAL
MJMesh::ElementSurface &MJMesh::ElementSurface::operator=(const MJMesh::ElementSurface &e)
{
	if (this != &e)
	{
#if USE_QUADS
		this->numVerts = 4;
		this->MJMesh::Element::vertices = this->vertices;
#else
		this->numVerts = 3;
		this->MJMesh::Element::vertices = this->vertices;
#endif //#if USE_QUADS

		this->shape = e.shape;

		this->vertices[0] = e.vertices[0];
		this->vertices[1] = e.vertices[1];
		this->vertices[2] = e.vertices[2];
#if USE_QUADS
		this->vertices[3] = e.vertices[3];
#endif //#if USE_QUADS
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL
#endif //#if USE_ARRAY

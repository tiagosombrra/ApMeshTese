#include "MJMesh/Element3D.h"

#include "Data/Point3D.h"
#include "Data/Edge3D.h"
#include "Data/Tetrahedron.h"
#include "Data/Pyramid.h"
#include "Data/Wedge.h"
#include "Data/Brick.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Vertex.h"
#include "Data/OStream.h"

using namespace MJMesh;

MJMesh::Element3D::Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
	MJMesh::Vertex *v4, ULInt id):
	MJMesh::Element(
			new Data::Tetrahedron(static_cast<Data::Point3D *>(v1->getPoint()), static_cast<Data::Point3D *>(v2->getPoint()), static_cast<Data::Point3D *>(v3->getPoint()), static_cast<Data::Point3D *>(v4->getPoint())),
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

#if USE_BRICKS
MJMesh::Element3D::Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
	MJMesh::Vertex *v4, MJMesh::Vertex *v5, ULInt id):
		MJMesh::Element(
			new Data::Pyramid(static_cast<Data::Point3D *>(v1->getPoint()),
				static_cast<Data::Point3D *>(v2->getPoint()),
				static_cast<Data::Point3D *>(v3->getPoint()),
				static_cast<Data::Point3D *>(v4->getPoint()),
				static_cast<Data::Point3D *>(v5->getPoint())),
			id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 5;
	this->MJMesh::Element::vertices = this->vertices;
#else
	this->resize(5);
#endif //#if USE_EXPERIMENTAL

	this->setVertices(v1, v2, v3, v4, v5);
}

MJMesh::Element3D::Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
	MJMesh::Vertex *v4, MJMesh::Vertex *v5, MJMesh::Vertex *v6, ULInt id):
		MJMesh::Element(
			new Data::Wedge(static_cast<Data::Point3D *>(v1->getPoint()),
				static_cast<Data::Point3D *>(v2->getPoint()),
				static_cast<Data::Point3D *>(v3->getPoint()),
				static_cast<Data::Point3D *>(v4->getPoint()),
				static_cast<Data::Point3D *>(v5->getPoint()),
				static_cast<Data::Point3D *>(v6->getPoint())),
			id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 6;
	this->MJMesh::Element::vertices = this->vertices;
#else
	this->resize(6);
#endif //#if USE_EXPERIMENTAL

	this->setVertices(v1, v2, v3, v4, v5, v6);
}

MJMesh::Element3D::Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
	MJMesh::Vertex *v4, MJMesh::Vertex *v5, MJMesh::Vertex *v6,
	MJMesh::Vertex *v7, MJMesh::Vertex *v8, ULInt id):
		MJMesh::Element(
			new Data::Brick(static_cast<Data::Point3D *>(v1->getPoint()),
				static_cast<Data::Point3D *>(v2->getPoint()),
				static_cast<Data::Point3D *>(v3->getPoint()),
				static_cast<Data::Point3D *>(v4->getPoint()),
				static_cast<Data::Point3D *>(v5->getPoint()),
				static_cast<Data::Point3D *>(v6->getPoint()),
				static_cast<Data::Point3D *>(v7->getPoint()),
				static_cast<Data::Point3D *>(v8->getPoint())),
			id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 8;
	this->MJMesh::Element::vertices = this->vertices;
#else
	this->resize(8);
#endif //#if USE_EXPERIMENTAL

	this->setVertices(v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif //#if USE_BRICKS

MJMesh::Element3D::~Element3D()
{
#if USE_EXPERIMENTAL
	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i])
		{
		    //Data::cout() << "trying v at " << this->vertices[i] << Data::endl;

			static_cast<MJMesh::Vertex *>(this->vertices[i])->remove(this);
		}
	}

	this->numVerts = 0;
	this->MJMesh::Element::vertices = NULL;
#endif //#if USE_EXPERIMENTAL
}

void MJMesh::Element3D::setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4)
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

#if USE_BRICKS
void MJMesh::Element3D::setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
	MJMesh::Vertex *v5)
{
	this->setVertex(0, v1);
	this->setVertex(1, v2);
	this->setVertex(2, v3);
	this->setVertex(3, v4);
	this->setVertex(4, v5);

	v1->add(this);
	v2->add(this);
	v3->add(this);
	v4->add(this);
	v5->add(this);
}

void MJMesh::Element3D::setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
	MJMesh::Vertex *v5, MJMesh::Vertex *v6)
{
	this->setVertex(0, v1);
	this->setVertex(1, v2);
	this->setVertex(2, v3);
	this->setVertex(3, v4);
	this->setVertex(4, v5);
	this->setVertex(5, v6);

	v1->add(this);
	v2->add(this);
	v3->add(this);
	v4->add(this);
	v5->add(this);
	v6->add(this);
}

void MJMesh::Element3D::setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
	MJMesh::Vertex *v5, MJMesh::Vertex *v6, MJMesh::Vertex *v7, MJMesh::Vertex *v8)
{
	this->setVertex(0, v1);
	this->setVertex(1, v2);
	this->setVertex(2, v3);
	this->setVertex(3, v4);
	this->setVertex(4, v5);
	this->setVertex(5, v6);
	this->setVertex(6, v7);
	this->setVertex(7, v8);

	v1->add(this);
	v2->add(this);
	v3->add(this);
	v4->add(this);
	v5->add(this);
	v6->add(this);
	v7->add(this);
	v8->add(this);
}
#endif //#if USE_BRICKS

UInt MJMesh::Element3D::numSides() const
{
    return static_cast<Data::Polyhedron *>(this->getShape())->numSides();
}

UInt MJMesh::Element3D::numVerticesInSide(UInt side) const
{
#if USE_BRICKS
    if (dynamic_cast<Data::Tetrahedron *>(this->getShape()))
#endif //#if USE_BRICKS
    {
        return 3;
    }

#if USE_BRICKS
    if (dynamic_cast<Data::Pyramid *>(this->getShape()))
    {
        if (side == 0)
        {
            return 4;
        }

        return 3;
    }

    if (dynamic_cast<Data::Wedge *>(this->getShape()))
    {
        if ((side == 0) || (side == 1))
        {
            return 3;
        }

        return 4;
    }

    if (dynamic_cast<Data::Brick *>(this->getShape()))
    {
        return 4;
    }

    return 0;
#endif //#if USE_BRICKS
}

Data::Vertex *MJMesh::Element3D::getVertexInSide(UInt vertex, UInt side) const
{
    /*UInt nv = this->numVerticesInSide(side);

    if (vertex >= nv)
    {
        return NULL;
    }*/

#if USE_BRICKS
    if (dynamic_cast<Data::Tetrahedron *>(this->getShape()))
#endif //#if USE_BRICKS
    {
        if (side == 0)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(2);
            }

            if (vertex == 2)
            {
                return this->getVertex(1);
            }
        }

        if (side == 1)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(1);
            }

            if (vertex == 2)
            {
                return this->getVertex(3);
            }
        }

        if (side == 2)
        {
            if (vertex == 0)
            {
                return this->getVertex(1);
            }

            if (vertex == 1)
            {
                return this->getVertex(2);
            }

            if (vertex == 2)
            {
                return this->getVertex(3);
            }
        }

        if (side == 3)
        {
            if (vertex == 0)
            {
                return this->getVertex(2);
            }

            if (vertex == 1)
            {
                return this->getVertex(0);
            }

            if (vertex == 2)
            {
                return this->getVertex(3);
            }
        }
    }

#if USE_BRICKS
    if (dynamic_cast<Data::Pyramid *>(this->getShape()))
    {
        if (side == 0)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(3);
            }

            if (vertex == 2)
            {
                return this->getVertex(2);
            }

            if (vertex == 3)
            {
                return this->getVertex(1);
            }
        }

        if (side == 1)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(1);
            }

            if (vertex == 2)
            {
                return this->getVertex(4);
            }
        }

        if (side == 2)
        {
            if (vertex == 0)
            {
                return this->getVertex(1);
            }

            if (vertex == 1)
            {
                return this->getVertex(2);
            }

            if (vertex == 2)
            {
                return this->getVertex(4);
            }
        }

        if (side == 3)
        {
            if (vertex == 0)
            {
                return this->getVertex(2);
            }

            if (vertex == 1)
            {
                return this->getVertex(3);
            }

            if (vertex == 2)
            {
                return this->getVertex(4);
            }
        }

        if (side == 4)
        {
            if (vertex == 0)
            {
                return this->getVertex(3);
            }

            if (vertex == 1)
            {
                return this->getVertex(0);
            }

            if (vertex == 2)
            {
                return this->getVertex(4);
            }
        }
    }

    if (dynamic_cast<Data::Wedge *>(this->getShape()))
    {
        if (side == 0)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(2);
            }

            if (vertex == 2)
            {
                return this->getVertex(1);
            }
        }

        if (side == 1)
        {
            if (vertex == 0)
            {
                return this->getVertex(3);
            }

            if (vertex == 1)
            {
                return this->getVertex(4);
            }

            if (vertex == 2)
            {
                return this->getVertex(5);
            }
        }

        if (side == 2)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(1);
            }

            if (vertex == 2)
            {
                return this->getVertex(4);
            }

            if (vertex == 3)
            {
                return this->getVertex(3);
            }
        }

        if (side == 3)
        {
            if (vertex == 0)
            {
                return this->getVertex(1);
            }

            if (vertex == 1)
            {
                return this->getVertex(2);
            }

            if (vertex == 2)
            {
                return this->getVertex(5);
            }

            if (vertex == 3)
            {
                return this->getVertex(4);
            }
        }

        if (side == 4)
        {
            if (vertex == 0)
            {
                return this->getVertex(2);
            }

            if (vertex == 1)
            {
                return this->getVertex(0);
            }

            if (vertex == 2)
            {
                return this->getVertex(3);
            }

            if (vertex == 3)
            {
                return this->getVertex(5);
            }
        }
    }

    if (dynamic_cast<Data::Brick *>(this->getShape()))
    {
        if (side == 0)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(3);
            }

            if (vertex == 2)
            {
                return this->getVertex(2);
            }

            if (vertex == 3)
            {
                return this->getVertex(1);
            }
        }

        if (side == 1)
        {
            if (vertex == 0)
            {
                return this->getVertex(4);
            }

            if (vertex == 1)
            {
                return this->getVertex(5);
            }

            if (vertex == 2)
            {
                return this->getVertex(6);
            }

            if (vertex == 3)
            {
                return this->getVertex(7);
            }
        }

        if (side == 2)
        {
            if (vertex == 0)
            {
                return this->getVertex(0);
            }

            if (vertex == 1)
            {
                return this->getVertex(1);
            }

            if (vertex == 2)
            {
                return this->getVertex(5);
            }

            if (vertex == 3)
            {
                return this->getVertex(4);
            }
        }

        if (side == 3)
        {
            if (vertex == 0)
            {
                return this->getVertex(1);
            }

            if (vertex == 1)
            {
                return this->getVertex(2);
            }

            if (vertex == 2)
            {
                return this->getVertex(6);
            }

            if (vertex == 3)
            {
                return this->getVertex(5);
            }
        }

        if (side == 4)
        {
            if (vertex == 0)
            {
                return this->getVertex(2);
            }

            if (vertex == 1)
            {
                return this->getVertex(3);
            }

            if (vertex == 2)
            {
                return this->getVertex(7);
            }

            if (vertex == 3)
            {
                return this->getVertex(6);
            }
        }

        if (side == 5)
        {
            if (vertex == 0)
            {
                return this->getVertex(3);
            }

            if (vertex == 1)
            {
                return this->getVertex(0);
            }

            if (vertex == 2)
            {
                return this->getVertex(4);
            }

            if (vertex == 3)
            {
                return this->getVertex(7);
            }
        }
    }
#endif //#if USE_BRICKS

    return NULL;
}

Real MJMesh::Element3D::gamma() const
{
#if USE_BRICKS
    if (this->numVertices() != 4)
    {
        return 0.0;
    }
#endif //#if USE_BRICKS

	return static_cast<Data::Tetrahedron *>(this->getShape())->gamma();
}

Real MJMesh::Element3D::quality() const
{
#if USE_BRICKS
    if (this->numVertices() != 4)
    {
        return 0.0;
    }
#endif //#if USE_BRICKS

	Data::Tetrahedron *t = static_cast<Data::Tetrahedron *>(this->getShape());

	Data::Vector3D a(t->getP1(), t->getP2());
	Data::Vector3D b(t->getP1(), t->getP3());
	Data::Vector3D c(t->getP1(), t->getP4());

	Data::Vector3D ab = a.cross(b);
	Data::Vector3D bc = b.cross(c);
	Data::Vector3D ca = c.cross(a);

	Real v6 = std::fabs(a.dot(bc));

	Data::Vector3D sum = bc;
	sum.sum(ca);
	sum.sum(ab);

	Real rInsc = v6/(bc.norm() + ca.norm() + ab.norm() + sum.norm());

	bc.multiply(a.squaredNorm());
	ca.multiply(b.squaredNorm());
	ab.multiply(c.squaredNorm());

	sum = bc;
	sum.sum(ca);
	sum.sum(ab);

	Real rCirc = sum.norm()/(2.0*v6);

	return 3.0*rInsc/rCirc;

	/*Real rInsc = REAL_MAX;
    Real rCirc = -REAL_MAX;

    Data::Tetrahedron *t = static_cast<Data::Tetrahedron *>(this->getShape());

    Point insc = t->incenter();
    Point circ = t->circumcenter();

    for (UInt i = 0; i < 4; i++)
    {
    	Data::Triangle3D tri(static_cast<Data::Point3D *>(t->getPoint(i)),
							 static_cast<Data::Point3D *>(t->getPoint((i+1)%4)),
							 static_cast<Data::Point3D *>(t->getPoint((i+2)%4)));

		Real d = tri.straightDistance(insc);

		//std::cout << "rinsc = " << d << std::endl;

		tri.setPoints(NULL, NULL, NULL);

		rInsc = (d < rInsc) ? d : rInsc;
    }

    for (UInt i = 0; i < 4; i++)
    {
		Real d = circ.distance(t->getPoint(i));

		//std::cout << "rcirc = " << d << std::endl;

		rCirc = (d > rCirc) ? d : rCirc;
    }

    return 3.0*rInsc/rCirc;*/
}

bool MJMesh::Element3D::isBad() const
{
	static const Real gammaStar = 12.0/std::sqrt(2.0);
	static const Real gammaStar30 = 30.0*gammaStar;
	static const Real gammaStarOver30 = gammaStar/30.0;

	Real gamma = this->gamma();

#if USE_JMSH_DEBUG_MESSAGES
	std::cout << "markos debug, back tracking, elem = " << this->getId() - 1 << ", ind_sha = " << gamma << ", limits = [" << gammaStarOver30 << ", " << gammaStar30 << "]" << std::endl;
	std::cout << "markos debug, elem = " << this->getId() - 1 << " is (" << this->getVertex(0)->getId() - 1 << ", " << this->getVertex(1)->getId() - 1 <<
					", " << this->getVertex(2)->getId() - 1 << ", " << this->getVertex(3)->getId() - 1 << ")" << std::endl;
#endif //#if USE_JMSH_DEBUG_MESSAGES

	//gamma* = gamma of an equilateral tetrahedron with side 1
	//return ((gamma < gammaStar) || (gamma > gammaStar30));
	//return ((gamma < 5.0) || (gamma > gammaStar30));
	return ((gamma < gammaStarOver30) || (gamma > gammaStar30));
}

bool MJMesh::Element3D::intercept(const Data::Element &e) const
{
	return this->intercept(&e);
}

bool MJMesh::Element3D::intercept(const Data::Element *e) const
{
#if USE_BRICKS
    if (this->numVertices() != 4)
    {
        return true;
    }
#endif //#if USE_BRICKS

	//if (!this->getShape()->box().intercept(e->getShape()->box()))
	if (!this->getShape()->box().optIntercept(e->getShape()->box()))
	{
		return false;
	}

	Data::Tetrahedron *t = static_cast<Data::Tetrahedron *>(e->getShape());

	//two tetrahedra intercept if
	//  one edge of one tetrahedron intercepts one face of the other tetrahedron or
	//  one point of one tetrahedron is inside the other tetrahedron

	bool intercepted = false;

	for (UInt i = 0; i < 4; i++)
	{
		Data::Point3D *p11 = static_cast<Data::Point3D *>(t->getPoint(i));
		Data::Point3D *p12 = static_cast<Data::Point3D *>(t->getPoint((i+1)%4));

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

	return intercepted;;
}

bool MJMesh::Element3D::intercept(const Data::Edge &e) const
{
	return this->intercept(&e);
}

bool MJMesh::Element3D::intercept(const Data::Edge *e) const
{
	for (UInt i = 0; i < 4; i++)
	{
		Data::Point *p1 = this->getShape()->getPoint(i);
		Data::Point *p2 = this->getShape()->getPoint((i+1)%4);
		Data::Point *p3 = this->getShape()->getPoint((i+2)%4);

		Data::Triangle3D t(p1, p2, p3);

		bool intercept = t.intercept(e);

		t.setPoints(NULL, NULL, NULL);

		if (intercept)
		{
			return true;
		}
	}

	return false;
}

bool MJMesh::Element3D::intercept(const Data::Point &p) const
{
	return this->intercept(&p);
}

bool MJMesh::Element3D::intercept(const Data::Point *p) const
{
	//return !static_cast<Data::Tetrahedron *>(this->getShape())->out(p);
	return !static_cast<Data::Polyhedron *>(this->getShape())->out(p);
}

bool MJMesh::Element3D::have(const Data::Edge &e) const
{
	return this->have(&e);
}

bool MJMesh::Element3D::have(const Data::Edge *e) const
{
	//Data::Tetrahedron *t = static_cast<Data::Tetrahedron *>(this->getShape());
	Data::Polyhedron *p = static_cast<Data::Polyhedron *>(this->getShape());

	//return ((t->have(e->getP1())) && (t->have(e->getP2())));
	return ((p->have(e->getP1())) && (p->have(e->getP2())));
}

//bool MJMesh::Element3D::have(const Data::Triangle &t) const
bool MJMesh::Element3D::have(const Data::Polygon &p) const
{
	//return this->have(&t);
	return this->have(&p);
}

//bool MJMesh::Element3D::have(const Data::Triangle *t) const
bool MJMesh::Element3D::have(const Data::Polygon *p) const
{
	//Data::Tetrahedron *tet = static_cast<Data::Tetrahedron *>(this->getShape());
	Data::Polyhedron *pol = static_cast<Data::Polyhedron *>(this->getShape());

	//return ((tet->have(t->getP1())) && (tet->have(t->getP2())) && (tet->have(t->getP3())));

	for (UInt i = 0; i < p->numPoints(); i++)
    {
        if (!pol->have(p->getPoint(i)))
        {
            return false;
        }
    }

    return true;
}

bool MJMesh::Element3D::have(const Data::FrontElement &e) const
{
	return this->have(&e);
}

bool MJMesh::Element3D::have(const Data::FrontElement *e) const
{
	return this->have(static_cast<const MJMesh::FrontElement3D *>(e));
}

bool MJMesh::Element3D::have(const MJMesh::FrontElement3D &e) const
{
	return this->have(&e);
}

bool MJMesh::Element3D::have(const MJMesh::FrontElement3D *e) const
{
	//return ((e->haveAdjacent(this)) || (this->have(static_cast<Data::Triangle3D *>(e->getShape()))));
	return ((e->haveAdjacent(this)) || (this->have(static_cast<Data::Polygon *>(e->getShape()))));
}

bool MJMesh::Element3D::isAdjacent(const Data::Element &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::Element3D::isAdjacent(const Data::Element *e) const
{
    UInt numSides = this->numSides();

    for (UInt i = 0; i < numSides; i++)
    {
        UInt numVerticesInSide = this->numVerticesInSide(i);

        bool foundAll = true;

        for (UInt j = 0; j < numVerticesInSide; j++)
        {
            Data::Vertex *v = this->getVertexInSide(j, i);

            if (!e->have(v))
            {
                foundAll = false;

                break;
            }
        }

        if (foundAll)
        {
            return true;
        }
    }

    return false;

	/*for (UInt i = 0; i < e->numVertices(); i++)
    {
        Data::Vertex *v1 = e->getVertex(i);
        Data::Vertex *v2 = e->getVertex((i + 1)%e->numVertices());
        Data::Vertex *v3 = e->getVertex((i + 2)%e->numVertices());

        if ((this->have(v1)) && (this->have(v2)) && (this->have(v3)))
        {
            return true;
        }
    }

    return false;*/

	/*Data::Polyhedron *p = static_cast<Data::Polyhedron *>(e->getShape());

	bool adjacent = false;

	for (UInt i = 0; i < p->numPoints(); i++)
	{
		Data::Point *p1 = p->getPoint(i);
		Data::Point *p2 = p->getPoint((i+1)%p->numPoints());

		Data::Edge e(p1, p2);

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
MJMesh::Element3D &MJMesh::Element3D::operator=(const MJMesh::Element3D &e)
{
	if (this != &e)
	{
#if USE_BRICKS
		this->numVerts = e.numVerts;
#else
		this->numVerts = 4;
#endif //#if USE_BRICKS
		this->MJMesh::Element::vertices = this->vertices;

		this->shape = e.shape;

		this->vertices[0] = e.vertices[0];
		this->vertices[1] = e.vertices[1];
		this->vertices[2] = e.vertices[2];
		this->vertices[3] = e.vertices[3];
#if USE_BRICKS
		this->vertices[4] = e.vertices[4];
		this->vertices[5] = e.vertices[5];
		this->vertices[6] = e.vertices[6];
		this->vertices[7] = e.vertices[7];
#endif //#if USE_BRICKS
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL
#endif //#if USE_ARRAY

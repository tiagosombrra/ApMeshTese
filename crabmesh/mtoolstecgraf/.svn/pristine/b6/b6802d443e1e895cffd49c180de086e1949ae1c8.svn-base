#include "MJMesh/FrontElement3D.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/Edge3D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Front.h"
#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Tetrahedron.h"
#include "Data/Quadrilateral.h"
#include "Data/State.h"

#include "Data/OStream.h"

using namespace MJMesh;

/*MJMesh::FrontElement3D::MJMesh::FrontElement3D(Data::Triangle3D *t) :
	MJMesh::FrontElement(t, t->getId())
{

}*/

MJMesh::FrontElement3D::FrontElement3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2,
	MJMesh::Vertex *v3, ULInt id) :
	MJMesh::FrontElement(
		new MJMesh::Triangle3D(v1, v2, v3),
		id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 3;
	this->MJMesh::FrontElement::vertices = this->vertices;
#else
	this->resize(3);
#endif //#if USE_EXPERIMENTAL

	this->setVertex(0, v1);
	this->setVertex(1, v2);
	this->setVertex(2, v3);

	v1->add(this);
	v2->add(this);
	v3->add(this);
}

#if USE_QUADS
MJMesh::FrontElement3D::FrontElement3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2,
	MJMesh::Vertex *v3, MJMesh::Vertex *v4, ULInt id) :
	MJMesh::FrontElement(
		new Data::Quadrilateral(v1->getPoint(), v2->getPoint(), v3->getPoint(), v4->getPoint()),
		id)
{
#if USE_EXPERIMENTAL
	this->numVerts = 4;
	this->MJMesh::FrontElement::vertices = this->vertices;
#else
	this->resize(4);
#endif //#if USE_EXPERIMENTAL

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

MJMesh::FrontElement3D::~FrontElement3D()
{
    /*//debug
    if (this->getId() == 84082)
    {
        Data::cout() << "deleting fel " << this->getId() << " at " << this << std::endl << "trace: " << Data::trace() << Data::endl;
    }
    //endebug*/

#if USE_EXPERIMENTAL
	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i])
		{
			static_cast<MJMesh::Vertex *>(this->vertices[i])->remove(this);
		}
	}

	this->numVerts = 0;
	this->MJMesh::FrontElement::vertices = NULL;
#endif //#if USE_EXPERIMENTAL
}

MJMesh::Vertex *MJMesh::FrontElement3D::getV3() const
{
	return static_cast<MJMesh::Vertex *>(this->getVertex(2));
}

#if USE_QUADS
MJMesh::Vertex *MJMesh::FrontElement3D::getV4() const
{
	return static_cast<MJMesh::Vertex *>(this->getVertex(3));
}
#endif //#if USE_QUADS

UInt MJMesh::FrontElement3D::numSides() const
{
    return static_cast<Data::Polygon *>(this->getShape())->numSides();
}

UInt MJMesh::FrontElement3D::numVerticesInSide(UInt /*side*/) const
{
    return 2;
}

Data::Vertex *MJMesh::FrontElement3D::getVertexInSide(UInt vertex, UInt side) const
{
    /*UInt nv = this->numVerticesInSide(side);

    if (vertex >= nv)
    {
        return NULL;
    }*/

    return this->getVertex((side + vertex)%this->numVertices());
}

bool MJMesh::FrontElement3D::intercept(const Data::Element &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement3D::intercept(const Data::Element *e) const
{
	return this->intercept(static_cast<const MJMesh::Element3D *>(e));;
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::Element3D &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::Element3D *e) const
{
	Data::Tetrahedron *t = static_cast<Data::Tetrahedron *>(e->getShape());
	MJMesh::Triangle3D *triangle = static_cast<MJMesh::Triangle3D *>(this->getShape());

	//Data::Box box1 = triangle->box();
	//Data::Box box2 = t->box();

	//if (!box1.intercept(box2))
	//if (!box1.optIntercept(box2))
	if (!triangle->box().optIntercept(t->box()))
	{
		return false;
	}

	return ((triangle->intercept(static_cast<Data::Point3D *>(t->getP1()), static_cast<Data::Point3D *>(t->getP2()), static_cast<Data::Point3D *>(t->getP2()))) ||
			(triangle->intercept(static_cast<Data::Point3D *>(t->getP1()), static_cast<Data::Point3D *>(t->getP2()), static_cast<Data::Point3D *>(t->getP4()))) ||
			(triangle->intercept(static_cast<Data::Point3D *>(t->getP2()), static_cast<Data::Point3D *>(t->getP3()), static_cast<Data::Point3D *>(t->getP4()))) ||
			(triangle->intercept(static_cast<Data::Point3D *>(t->getP3()), static_cast<Data::Point3D *>(t->getP1()), static_cast<Data::Point3D *>(t->getP4()))) ||
			(t->in(static_cast<Data::Point3D *>(triangle->getP1()))) ||
			(t->in(static_cast<Data::Point3D *>(triangle->getP2()))) ||
			(t->in(static_cast<Data::Point2D *>(triangle->getP3()))));
}

bool MJMesh::FrontElement3D::intercept(const Data::FrontElement *e) const
{
	return this->intercept(static_cast<const MJMesh::FrontElement3D *>(e));
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::FrontElement3D *e) const
{
	return this->intercept(static_cast<MJMesh::Triangle3D *>(e->getShape()));
}

bool MJMesh::FrontElement3D::intercept(const Data::Triangle &t) const
{
	return this->intercept(&t);
}

bool MJMesh::FrontElement3D::intercept(const Data::Triangle *t) const
{
	return this->intercept(static_cast<const MJMesh::Triangle3D *>(t));
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::Triangle3D &t) const
{
	return this->intercept(&t);
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::Triangle3D *t) const
{
	return static_cast<MJMesh::Triangle3D *>(this->getShape())->intercept(t);
}

bool MJMesh::FrontElement3D::intercept(const Data::Edge &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement3D::intercept(const Data::Edge *e) const
{
	return this->intercept(static_cast<const MJMesh::Edge3D *>(e));
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::Edge3D &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement3D::intercept(const MJMesh::Edge3D *e) const
{
	return static_cast<Data::Triangle3D *>(this->getShape())->intercept(e);
}

bool MJMesh::FrontElement3D::intercept(const Data::Point &p) const
{
	return this->intercept(&p);
}

bool MJMesh::FrontElement3D::intercept(const Data::Point *p) const
{
	return (this->distance(p) < Data::getTolerance());


	//same as MJMesh::FrontElement2D::intercept(const Data::Point *p) const

	//return (this->distance(p)/this->getShape()->size() < Shape::getTolerance()*50.0);
	///*

	Real size = this->getShape()->size();

	if (size < 1.0)
	{
		//return (this->distance(p)/(size*size) < Data::getTolerance()*50.0*50.0);
		return (this->distance(p)*size < Data::getTolerance()*50.0);
	}

	//return (this->distance(p) < Data::getTolerance()*50.0*50.0);
	return (this->distance(p) < Data::getTolerance()*50.0);
	//*/

	//return this->distance(p) < Data::getTolerance()*std::pow(this->getShape()->size(), 2.0);
}

#if USE_EXACT
bool MJMesh::FrontElement3D::eintercept(const Data::FrontElement *e) const
{
	return this->eintercept(static_cast<const MJMesh::FrontElement3D *>(e));
}

bool MJMesh::FrontElement3D::eintercept(const MJMesh::FrontElement3D *e) const
{
	return this->eintercept(static_cast<MJMesh::Triangle3D *>(e->getShape()));
}

bool MJMesh::FrontElement3D::eintercept(const Data::Triangle &t) const
{
	return this->eintercept(&t);
}

bool MJMesh::FrontElement3D::eintercept(const Data::Triangle *t) const
{
	return this->eintercept(static_cast<const MJMesh::Triangle3D *>(t));
}

bool MJMesh::FrontElement3D::eintercept(const MJMesh::Triangle3D &t) const
{
	return this->eintercept(&t);
}

bool MJMesh::FrontElement3D::eintercept(const MJMesh::Triangle3D *t) const
{
	return static_cast<MJMesh::Triangle3D *>(this->getShape())->eintercept(t);
}
#endif //#if USE_EXACT

bool MJMesh::FrontElement3D::accordingToNormal(Data::Point *p, bool insideTest) const
{
	return static_cast<MJMesh::Triangle3D *>(this->getShape())->accordingToNormal(static_cast<Data::Point3D *>(p), insideTest);
}

bool MJMesh::FrontElement3D::have(const Data::Triangle &t) const
{
	return this->have(&t);
}

bool MJMesh::FrontElement3D::have(const Data::Triangle *t) const
{
	return this->have(static_cast<const MJMesh::Triangle3D *>(t));
}

bool MJMesh::FrontElement3D::have(const MJMesh::Triangle3D &t) const
{
	return this->have(&t);
}

bool MJMesh::FrontElement3D::have(const MJMesh::Triangle3D *t) const
{
	return static_cast<MJMesh::Triangle3D *>(this->getShape())->equal(t);
}

bool MJMesh::FrontElement3D::have(const Data::Edge &e) const
{
	return this->have(&e);
}

bool MJMesh::FrontElement3D::have(const Data::Edge *e) const
{
	return (e->equal(this->getV1()->getPoint(), this->getV2()->getPoint()) ||
			e->equal(this->getV2()->getPoint(), this->getV3()->getPoint()) ||
			e->equal(this->getV3()->getPoint(), this->getV1()->getPoint()));
}

/*bool MJMesh::FrontElement3D::have(const MJMesh::Edge3D &e) const
{
	return this->have(&e);
}

bool MJMesh::FrontElement3D::have(const MJMesh::Edge3D *e) const
{
	return (e->equal(this->getV1()->getPoint(), this->getV2()->getPoint()) ||
			e->equal(this->getV2()->getPoint(), this->getV3()->getPoint()) ||
			e->equal(this->getV3()->getPoint(), this->getV1()->getPoint()));
}*/

bool MJMesh::FrontElement3D::isAdjacent(const Data::Element &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::FrontElement3D::isAdjacent(const Data::Element *e) const
{
	return this->isAdjacent(static_cast<const MJMesh::Element3D *>(e));
}

bool MJMesh::FrontElement3D::isAdjacent(const MJMesh::Element3D &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::FrontElement3D::isAdjacent(const MJMesh::Element3D *e) const
{
	Data::Tetrahedron *t = static_cast<Data::Tetrahedron *>(e->getShape());
	MJMesh::Triangle3D *triangle = static_cast<MJMesh::Triangle3D *>(this->getShape());

	return ((t->have(triangle->getP1())) &&
			(t->have(triangle->getP2())) &&
			(t->have(triangle->getP3())));
}

Real MJMesh::FrontElement3D::angle(const Data::Point *p) const
{
	//return static_cast<Data::Triangle3D *>(this->shape)->solidAngle(Data::Point3D(p));
	return static_cast<Data::Triangle3D *>(this->shape)->solidAngle(static_cast<const Data::Point3D *>(p));
}

Real MJMesh::FrontElement3D::distance(const Data::Point *p) const
{
	//return static_cast<Data::Triangle3D *>(this->shape)->distance(Data::Point3D(p));
	return static_cast<Data::Triangle3D *>(this->shape)->distance(static_cast<const Data::Point3D *>(p));
}

Real MJMesh::FrontElement3D::straightDistance(const Data::Point *p) const
{
	return static_cast<Data::Triangle3D *>(this->shape)->straightDistance(static_cast<const Data::Point3D *>(p));
}

#if USE_ATTRIBS
void MJMesh::FrontElement3D::invert()
{
	MJMesh::FrontElement::invert();

	static_cast<MJMesh::Triangle3D *>(this->getShape())->invertNormal();
}
#endif //#if USE_ATTRIBS

Data::Element *MJMesh::FrontElement3D::advance(Data::Vertex *v, const Data::Front *f,
	Data::FrontElementList &newFront, Data::FrontElementList &oldFront) const
{
	return this->advance(static_cast<MJMesh::Vertex *>(v), static_cast<const MJMesh::Front *>(f), newFront, oldFront);
}

MJMesh::Element3D *MJMesh::FrontElement3D::advance(MJMesh::Vertex *v, const MJMesh::Front *f,
	Data::FrontElementList &newFront, Data::FrontElementList &oldFront) const
{
	MJMesh::Element3D *element = new MJMesh::Element3D(this->getV1(), this->getV2(), this->getV3(), v);

	this->add(element);

	Data::VertexVector vertices(3, static_cast<Data::Vertex *>(NULL));

	vertices[0] = this->getV1(); //vertices.push_back(this->getV1());
	vertices[1] = this->getV2(); //vertices.push_back(this->getV2());
	vertices[2] = v; //vertices.push_back(v);

	MJMesh::FrontElement3D *fel1 = static_cast<MJMesh::FrontElement3D *>(f->find(vertices, this->getV3()));

	vertices[0] = this->getV2();
	vertices[1] = this->getV3();

	MJMesh::FrontElement3D *fel2 = static_cast<MJMesh::FrontElement3D *>(f->find(vertices, this->getV1()));

	vertices[0] = this->getV3();
	vertices[1] = this->getV1();

	MJMesh::FrontElement3D *fel3 = static_cast<MJMesh::FrontElement3D *>(f->find(vertices, this->getV2()));

	if (fel1)
	{
		oldFront.push_back(fel1);
	}
	else
	{
		fel1 = new MJMesh::FrontElement3D(this->getV1(), this->getV2(), v);

		newFront.push_back(fel1);
	}

	if (fel2)
	{
		oldFront.push_back(fel2);
	}
	else
	{
		fel2 = new MJMesh::FrontElement3D(this->getV2(), this->getV3(), v);

		newFront.push_back(fel2);
	}

	if (fel3)
	{
		oldFront.push_back(fel3);
	}
	else
	{
		fel3 = new MJMesh::FrontElement3D(this->getV3(), this->getV1(), v);

		newFront.push_back(fel3);
	}

	fel1->add(element);
	fel2->add(element);
	fel3->add(element);

	return element;
}

bool MJMesh::FrontElement3D::match(const Data::VertexVector &vertices) const
{
    return (((this->getV1() == vertices[0]) && (this->getV2() == vertices[1]) && this->getV3() == vertices[2]) ||
			((this->getV1() == vertices[0]) && (this->getV2() == vertices[2]) && this->getV3() == vertices[1]) ||
			((this->getV1() == vertices[1]) && (this->getV2() == vertices[0]) && this->getV3() == vertices[2]) ||
			((this->getV1() == vertices[1]) && (this->getV2() == vertices[2]) && this->getV3() == vertices[0]) ||
			((this->getV1() == vertices[2]) && (this->getV2() == vertices[1]) && this->getV3() == vertices[0]) ||
			((this->getV1() == vertices[2]) && (this->getV2() == vertices[0]) && this->getV3() == vertices[1]));
}

bool MJMesh::FrontElement3D::geometricallyMatch(const Data::VertexVector &vertices) const
{
	return static_cast<MJMesh::Triangle3D *>(this->getShape())->match(
		vertices[0]->getPoint(), vertices[1]->getPoint(), vertices[2]->getPoint());
}

#if USE_EXPERIMENTAL
MJMesh::FrontElement3D &MJMesh::FrontElement3D::operator=(const MJMesh::FrontElement3D &e)
{
	if (this != &e)
	{
#if USE_QUADS
		this->numVerts = e.numVerts;
#else
		this->numVerts = 3;
#endif //MJMesh::Vertex *v3,
		this->MJMesh::FrontElement::vertices = this->vertices;

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

#if USE_GUI
void MJMesh::FrontElement3D::drawNormal(Real factor) const
{
	Data::Triangle3D *t = static_cast<Data::Triangle3D *>(this->getShape());

	if (!Data::getState().isInVisibleSpace(t))
	{
		return;
	}

	Data::Vector3D normal = t->normal();

	//normal.normalize();

	Real size = Data::getState().getBox().orientedSize();
	size = 18.0*t->surface()/(size*size/2.0);

	normal.multiply(factor*size*(this->isHighlighted() ? 2.0 : 1.0));

	normal.setColor(this->getR(), this->getG(), this->getB());

    glPointSize(static_cast<GLfloat>(Data::getState().getPointSize()));
    glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	GLboolean lighting = glIsEnabled(GL_LIGHTING);

	if (lighting)
	{
		glDisable(GL_LIGHTING);
	}

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(t->mid3D(), false, true);
#else
		this->explodeAndShrink(t->mid(), false, true);
#endif //#if USE_EXPERIMENTAL_3

		normal.draw();
	glPopMatrix();

	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
}
#endif //#if USE_GUI

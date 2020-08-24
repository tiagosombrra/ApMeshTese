#include "MJMesh/FrontElementSurface.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/Front.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Edge3D.h"
#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Box.h"
#include "Data/Triangle3D.h"
#include "Data/Element.h"
#include "Data/State.h"

using namespace MJMesh;

/*MJMesh::FrontElementSurface::MJMesh::FrontElementSurface(MJMesh::Edge3D *e) :
	MJMesh::FrontElement(e, e->getId())
{

}*/

MJMesh::FrontElementSurface::FrontElementSurface(MJMesh::Vertex *v1, MJMesh::Vertex *v2, ULInt id) :
	MJMesh::FrontElement(
		new MJMesh::Edge3D(v1, v2),
		id)
{
	/*this->vertices.push_back(v1);
	this->vertices.push_back(v2);*/

#if USE_EXPERIMENTAL
	this->numVerts = 2;
	this->MJMesh::FrontElement::vertices = this->vertices;
#else
	this->resize(2);
#endif //#if USE_EXPERIMENTAL

	this->setVertex(0, v1);
	this->setVertex(1, v2);

	v1->add(this);
	v2->add(this);
}

MJMesh::FrontElementSurface::~FrontElementSurface()
{
	/*if (this->getV1())
	{
		this->getV1()->remove(static_cast<Data::Edge3D *>(this->getShape()));
	}

	if (this->getV2())
	{
		this->getV2()->remove(static_cast<Data::Edge3D *>(this->getShape()));
	}*/
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

UInt MJMesh::FrontElementSurface::numSides() const
{
    return 0;
}

UInt MJMesh::FrontElementSurface::numVerticesInSide(UInt /*side*/) const
{
    return 0;
}

Data::Vertex *MJMesh::FrontElementSurface::getVertexInSide(UInt /*vertex*/, UInt /*side*/) const
{
    return NULL;
}

void MJMesh::FrontElementSurface::setNormal(const Data::Vector3D &normal)
{
	this->normal = normal;
}

const Data::Vector3D &MJMesh::FrontElementSurface::getNormal() const
{
	return this->normal;
}

bool MJMesh::FrontElementSurface::intercept(const Data::Element &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElementSurface::intercept(const Data::Element *e) const
{
	return this->intercept(static_cast<const MJMesh::ElementSurface *>(e));
}

bool MJMesh::FrontElementSurface::intercept(const MJMesh::ElementSurface &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElementSurface::intercept(const MJMesh::ElementSurface *e) const
{
	Data::Triangle3D *t = static_cast<Data::Triangle3D *>(e->getShape());
	MJMesh::Edge3D *edge = static_cast<MJMesh::Edge3D *>(this->getShape());

	//Data::Box box1 = edge->box();
	//Data::Box box2 = t->box();

	//if (!box1.intercept(box2))
	//if (!box1.optIntercept(box2))
	if (!edge->box().optIntercept(t->box()))
	{
		return false;
	}

	return ((edge->intercept(static_cast<Data::Point3D *>(t->getP1()), static_cast<Data::Point3D *>(t->getP2()))) ||
			(edge->intercept(static_cast<Data::Point3D *>(t->getP2()), static_cast<Data::Point3D *>(t->getP3()))) ||
			(edge->intercept(static_cast<Data::Point3D *>(t->getP3()), static_cast<Data::Point3D *>(t->getP1()))) ||
			(t->in(static_cast<Data::Point3D *>(edge->getP1()))) ||
			(t->in(static_cast<Data::Point3D *>(edge->getP2()))));
}

bool MJMesh::FrontElementSurface::intercept(const Data::FrontElement *e) const
{
	return this->intercept(static_cast<const MJMesh::FrontElementSurface *>(e));
}

bool MJMesh::FrontElementSurface::intercept(const MJMesh::FrontElementSurface *e) const
{
	return this->intercept(static_cast<MJMesh::Edge3D *>(e->getShape()));
}

bool MJMesh::FrontElementSurface::intercept(const Data::Edge &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElementSurface::intercept(const Data::Edge *e) const
{
	return this->intercept(static_cast<const MJMesh::Edge3D *>(e));
}

bool MJMesh::FrontElementSurface::intercept(const MJMesh::Edge3D &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElementSurface::intercept(const MJMesh::Edge3D *e) const
{
	return static_cast<MJMesh::Edge3D *>(this->getShape())->intercept(e);
}

bool MJMesh::FrontElementSurface::intercept(const Data::Point &p) const
{
	return this->intercept(&p);
}

bool MJMesh::FrontElementSurface::intercept(const Data::Point *p) const
{
	return (this->distance(p) < Data::getTolerance());


	//return (this->distance(p)/this->getShape()->size() < Shape::getTolerance()*50.0);
	///*

	Real size = this->getShape()->size();

	if (size < 1.0)
	{
		return (this->distance(p)/size < Data::getTolerance()*50.0);
		//return (this->distance(p)*size < Data::getTolerance()*50.0);
	}

	return (this->distance(p) < Data::getTolerance()*50.0);
	//*/

	//return this->distance(p) < Data::getTolerance()*this->getShape()->size();
}

#if USE_EXACT
bool MJMesh::FrontElementSurface::eintercept(const Data::FrontElement *e) const
{
	return this->eintercept(static_cast<const MJMesh::FrontElementSurface *>(e));
}

bool MJMesh::FrontElementSurface::eintercept(const MJMesh::FrontElementSurface *e) const
{
	return this->eintercept(static_cast<MJMesh::Edge3D *>(e->getShape()));
}

bool MJMesh::FrontElementSurface::eintercept(const Data::Edge &e) const
{
	return this->eintercept(&e);
}

bool MJMesh::FrontElementSurface::eintercept(const Data::Edge *e) const
{
	return this->eintercept(static_cast<const MJMesh::Edge3D *>(e));
}

bool MJMesh::FrontElementSurface::eintercept(const MJMesh::Edge3D &e) const
{
	return this->eintercept(&e);
}

bool MJMesh::FrontElementSurface::eintercept(const MJMesh::Edge3D *e) const
{
	return static_cast<MJMesh::Edge3D *>(this->getShape())->eintercept(e);
}
#endif //#if USE_EXACT

bool MJMesh::FrontElementSurface::accordingToNormal(Data::Point *p, bool insideTest) const
{
#if USE_EXPERIMENTAL_3
	Data::Vector3D v(this->getShape()->mid3D(), *p);
#else
	Data::Vector3D v(this->getShape()->mid(), p);
#endif //#if USE_EXPERIMENTAL_3

	v.normalize();

	return (insideTest) ? (this->normal.dot(v) > -Data::getTolerance()) : (this->normal.dot(v) > Data::getTolerance());
}

bool MJMesh::FrontElementSurface::have(const Data::Edge &e) const
{
	return this->have(&e);
}

bool MJMesh::FrontElementSurface::have(const Data::Edge *e) const
{
	return this->have(static_cast<const MJMesh::Edge3D *>(e));
}

bool MJMesh::FrontElementSurface::have(const MJMesh::Edge3D &e) const
{
	return this->have(&e);
}

bool MJMesh::FrontElementSurface::have(const MJMesh::Edge3D *e) const
{
	return static_cast<MJMesh::Edge3D *>(this->getShape())->equal(e);
}

bool MJMesh::FrontElementSurface::isAdjacent(const Data::Element &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::FrontElementSurface::isAdjacent(const Data::Element *e) const
{
	return this->isAdjacent(static_cast<const MJMesh::ElementSurface *>(e));
}

bool MJMesh::FrontElementSurface::isAdjacent(const MJMesh::ElementSurface &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::FrontElementSurface::isAdjacent(const MJMesh::ElementSurface *e) const
{
	Data::Triangle3D *t = static_cast<Data::Triangle3D *>(e->getShape());
	MJMesh::Edge3D *edge = static_cast<MJMesh::Edge3D *>(this->getShape());

	return ((t->have(edge->getP1())) &&
			(t->have(edge->getP2())));
}

Real MJMesh::FrontElementSurface::angle(const Data::Point *p) const
{
	//return static_cast<MJMesh::Edge3D *>(this->shape)->angle(Data::Point3D(p));
	return static_cast<MJMesh::Edge3D *>(this->shape)->angle(static_cast<const Data::Point3D *>(p));
}

Real MJMesh::FrontElementSurface::distance(const Data::Point *p) const
{
	//return static_cast<MJMesh::Edge3D *>(this->shape)->distance(Data::Point3D(p));
	return static_cast<MJMesh::Edge3D *>(this->shape)->distance(static_cast<const Data::Point3D *>(p));
}

Real MJMesh::FrontElementSurface::straightDistance(const Data::Point *p) const
{
	return static_cast<MJMesh::Edge3D *>(this->shape)->straightDistance(static_cast<const Data::Point3D *>(p));
}

#if USE_ATTRIBS
void MJMesh::FrontElementSurface::invert()
{
	/*if (this->getId() <= 72)
	{
		std::cout << "inverting fel " << this->getId() << std::endl;
	}*/

	MJMesh::FrontElement::invert();

	static_cast<MJMesh::Edge3D *>(this->getShape())->invertNormal();
}
#endif //#if USE_ATTRIBS

Data::Element *MJMesh::FrontElementSurface::advance(Data::Vertex *v, const Data::Front *f,
	Data::FrontElementList &newFront, Data::FrontElementList &oldFront) const
{
	return this->advance(static_cast<MJMesh::Vertex *>(v), static_cast<const MJMesh::Front *>(f), newFront, oldFront);
}

bool MJMesh::FrontElementSurface::match(const Data::VertexVector &vertices) const
{
    return (((this->getV1() == vertices[0]) && (this->getV2() == vertices[1])) ||
			((this->getV1() == vertices[1]) && (this->getV2() == vertices[0])));
}

bool MJMesh::FrontElementSurface::geometricallyMatch(const Data::VertexVector &vertices) const
{
	return static_cast<MJMesh::Edge3D *>(this->getShape())->match(
		vertices[0]->getPoint(), vertices[1]->getPoint());

	/*MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(vertices[0]);
	MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(vertices[1]);

	MJMesh::Edge3D *e = static_cast<MJMesh::Edge3D *>(this->getShape());

	return e->match(v1->getPoint(), v2->getPoint());*/
}

MJMesh::ElementSurface *MJMesh::FrontElementSurface::advance(MJMesh::Vertex *v, const MJMesh::Front *f,
	Data::FrontElementList &newFront, Data::FrontElementList &oldFront) const
{
	MJMesh::ElementSurface *element = new MJMesh::ElementSurface(this->getV1(), this->getV2(), v);

	this->add(element);

	Data::VertexVector vertices(2, static_cast<Data::Vertex *>(NULL));

	vertices[0] = this->getV1(); //vertices.push_back(this->getV1());
	vertices[1] = v; //vertices.push_back(v);

	MJMesh::FrontElementSurface *fel1 = static_cast<MJMesh::FrontElementSurface *>(f->find(vertices, this->getV2()));

	vertices[0] = this->getV2();

	MJMesh::FrontElementSurface *fel2 = static_cast<MJMesh::FrontElementSurface *>(f->find(vertices, this->getV1()));

	if (fel1)
	{
		oldFront.push_back(fel1);
	}
	else
	{
		fel1 = new MJMesh::FrontElementSurface(this->getV1(), v);

		newFront.push_back(fel1);
	}

	if (fel2)
	{
		oldFront.push_back(fel2);
	}
	else
	{
		fel2 = new MJMesh::FrontElementSurface(v, this->getV2());

		newFront.push_back(fel2);
	}

	fel1->add(element);
	fel2->add(element);

	return element;
}

#if USE_EXPERIMENTAL
MJMesh::FrontElementSurface &MJMesh::FrontElementSurface::operator=(const MJMesh::FrontElementSurface &e)
{
	if (this != &e)
	{
		this->numVerts = 2;
		this->MJMesh::FrontElement::vertices = this->vertices;

		this->shape = e.shape;

		this->vertices[0] = e.vertices[0];
		this->vertices[1] = e.vertices[1];
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL

#if USE_GUI
void MJMesh::FrontElementSurface::drawNormal(Real factor) const
{
	MJMesh::Edge3D *e = static_cast<MJMesh::Edge3D *>(this->getShape());

	if (!Data::getState().isInVisibleSpace(e))
	{
		return;
	}

	Data::Vector3D normal = this->normal;

	//normal.normalize();

	Real size = Data::getState().getBox().orientedSize();
	size = 6.0*e->length()/size;

	normal.multiply(factor*size*(this->isHighlighted() ? 2.0 : 1.0));

	normal.setColor(this->getR(), this->getG(), this->getB());

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));
	glPointSize(static_cast<GLfloat>(Data::getState().getPointSize(this->isHighlighted())));

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(e->mid3D(), false, true);
#else
		this->explodeAndShrink(e->mid(), false, true);
#endif //#if USE_EXPERIMENTAL_3

		normal.draw();
	glPopMatrix();
}
#endif //#if USE_GUI

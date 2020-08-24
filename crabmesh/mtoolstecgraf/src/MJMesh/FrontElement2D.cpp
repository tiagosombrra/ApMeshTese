#include "MJMesh/FrontElement2D.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/Front.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Edge2D.h"
#include "Data/Point2D.h"
#include "Data/Vector2D.h"
#include "Data/Box.h"
#include "Data/Triangle2D.h"
#include "Data/Element.h"
#include "Data/State.h"

using namespace MJMesh;

/*MJMesh::FrontElement2D::MJMesh::FrontElement2D(MJMesh::Edge2D *e) :
	MJMesh::FrontElement(e, e->getId())
{

}*/

MJMesh::FrontElement2D::FrontElement2D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, ULInt id) :
	MJMesh::FrontElement(
		new MJMesh::Edge2D(v1, v2),
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

MJMesh::FrontElement2D::~FrontElement2D()
{
	/*if (this->getV1())
	{
		this->getV1()->remove(static_cast<Data::Edge2D *>(this->getShape()));
	}

	if (this->getV2())
	{
		this->getV2()->remove(static_cast<Data::Edge2D *>(this->getShape()));
	}*/

	/*if (this->getId() == 1061)
    {
        std::cout << "deleting fel " << this->text() << " at " << this << ", backtrace: " << std::endl;
        std::cout << Data::trace() << std::endl;
    }*/

#if USE_EXPERIMENTAL
	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i])
		{
		    //std::cout << "removing from vertex at " << this->vertices[i] << std::endl;

			static_cast<MJMesh::Vertex *>(this->vertices[i])->remove(this);
		}
	}

	this->numVerts = 0;
	this->MJMesh::FrontElement::vertices = NULL;
#endif //#if USE_EXPERIMENTAL
}

UInt MJMesh::FrontElement2D::numSides() const
{
    return 0;
}

UInt MJMesh::FrontElement2D::numVerticesInSide(UInt /*side*/) const
{
    return 0;
}

Data::Vertex *MJMesh::FrontElement2D::getVertexInSide(UInt /*vertex*/, UInt /*side*/) const
{
    return NULL;
}

bool MJMesh::FrontElement2D::intercept(const Data::Element &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement2D::intercept(const Data::Element *e) const
{
	return this->intercept(static_cast<const MJMesh::Element2D *>(e));
}

bool MJMesh::FrontElement2D::intercept(const MJMesh::Element2D &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement2D::intercept(const MJMesh::Element2D *e) const
{
	Data::Triangle2D *t = static_cast<Data::Triangle2D *>(e->getShape());
	MJMesh::Edge2D *edge = static_cast<MJMesh::Edge2D *>(this->getShape());

	//Data::Box box1 = edge->box();
	//Data::Box box2 = t->box();

	//if (!box1.intercept(box2))
	//if (!box1.optIntercept(box2))
	if (!edge->box().optIntercept(t->box()))
	{
		return false;
	}

	return ((edge->intercept(static_cast<Data::Point2D *>(t->getP1()), static_cast<Data::Point2D *>(t->getP2()))) ||
			(edge->intercept(static_cast<Data::Point2D *>(t->getP2()), static_cast<Data::Point2D *>(t->getP3()))) ||
			(edge->intercept(static_cast<Data::Point2D *>(t->getP3()), static_cast<Data::Point2D *>(t->getP1()))) ||
			(t->in(static_cast<Data::Point2D *>(edge->getP1()))) ||
			(t->in(static_cast<Data::Point2D *>(edge->getP2()))));
}

bool MJMesh::FrontElement2D::intercept(const Data::FrontElement *e) const
{
	return this->intercept(static_cast<const MJMesh::FrontElement2D *>(e));
}

bool MJMesh::FrontElement2D::intercept(const MJMesh::FrontElement2D *e) const
{
	return this->intercept(static_cast<MJMesh::Edge2D *>(e->getShape()));
}

bool MJMesh::FrontElement2D::intercept(const Data::Edge &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement2D::intercept(const Data::Edge *e) const
{
	return this->intercept(static_cast<const MJMesh::Edge2D *>(e));
}

bool MJMesh::FrontElement2D::intercept(const MJMesh::Edge2D &e) const
{
	return this->intercept(&e);
}

bool MJMesh::FrontElement2D::intercept(const MJMesh::Edge2D *e) const
{
	return static_cast<MJMesh::Edge2D *>(this->getShape())->intercept(e);
}

bool MJMesh::FrontElement2D::intercept(const Data::Point &p) const
{
	return this->intercept(&p);
}

bool MJMesh::FrontElement2D::intercept(const Data::Point *p) const
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
bool MJMesh::FrontElement2D::eintercept(const Data::FrontElement *e) const
{
	return this->eintercept(static_cast<const MJMesh::FrontElement2D *>(e));
}

bool MJMesh::FrontElement2D::eintercept(const MJMesh::FrontElement2D *e) const
{
	return this->eintercept(static_cast<MJMesh::Edge2D *>(e->getShape()));
}

bool MJMesh::FrontElement2D::eintercept(const Data::Edge &e) const
{
	return this->eintercept(&e);
}

bool MJMesh::FrontElement2D::eintercept(const Data::Edge *e) const
{
	return this->eintercept(static_cast<const MJMesh::Edge2D *>(e));
}

bool MJMesh::FrontElement2D::eintercept(const MJMesh::Edge2D &e) const
{
	return this->eintercept(&e);
}

bool MJMesh::FrontElement2D::eintercept(const MJMesh::Edge2D *e) const
{
	return static_cast<MJMesh::Edge2D *>(this->getShape())->eintercept(e);
}
#endif //#if USE_EXACT

bool MJMesh::FrontElement2D::accordingToNormal(Data::Point *p, bool insideTest) const
{
	return static_cast<MJMesh::Edge2D *>(this->getShape())->accordingToNormal(static_cast<Data::Point2D *>(p), insideTest);
}

bool MJMesh::FrontElement2D::have(const Data::Edge &e) const
{
	return this->have(&e);
}

bool MJMesh::FrontElement2D::have(const Data::Edge *e) const
{
	return this->have(static_cast<const MJMesh::Edge2D *>(e));
}

bool MJMesh::FrontElement2D::have(const MJMesh::Edge2D &e) const
{
	return this->have(&e);
}

bool MJMesh::FrontElement2D::have(const MJMesh::Edge2D *e) const
{
	return static_cast<MJMesh::Edge2D *>(this->getShape())->equal(e);
}

bool MJMesh::FrontElement2D::isAdjacent(const Data::Element &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::FrontElement2D::isAdjacent(const Data::Element *e) const
{
	return this->isAdjacent(static_cast<const MJMesh::Element2D *>(e));
}

bool MJMesh::FrontElement2D::isAdjacent(const MJMesh::Element2D &e) const
{
	return this->isAdjacent(&e);
}

bool MJMesh::FrontElement2D::isAdjacent(const MJMesh::Element2D *e) const
{
	Data::Triangle2D *t = static_cast<Data::Triangle2D *>(e->getShape());
	MJMesh::Edge2D *edge = static_cast<MJMesh::Edge2D *>(this->getShape());

	return ((t->have(edge->getP1())) &&
			(t->have(edge->getP2())));
}

Real MJMesh::FrontElement2D::angle(const Data::Point *p) const
{
	//return static_cast<MJMesh::Edge2D *>(this->shape)->angle(Data::Point2D(p));
	return static_cast<MJMesh::Edge2D *>(this->shape)->angle(static_cast<const Data::Point2D *>(p));
}

Real MJMesh::FrontElement2D::distance(const Data::Point *p) const
{
	//return static_cast<MJMesh::Edge2D *>(this->shape)->distance(Data::Point2D(p));
	return static_cast<MJMesh::Edge2D *>(this->shape)->distance(static_cast<const Data::Point2D *>(p));
}

Real MJMesh::FrontElement2D::straightDistance(const Data::Point *p) const
{
	return static_cast<MJMesh::Edge2D *>(this->shape)->straightDistance(static_cast<const Data::Point2D *>(p));
}

#if USE_ATTRIBS
void MJMesh::FrontElement2D::invert()
{
	/*if (this->getId() <= 72)
	{
		std::cout << "inverting fel " << this->getId() << std::endl;
	}*/

	MJMesh::FrontElement::invert();

	static_cast<MJMesh::Edge2D *>(this->getShape())->invertNormal();
}
#endif //#if USE_ATTRIBS

Data::Element *MJMesh::FrontElement2D::advance(Data::Vertex *v, const Data::Front *f,
	Data::FrontElementList &newFront, Data::FrontElementList &oldFront) const
{
	return this->advance(static_cast<MJMesh::Vertex *>(v), static_cast<const MJMesh::Front *>(f), newFront, oldFront);
}

bool MJMesh::FrontElement2D::match(const Data::VertexVector &vertices) const
{
    return (((this->getV1() == vertices[0]) && (this->getV2() == vertices[1])) ||
			((this->getV1() == vertices[1]) && (this->getV2() == vertices[0])));
}

bool MJMesh::FrontElement2D::geometricallyMatch(const Data::VertexVector &vertices) const
{
	return static_cast<MJMesh::Edge2D *>(this->getShape())->match(
		vertices[0]->getPoint(), vertices[1]->getPoint());

	/*MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(vertices[0]);
	MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(vertices[1]);

	MJMesh::Edge2D *e = static_cast<MJMesh::Edge2D *>(this->getShape());

	return e->match(v1->getPoint(), v2->getPoint());*/
}

MJMesh::Element2D *MJMesh::FrontElement2D::advance(MJMesh::Vertex *v, const MJMesh::Front *f,
	Data::FrontElementList &newFront, Data::FrontElementList &oldFront) const
{
	MJMesh::Element2D *element = new MJMesh::Element2D(this->getV1(), this->getV2(), v);

	this->add(element);

	Data::VertexVector vertices(2, static_cast<Data::Vertex *>(NULL));

	vertices[0] = this->getV1(); //vertices.push_back(this->getV1());
	vertices[1] = v; //vertices.push_back(v);

	MJMesh::FrontElement2D *fel1 = static_cast<MJMesh::FrontElement2D *>(f->find(vertices, this->getV2()));

	vertices[0] = this->getV2();

	MJMesh::FrontElement2D *fel2 = static_cast<MJMesh::FrontElement2D *>(f->find(vertices, this->getV1()));

	if (fel1)
	{
		oldFront.push_back(fel1);
	}
	else
	{
		fel1 = new MJMesh::FrontElement2D(this->getV1(), v);

		newFront.push_back(fel1);
	}

	if (fel2)
	{
		oldFront.push_back(fel2);
	}
	else
	{
		fel2 = new MJMesh::FrontElement2D(v, this->getV2());

		newFront.push_back(fel2);
	}

	fel1->add(element);
	fel2->add(element);

	return element;
}

#if USE_EXPERIMENTAL
MJMesh::FrontElement2D &MJMesh::FrontElement2D::operator=(const MJMesh::FrontElement2D &e)
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
void MJMesh::FrontElement2D::drawNormal(Real factor) const
{
	MJMesh::Edge2D *e = static_cast<MJMesh::Edge2D *>(this->getShape());

	if (!Data::getState().isInVisibleSpace(e))
	{
		return;
	}

	Data::Vector2D normal = e->normal();

	//normal.normalize();

	Real size = Data::getState().getBox().orientedSize();
	size = 6.0*e->length()/size;

	normal.multiply(factor*size*(this->isHighlighted() ? 2.0 : 1.0));

	normal.setColor(this->getR(), this->getG(), this->getB());

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));
	glPointSize(static_cast<GLfloat>(Data::getState().getPointSize(this->isHighlighted())));

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(e->mid2D(), false, true);
#else
		this->explodeAndShrink(e->mid(), false, true);
#endif //#if USE_EXPERIMENTAL_3

		normal.draw();
	glPopMatrix();
}
#endif //#if USE_GUI

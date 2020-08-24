#include "FuncMesh/FrontAdvancer.h"

#include "Data/FrontElement.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Tetrahedron.h"

using namespace FuncMesh;

FuncMesh::FrontAdvancer::FrontAdvancer(const Data::Boundary *boundary) :
	//Performer::FrontAdvancer(boundary)
	MJMesh::FrontAdvancer(0, boundary, NULL, false)
{
	this->fRadius2D = NULL;
	this->fRadius3D = NULL;

	this->fIdeal2D = NULL;
	this->fIdeal3D = NULL;

	//this->fMakeElement = NULL;
}

FuncMesh::FrontAdvancer::~FrontAdvancer()
{

}

void FuncMesh::FrontAdvancer::setRadius(Real (*fRadius)(const Real p1[], const Real p2[]))
{
	this->fRadius2D = fRadius;

	this->dimension = 2;
}

void FuncMesh::FrontAdvancer::setRadius(Real (*fRadius)(const Real p1[], const Real p2[], const Real p3[]))
{
	this->fRadius3D = fRadius;

	this->dimension = 3;
}

void FuncMesh::FrontAdvancer::setIdeal(bool (*fIdeal)(const Real p1[], const Real p2[], Real ideal[]))
{
	this->fIdeal2D = fIdeal;

	this->dimension = 2;
}

void FuncMesh::FrontAdvancer::setIdeal(bool (*fIdeal)(const Real p1[], const Real p2[], const Real p3[], Real ideal[]))
{
	this->fIdeal3D = fIdeal;

	this->dimension = 3;
}

/*void FuncMesh::FrontAdvancer::setMakeElement(Data::ElementTuple (*fMakeElement)(const Data::FrontElement *, const Data::Point *))
{
	this->fMakeElement = fMakeElement;
}*/

/*Data::ElementTuple FuncMesh::FrontAdvancer::makeElement(const Data::FrontElement *e, const Data::Point *p) const
{
	if (this->fMakeElement)
	{
		return this->fMakeElement(e, p);
	}

	*//*UInt dimension = p->dimension();

	Data::Vertex *v = new Data::Vertex(const_cast<Data::Point *>(p));

	Data::GeometricShape *shape = NULL;

	Data::FrontElementList *fels = new Data::FrontElementList(e->numVertices(), NULL);

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		if (dimension == 2)
		{
			shape = new Data::Edge2D();
		}
		else
		{
			shape = new Data::Triangle3D();
		}

		Data::FrontElement *fel = new Data::FrontElement(shape);

		for (UInt j = 0; j < fel->numVertices() - 1; j++)
		{
			fel->setVertex(j, e->getVertex((i + j)%e->numVertices()));
		}

		fel->setVertex(fel->numVertices() - 1, v);

		//inverts the last edge, so that its normal points towards the
		//  outside region of the triangular element
		if ((dimension == 2) && (i == 1))
		{
			fel->invert();
		}

		fels->push_back(fel);

		shape = NULL;
	}

	if (dimension == 2)
	{
		shape = new Data::Triangle2D();
	}
	else if (dimension == 3)
	{
		shape = new Data::Tetrahedron();
	}

	Data::Element *el = new Data::Element(shape);

	for (UInt i = 0; i < el->numVertices() - 1; i++)
	{
		el->setVertex(i, e->getVertex(i));
	}

	el->setVertex(el->numVertices() - 1, v);

#if USE_C__11
	return std::make_tuple(v, fels, el);
#else
	Data::ElementTuple t;

	t.v = v;
	t.fes = fels;
	t.e = el;

	return t;
#endif //#if USE_C__11*//*

#if USE_C__11
	return Data::ElementTuple();
#else
	Data::ElementTuple t;

	t.v = NULL;
	t.fes = NULL;
	t.e = NULL;

	return t;
#endif //#if USE_C__11
}*/

Real FuncMesh::FrontAdvancer::radius(const Data::FrontElement *element) const
{
	if (this->dimension == 2)
	{
		/*Real x0 = element->getVertex(0)->getPoint()->getCoord(0);
		Real y0 = element->getVertex(0)->getPoint()->getCoord(1);

		Real x1 = element->getVertex(1)->getPoint()->getCoord(0);
		Real y1 = element->getVertex(1)->getPoint()->getCoord(1);

		return this->fRadius2D(x0, y0, x1, y1);*/
		return this->fRadius2D(
					element->getVertex(0)->getPoint()->getCoordsArray(),
					element->getVertex(1)->getPoint()->getCoordsArray());
	}

	/*Real x0 = element->getVertex(0)->getPoint()->getCoord(0);
	Real y0 = element->getVertex(0)->getPoint()->getCoord(1);
	Real z0 = element->getVertex(0)->getPoint()->getCoord(2);

	Real x1 = element->getVertex(1)->getPoint()->getCoord(0);
	Real y1 = element->getVertex(1)->getPoint()->getCoord(1);
	Real z1 = element->getVertex(1)->getPoint()->getCoord(2);

	Real x2 = element->getVertex(2)->getPoint()->getCoord(0);
	Real y2 = element->getVertex(2)->getPoint()->getCoord(1);
	Real z2 = element->getVertex(2)->getPoint()->getCoord(2);

	return this->fRadius3D(x0, y0, z0, x1, y1, z1, x2, y2, z2);*/
	return this->fRadius3D(
					element->getVertex(0)->getPoint()->getCoordsArray(),
					element->getVertex(1)->getPoint()->getCoordsArray(),
					element->getVertex(2)->getPoint()->getCoordsArray());
}

Data::PointList FuncMesh::FrontAdvancer::ideal(const Data::FrontElement *element, Real /*factor*/) const
{
	Data::PointList pts;

	if (this->dimension == 2)
	{
		/*Real x0 = element->getVertex(0)->getPoint()->getCoord(0);
		Real y0 = element->getVertex(0)->getPoint()->getCoord(1);

		Real x1 = element->getVertex(1)->getPoint()->getCoord(0);
		Real y1 = element->getVertex(1)->getPoint()->getCoord(1);

		Real x, y;
		x = y = 0;*/

		Real ideal[] = {0.0, 0.0};

		//if (this->fIdeal2D(x0, y0, x1, y1, &x, &y))
		if (this->fIdeal2D(
					&element->getVertex(0)->getPoint()->getCoords()[0],
					&element->getVertex(1)->getPoint()->getCoords()[0],
					ideal))
		{
			//pts.push_back(new Data::Point2D(x, y));
			//pts[0] = new Data::Point2D(ideal[0], ideal[1]);
			pts.push_back(new Data::Point2D(ideal[0], ideal[1]));
		}

		return pts;
	}

	/*Real x0 = element->getVertex(0)->getPoint()->getCoord(0);
	Real y0 = element->getVertex(0)->getPoint()->getCoord(1);
	Real z0 = element->getVertex(0)->getPoint()->getCoord(2);

	Real x1 = element->getVertex(1)->getPoint()->getCoord(0);
	Real y1 = element->getVertex(1)->getPoint()->getCoord(1);
	Real z1 = element->getVertex(1)->getPoint()->getCoord(2);

	Real x2 = element->getVertex(2)->getPoint()->getCoord(0);
	Real y2 = element->getVertex(2)->getPoint()->getCoord(1);
	Real z2 = element->getVertex(2)->getPoint()->getCoord(2);

	Real x, y, z;
	x = y = z = 0.0;*/

	Real ideal[] = {0.0, 0.0, 0.0};

	//if (this->fIdeal3D(x0, y0, z0, x1, y1, z1, x2, y2, z2, &x, &y, &z))
	if (this->fIdeal3D(
					&element->getVertex(0)->getPoint()->getCoords()[0],
					&element->getVertex(1)->getPoint()->getCoords()[0],
					&element->getVertex(2)->getPoint()->getCoords()[0],
					ideal))
	{
		//pts.push_back(new Data::Point3D(x, y, z));
		//pts[0] = new Data::Point3D(ideal[0], ideal[1], ideal[2]);
		pts.push_back(new Data::Point3D(ideal[0], ideal[1], ideal[2]));
	}

	return pts;
}

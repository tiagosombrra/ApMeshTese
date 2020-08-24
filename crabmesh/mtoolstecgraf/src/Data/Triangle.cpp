#include "Data/Triangle.h"

#include "Data/Point2D.h"
#include "Data/Vector2D.h"
#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Edge.h"

#include "Data/Identifiable.h"
#include "Data/State.h"

using namespace Data;

Data::Triangle::Triangle(Point *p1, Point *p2, Point *p3) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	Polygon()
#else
	Polygon(3)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 3;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	this->setPoints(p1, p2, p3);
}

Data::Triangle::Triangle(const Point& p1, const Point &p2, const Point &p3) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	Polygon()
#else
	Polygon(3)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 3;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_EXPERIMENTAL
	UInt dim = p1.dimension();
	this->setPoints(
        (dim == 2) ? static_cast<Point *>(new Point2D(p1)) : static_cast<Point *>(new Point3D(p1)),
        (dim == 2) ? static_cast<Point *>(new Point2D(p2)) : static_cast<Point *>(new Point3D(p2)),
        (dim == 2) ? static_cast<Point *>(new Point2D(p3)) : static_cast<Point *>(new Point3D(p3)));
#else
	this->setPoints(new Point(p1), new Point(p2), new Point(p3));
#endif //#if USE_EXPERIMENTAL
}

Data::Triangle::~Triangle()
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 0;
	this->GeometricShape::points = NULL;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if USE_EXPERIMENTAL_4
void Data::Triangle::setPoint(UInt i, Point *p)
{
    this->points[i] = p;
}

Point *Data::Triangle::getPoint(UInt i) const
{
    return this->points[i];
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Triangle::numPoints() const
{
	return 3;
}

void Data::Triangle::setPoints(Point *p1, Point *p2, Point *p3)
{
	this->setP1(p1);
	this->setP2(p2);
	this->setP3(p3);
}

void Data::Triangle::setP1(Point *p)
{
	//this->setPoint(0, p);
	this->points[0] = p;
}

Point *Data::Triangle::getP1() const
{
	//return (this->numPoints() > 0) ? this->getPoint(0) : NULL;
	return this->points[0];
}

void Data::Triangle::setP2(Point *p)
{
	//this->setPoint(1, p);
	this->points[1] = p;
}

Point *Data::Triangle::getP2() const
{
	//return (this->numPoints() > 1) ? this->getPoint(1) : NULL;
	return this->points[1];
}

void Data::Triangle::setP3(Point *p)
{
	//this->setPoint(2, p);
	this->points[2] = p;
}

Point *Data::Triangle::getP3() const
{
	//return (this->numPoints() > 2) ? this->getPoint(2) : NULL;
	return this->points[2];
}

UInt Data::Triangle::numSides() const
{
    return 3;
}

#if USE_EXPERIMENTAL
UInt Data::Triangle::dimension() const
{
	return this->points[0] ? this->points[0]->dimension() : 0;
}
#endif //#if USE_EXPERIMENTAL

Real Data::Triangle::circumradius() const
{
	Real a = this->getP1()->distance(this->getP2());
	Real b = this->getP2()->distance(this->getP3());
	Real c = this->getP3()->distance(this->getP1());

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	return 0.25*a*b*c/sqrt(s*d);
}

Real Data::Triangle::inradius() const
{
	Real a = this->getP1()->distance(this->getP2());
	Real b = this->getP2()->distance(this->getP3());
	Real c = this->getP3()->distance(this->getP1());

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	return std::sqrt(d/s);
}

Real Data::Triangle::gamma() const
{
	Real s = this->surface();

	if (s < std::pow(Data::getTolerance(), this->dimension()))
	{
		return 0.0;
	}

	Real d1 = this->getP1()->distance(this->getP2());
    Real d2 = this->getP2()->distance(this->getP3());
    Real d3 = this->getP3()->distance(this->getP1());

    return (d1*d1 + d2*d2 + d3*d3)/(3.0*s);
    //return std::pow(std::sqrt((d1*d1 + d2*d2 + d3*d3)/3.0), 2.0)/s;
}

bool Data::Triangle::match(const Triangle &t) const
{
	return this->match(*(t.getP1()), *(t.getP2()), *(t.getP3()));
}

bool Data::Triangle::match(const Triangle *t) const
{
	return this->match(t->getP1(), t->getP2(), t->getP3());
}

bool Data::Triangle::match(const Point &p1, const Point &p2, const Point &p3) const
{
	return (((this->getP1()->match(p1)) && (this->getP2()->match(p2)) && this->getP3()->match(p3)) ||
			((this->getP1()->match(p1)) && (this->getP2()->match(p3)) && this->getP3()->match(p2)) ||
			((this->getP1()->match(p2)) && (this->getP2()->match(p1)) && this->getP3()->match(p3)) ||
			((this->getP1()->match(p2)) && (this->getP2()->match(p3)) && this->getP3()->match(p1)) ||
			((this->getP1()->match(p3)) && (this->getP2()->match(p2)) && this->getP3()->match(p1)) ||
			((this->getP1()->match(p3)) && (this->getP2()->match(p1)) && this->getP3()->match(p2)));
}

bool Data::Triangle::match(const Point *p1, const Point *p2, const Point *p3) const
{
	return (((this->getP1() == p1) && (this->getP2() == p2) && this->getP3() == p3) ||
			((this->getP1() == p1) && (this->getP2() == p3) && this->getP3() == p2) ||
			((this->getP1() == p2) && (this->getP2() == p1) && this->getP3() == p3) ||
			((this->getP1() == p2) && (this->getP2() == p3) && this->getP3() == p1) ||
			((this->getP1() == p3) && (this->getP2() == p2) && this->getP3() == p1) ||
			((this->getP1() == p3) && (this->getP2() == p1) && this->getP3() == p2));
}

#if USE_ARRAY
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
Triangle &Data::Triangle::operator=(const Triangle &t)
{
	if (this != &t)
	{
#if (!USE_EXPERIMENTAL_4)
		this->numPts = 3;
		this->GeometricShape::points = this->points;
#endif //#if (!USE_EXPERIMENTAL_4)

		UInt dim = t.dimension();

		this->points[0] = (!t.points[0]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[0])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[0])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[0])) : new Point(t.points[0])));
#endif //#if USE_EXPERIMENTAL_3
		this->points[1] = (!t.points[1]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[1])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[1])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[1])) : new Point(t.points[1])));
#endif //#if USE_EXPERIMENTAL_3
		this->points[2] = (!t.points[2]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[2])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[2])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[2])) : new Point(t.points[2])));
#endif //#if USE_EXPERIMENTAL_3
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
bool Data::Triangle::isInVisibleSpace() const
{
	return (!this->isBoxInVisibleSpace()) ? false :
			((Data::getState().isPlaneInclusive()) ? this->isPointInVisibleSpace() : this->arePointsInVisibleSpace());
}
#endif //#if USE_GUI

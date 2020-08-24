#include "Data/Spline/C1CubicBezierCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"
#include "Data/Spline/BezierSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::C1CubicBezierCurve::C1CubicBezierCurve() :
	Data::Spline::CubicBezierCurve::CubicBezierCurve()
{

}

Data::Spline::C1CubicBezierCurve::~C1CubicBezierCurve()
{

}

void Data::Spline::C1CubicBezierCurve::add(Point *p)
{
    Data::Spline::CubicBezierCurve::add(p);

	if ((this->numPoints() > 3) &&
		((this->numPoints() - 1)%3 == 0))
	{
		Point *p1 = this->getPoint(this->numPoints() - 2);
		Point *p2 = this->getPoint(this->numPoints() - 1);

		Point3D p3d1 = this->pt3d(p1);
		Point3D p3d2 = this->pt3d(p2);

		Point3D next = this->findC1Point(p3d1, p3d2);

		this->add(new Point3D(next));
	}
}

void Data::Spline::C1CubicBezierCurve::setPoint(UInt i, Point *p)
{
	Point3D old = this->pt3d(this->getPoint(i));
	Point3D cur = this->pt3d(p);

	Data::Spline::CubicBezierCurve::setPoint(i, p);

	this->correct(i, old, cur);
}

void Data::Spline::C1CubicBezierCurve::move(UInt i, Real x, Real y, Real z)
{
	Point3D old = this->pt3d(this->getPoint(i));
	Point3D cur(x, y, z);

	for (UInt j = 0; j < this->getPoint(i)->dimension(); j++)
	{
		this->getPoint(i)->setCoord(j, cur.getCoord(j));
	}

	this->correct(i, old, cur);
}

void Data::Spline::C1CubicBezierCurve::correct(UInt i, const Point3D &old, const Point3D &cur) const
{
	if (i <= 1)
	{
		return;
	}

	if (i%3 == 0)
	{
		Point3D prev = this->pt3d(this->getPoint(i - 1));

		Vector3D v(prev, old);

		Point3D next = this->findC1Point(cur, v);
		v.multiply(-1);
		prev = this->findC1Point(cur, v);

		for (UInt j = 0; j < this->getPoint(i - 1)->dimension(); j++)
		{
			this->getPoint(i - 1)->setCoord(j, prev.getCoord(j));
		}

		if (i + 1 < this->numPoints())
		{
			for (UInt j = 0; j < this->getPoint(i + 1)->dimension(); j++)
			{
				this->getPoint(i + 1)->setCoord(j, next.getCoord(j));
			}
		}
	}
	else if (i%3 == 1)
	{
		Point3D prev = this->pt3d(this->getPoint(i - 1));

		Point3D prev2 = this->findC1Point(cur, prev);

		for (UInt j = 0; j < this->getPoint(i - 2)->dimension(); j++)
		{
			this->getPoint(i - 2)->setCoord(j, prev2.getCoord(j));
		}
	}
	else if (i%3 == 2)
	{
		if (i + 2 < this->numPoints())
		{
			Point3D next = this->pt3d(this->getPoint(i + 1));

			Point3D next2 = this->findC1Point(cur, next);

			for (UInt j = 0; j < this->getPoint(i + 2)->dimension(); j++)
			{
				this->getPoint(i + 2)->setCoord(j, next2.getCoord(j));
			}
		}
	}
}

Point3D Data::Spline::C1CubicBezierCurve::findC1Point(const Point3D &p1, const Point3D &p2) const
{
	Vector3D v(p1, p2);

	return this->findC1Point(p2, v);
}

Point3D Data::Spline::C1CubicBezierCurve::findC1Point(const Point3D &p, const Vector3D &v) const
{
	Point3D p2(p);

	p2.sum(v);

	return p2;
}

#if USE_GUI
std::string Data::Spline::C1CubicBezierCurve::text() const
{
	std::stringstream s;

	s << "C1" << Data::Spline::CubicBezierCurve::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

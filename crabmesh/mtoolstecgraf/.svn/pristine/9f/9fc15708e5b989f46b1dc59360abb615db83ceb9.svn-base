#include "Data/Spline/C1HermiteCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::C1HermiteCurve::C1HermiteCurve() :
	Data::Spline::HermiteCurve::HermiteCurve()
{

}

Data::Spline::C1HermiteCurve::~C1HermiteCurve()
{

}

void Data::Spline::C1HermiteCurve::add(Point *p)
{
    Data::Spline::HermiteCurve::add(p);

	if ((this->numPoints() > 3) &&
		((this->numPoints() - 1)%3 == 0))
	{
		this->add(new Point3D(p));
	}
}

void Data::Spline::C1HermiteCurve::setPoint(UInt i, Point *p)
{
	Point3D old = this->pt3d(this->getPoint(i));
	Point3D cur = this->pt3d(p);

	Data::Spline::HermiteCurve::setPoint(i, p);

	this->correct(i, old, cur);
}

void Data::Spline::C1HermiteCurve::move(UInt i, Real x, Real y, Real z)
{
	Point3D old = this->pt3d(this->getPoint(i));
	Point3D cur(x, y, z);

	for (UInt j = 0; j < this->getPoint(i)->dimension(); j++)
	{
		this->getPoint(i)->setCoord(j, cur.getCoord(j));
	}

	this->correct(i, old, cur);
}

void Data::Spline::C1HermiteCurve::correct(UInt i, const Point3D &old, const Point3D &cur) const
{
	if (i <= 1)
	{
		return;
	}

	if (i%3 == 2)
	{
		if (i + 1 < this->numPoints())
		{
			Point3D next = this->pt3d(this->getPoint(i + 1));

			Vector3D v(old, cur);

			next.sum(v);

			for (UInt j = 0; j < this->getPoint(i + 1)->dimension(); j++)
			{
				this->getPoint(i + 1)->setCoord(j, next.getCoord(i));
			}

			if (i + 2 < this->numPoints())
			{
				for (UInt j = 0; j < this->getPoint(i + 2)->dimension(); j++)
				{
					this->getPoint(i + 2)->setCoord(j, next.getCoord(i));
				}
			}
		}
	}
	else if (i%3 == 1)
	{
		for (UInt j = 0; j < this->getPoint(i - 1)->dimension(); j++)
		{
			this->getPoint(i - 1)->setCoord(j, cur.getCoord(j));
		}
	}
	else if (i%3 == 0)
	{
		if (i + 1 < this->numPoints())
		{
			for (UInt j = 0; j < this->getPoint(i + 1)->dimension(); j++)
			{
				this->getPoint(i + 1)->setCoord(j, cur.getCoord(j));
			}
		}
	}
}

#if USE_GUI
std::string Data::Spline::C1HermiteCurve::text() const
{
	std::stringstream s;

	s << "C1" << Data::Spline::HermiteCurve::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

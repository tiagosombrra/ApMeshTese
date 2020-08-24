#include "Data/Spline/C1BicubicBezierSurface.h"

#if USE_SPLINES

#include "Data/Point3D.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::C1BicubicBezierSurface::C1BicubicBezierSurface() :
	Data::Spline::BicubicBezierSurface::BicubicBezierSurface()
{

}

Data::Spline::C1BicubicBezierSurface::~C1BicubicBezierSurface()
{

}

UInt Data::Spline::C1BicubicBezierSurface::add(UInt row, Point *p)
{
    UInt col = Data::Spline::BicubicBezierSurface::add(row, p);

	if (((row + 1) > 3) && (row%3 == 0))
	{
		Point *p1 = this->getPoint(row - 1, col);
		Point *p2 = this->getPoint(row, col);

		Point3D p3d1 = this->pt3d(p1);
		Point3D p3d2 = this->pt3d(p2);

		Point3D next = this->findC1Point(p3d1, p3d2);

		if ((this->numPointsV() <= (row + 1)) || (!this->getPoint(row + 1, col)))
		{
			this->add(row + 1, new Point3D(next));
		}
	}

	if (((col + 1) > 3) && (col%3 == 0))
	{
		Point *p1 = this->getPoint(row, col - 1);
		Point *p2 = this->getPoint(row, col);

		Point3D p3d1 = this->pt3d(p1);
		Point3D p3d2 = this->pt3d(p2);

		Point3D next = this->findC1Point(p3d1, p3d2);

		if ((this->numPointsU() <= (col + 1)) || (!this->getPoint(row, col + 1)))
		{
			this->add(row, new Point3D(next));
		}
	}

	return col;
}

void Data::Spline::C1BicubicBezierSurface::setPoint(UInt i, UInt j, Point *p)
{
	Point3D old = this->pt3d(this->getPoint(i, j));
	Point3D cur = this->pt3d(p);

	Data::Spline::BicubicBezierSurface::setPoint(i, j, p);

	this->correct(i, j, old, cur);
}

void Data::Spline::C1BicubicBezierSurface::move(UInt i, UInt j, Real x, Real y, Real z)
{
	Point3D old = this->pt3d(this->getPoint(i, j));
	Point3D cur(x, y, z);

	for (UInt k = 0; k < this->getPoint(i, j)->dimension(); k++)
	{
		this->getPoint(i, j)->setCoord(k, cur.getCoord(k));
	}

	this->correct(i, j, old, cur);
}

void Data::Spline::C1BicubicBezierSurface::correct(UInt i, UInt j, const Point3D &old, const Point3D &cur, bool vdirection) const
{
	if ((vdirection) && (i > 1))
	{
		if (i%3 == 0)
		{
			Point3D prev = this->pt3d(this->getPoint(i - 1, j));
			Point3D oldPrev = prev, oldNext;

			Vector3D v(prev, old);

			Point3D next = this->findC1Point(cur, v);
			v.multiply(-1);
			prev = this->findC1Point(cur, v);

			for (UInt k = 0; k < this->getPoint(i - 1, j)->dimension(); k++)
			{
				this->getPoint(i - 1, j)->setCoord(k, prev.getCoord(k));
			}

			if (i + 1 < this->numPointsV())
			{
				oldNext = this->pt3d(this->getPoint(i + 1, j));

				for (UInt k = 0; k < this->getPoint(i + 1, j)->dimension(); k++)
				{
					this->getPoint(i + 1, j)->setCoord(k, next.getCoord(k));
				}
			}

			this->correct(i - 1, j, oldPrev, prev, false);

			if (i + 1 < this->numPointsV())
			{
				this->correct(i + 1, j, oldNext, next, false);
			}
		}
		else if (i%3 == 1)
		{
			Point3D prev = this->pt3d(this->getPoint(i - 1, j));
			Point3D oldPrev2 = this->pt3d(this->getPoint(i - 2, j));

			Point3D prev2 = this->findC1Point(cur, prev);

			for (UInt k = 0; k < this->getPoint(i - 2, j)->dimension(); k++)
			{
				this->getPoint(i - 2, j)->setCoord(k, prev2.getCoord(k));
			}

			this->correct(i - 2, j, oldPrev2, prev2, false);
		}
		else if (i%3 == 2)
		{
			if (i + 2 < this->numPointsV())
			{
				Point3D next = this->pt3d(this->getPoint(i + 1, j));
				Point3D oldNext2 = this->pt3d(this->getPoint(i + 2, j));

				Point3D next2 = this->findC1Point(cur, next);

				for (UInt k = 0; k < this->getPoint(i + 2, j)->dimension(); k++)
				{
					this->getPoint(i + 2, j)->setCoord(k, next2.getCoord(k));
				}

				this->correct(i + 2, j, oldNext2, next2, false);
			}
		}
	}

	if (j > 1)
	{
		if (j%3 == 0)
		{
			Point3D prev = this->pt3d(this->getPoint(i, j - 1));

			Vector3D v(prev, old);

			Point3D next = this->findC1Point(cur, v);
			v.multiply(-1);
			prev = this->findC1Point(cur, v);

			for (UInt k = 0; k < this->getPoint(i, j - 1)->dimension(); k++)
			{
				this->getPoint(i, j - 1)->setCoord(k, prev.getCoord(k));
			}

			if (j + 1 < this->numPointsU())
			{
				for (UInt k = 0; k < this->getPoint(i, j + 1)->dimension(); k++)
				{
					this->getPoint(i, j + 1)->setCoord(k, next.getCoord(k));
				}
			}
		}
		else if (j%3 == 1)
		{
			Point3D prev = this->pt3d(this->getPoint(i, j - 1));

			Point3D prev2 = this->findC1Point(cur, prev);

			for (UInt k = 0; k < this->getPoint(i, j - 2)->dimension(); k++)
			{
				this->getPoint(i, j - 2)->setCoord(k, prev2.getCoord(k));
			}
		}
		else if (j%3 == 2)
		{
			if (j + 2 < this->numPointsU())
			{
				Point3D next = this->pt3d(this->getPoint(i, j + 1));

				Point3D next2 = this->findC1Point(cur, next);

				for (UInt k = 0; k < this->getPoint(i, j + 2)->dimension(); k++)
				{
					this->getPoint(i, j + 2)->setCoord(k, next2.getCoord(k));
				}
			}
		}
	}
}

Point3D Data::Spline::C1BicubicBezierSurface::findC1Point(const Point3D &p1, const Point3D &p2) const
{
	Vector3D v(p1, p2);

	return this->findC1Point(p2, v);
}

Point3D Data::Spline::C1BicubicBezierSurface::findC1Point(const Point3D &p, const Vector3D &v) const
{
	Point3D p2(p);

	p2.sum(v);

	return p2;
}

#if USE_GUI
std::string Data::Spline::C1BicubicBezierSurface::text() const
{
	std::stringstream s;

	s << "C1" << Data::Spline::BicubicBezierSurface::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

#include "Data/Polygon.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"

using namespace Data;

Data::Polygon::Polygon() :
	GeometricShape()
{

}

#if (!USE_EXPERIMENTAL_4)
Data::Polygon::Polygon(UInt numPoints) :
	GeometricShape(numPoints)
{

}
#endif //#if (!USE_EXPERIMENTAL_4)

Data::Polygon::~Polygon()
{

}

UInt Data::Polygon::numSides() const
{
    return this->numPoints();
}

Real Data::Polygon::perimeter() const
{
	Real d = 0;

	if (this->numPoints() > 2)
	{
		for (UInt i = 0; i < this->numPoints() - 1; i++)
		{
#if USE_EXPERIMENTAL_4
			d += getPoint(i)->distance(this->getPoint(i+1));
#else
			d += this->points[i]->distance(this->points[i+1]);
#endif //#if USE_EXPERIMENTAL_4
		}

#if USE_EXPERIMENTAL_4
		d += this->getPoint(this->numPoints() - 1)->distance(this->getPoint(0));
#else
		d += this->points[this->numPoints() - 1]->distance(this->points[0]);
#endif //#if USE_EXPERIMENTAL_4
	}

	return d;
}

Real Data::Polygon::orientedSize() const
{
	return this->orientedSurface();
}

bool Data::Polygon::match(const GeometricShape &s) const
{
	return this->match(&s);
}

bool Data::Polygon::match(const GeometricShape *s) const
{
	Polygon *p = dynamic_cast<Polygon *>(const_cast<GeometricShape *>(s));

	return this->match(p);
}

bool Data::Polygon::match(const Polygon &p) const
{
	if (this->numPoints() != p.numPoints())
	{
		return false;
	}

	UInt first = 0;

	for (; first < p.numPoints(); first++)
	{
#if USE_EXPERIMENTAL_4
		if ((this->getPoint(0) == p.getPoint(first)) ||
			(this->getPoint(0)->match(p.getPoint(first))))
#else
		if ((this->points[0] == p.points[first]) ||
			(this->points[0]->match(p.points[first])))
#endif //#if USE_EXPERIMENTAL_4
		{
			break;
		}
	}

	if (first == p.numPoints())
	{
		return false;
	}

	for (UInt i = 1; i < this->numPoints(); i++)
	{
		UInt next = (first + i)%p.numPoints();

#if USE_EXPERIMENTAL_4
		if ((this->getPoint(i) != p.getPoint(next)) &&
			(!this->getPoint(i)->match(p.getPoint(next))))
#else
		if ((this->points[i] != p.points[next]) &&
			(!this->points[i]->match(p.points[next])))
#endif //#if USE_EXPERIMENTAL_4
		{
			return false;
		}
	}

	return true;
}

bool Data::Polygon::match(const Polygon *p) const
{
	/*if (!p)
	{
		return false;
	}*/

	return p ? ((this == p) || (this->match(*p))) : false;
}

bool Data::Polygon::equal(const GeometricShape &s) const
{
	return this->equal(&s);
}

bool Data::Polygon::equal(const GeometricShape *s) const
{
	Polygon *p = dynamic_cast<Polygon *>(const_cast<GeometricShape *>(s));

	return this->equal(p);
}

bool Data::Polygon::equal(const Polygon &p) const
{
	if (this->numPoints() != p.numPoints())
	{
		return false;
	}

	UInt first = 0;

	for (; first < p.numPoints(); first++)
	{
#if USE_EXPERIMENTAL_4
		if ((this->getPoint(0) == p.getPoint(first)) ||
			(this->getPoint(0)->equal(p.getPoint(first))))
#else
		if ((this->points[0] == p.points[first]) ||
			(this->points[0]->equal(p.points[first])))
#endif //#if USE_EXPERIMENTAL_4
		{
			break;
		}
	}

	if (first == p.numPoints())
	{
		return false;
	}

	if (this->numPoints() == 1)
	{
		return true;
	}

	UInt incr = 0;

#if USE_EXPERIMENTAL_4
	if ((this->getPoint(1) == p.getPoint((first + 1)%p.numPoints())) ||
		(this->getPoint(1)->equal(p.getPoint((first + 1)%p.numPoints()))))
#else
	if ((this->points[1] == p.points[(first + 1)%p.numPoints()]) ||
		(this->points[1]->equal(p.points[(first + 1)%p.numPoints()])))
#endif //#if USE_EXPERIMENTAL_4
	{
		incr = 1;
	}
#if USE_EXPERIMENTAL_4
	else if ((this->getPoint(1) == p.getPoint((first + p.numPoints() - 1)%p.numPoints())) ||
			 (this->getPoint(1)->equal(p.getPoint((first + p.numPoints() - 1)%p.numPoints()))))
#else
	else if ((this->points[1] == p.points[(first + p.numPoints() - 1)%p.numPoints()]) ||
			 (this->points[1]->equal(p.points[(first + p.numPoints() - 1)%p.numPoints()])))
#endif //#if USE_EXPERIMENTAL_4
	{
		incr = p.numPoints() - 1;
	}

	if (incr == 0)
	{
		return false;
	}

	for (UInt i = 2; i < this->numPoints(); i++)
	{
		UInt next = static_cast<UInt>((static_cast<Int>(first) + static_cast<Int>(i)*incr)%(static_cast<Int>(p.numPoints())));

#if USE_EXPERIMENTAL_4
		if ((this->getPoint(i) != p.getPoint(next)) &&
			(!this->getPoint(i)->equal(p.getPoint(next))))
#else
		if ((this->points[i] != p.points[next]) &&
			(!this->points[i]->equal(p.points[next])))
#endif //#if USE_EXPERIMENTAL_4
		{
			return false;
		}
	}

	return true;
}

bool Data::Polygon::equal(const Polygon *p) const
{
	/*if (!p)
	{
		return false;
	}*/

	return p ? ((this == p) || (this->equal(*p))) : false;
}

Vector3D Data::Polygon::normal() const
{
	if (this->dimension() == 2)
	{
		Point2D p1(this->getPoint(0));
		Point2D p2(this->getPoint(1));
		Point2D p3(this->getPoint(2));

		Vector2D v1(p1, p2);
		Vector2D v2(p1, p3);

		return Vector3D(0.0, 0.0, v1.cross(v2));
	}

	if (this->dimension() == 3)
	{
		Point3D p1(this->getPoint(0));
		Point3D p2(this->getPoint(1));
		Point3D p3(this->getPoint(2));

		Vector3D v1(p1, p2);
		Vector3D v2(p1, p3);

		Vector3D n = v1.cross(v2);

		n.normalize();

		return n;
	}

	return Vector3D();
}

#if USE_EXACT
EVector3D Data::Polygon::enormal() const
{
	if (this->dimension() == 2)
	{
#if USE_EXPERIMENTAL_3
		EPoint2D p1(static_cast<Data::Point2D *>(this->getPoint(0))->epoint2D());
		EPoint2D p2(static_cast<Data::Point2D *>(this->getPoint(1))->epoint2D());
		EPoint2D p3(static_cast<Data::Point2D *>(this->getPoint(2))->epoint2D());
#else
		EPoint2D p1(this->getPoint(0)->epoint());
		EPoint2D p2(this->getPoint(1)->epoint());
		EPoint2D p3(this->getPoint(2)->epoint());
#endif //#if USE_EXPERIMENTAL_3

		EVector2D v1(p1, p2);
		EVector2D v2(p1, p3);

		return EVector3D(0.0, 0.0, v1.cross(v2));
	}

	if (this->dimension() == 3)
	{
#if USE_EXPERIMENTAL_3
		EPoint3D p1(static_cast<Data::Point3D *>(this->getPoint(0))->epoint3D());
		EPoint3D p2(static_cast<Data::Point3D *>(this->getPoint(1))->epoint3D());
		EPoint3D p3(static_cast<Data::Point3D *>(this->getPoint(2))->epoint3D());
#else
		EPoint3D p1(this->getPoint(0)->epoint());
		EPoint3D p2(this->getPoint(1)->epoint());
		EPoint3D p3(this->getPoint(2)->epoint());
#endif //#if USE_EXPERIMENTAL_3

		EVector3D v1(p1, p2);
		EVector3D v2(p1, p3);

		return v1.cross(v2);
	}

	return EVector3D();
}
#endif //#if USE_EXACT

#if USE_GUI
std::string Data::Polygon::text() const
{
	std::stringstream str;

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		str << " ";

		if (this->getPoint(i))
		{
			str << this->getPoint(i)->text();
		}
		else
		{
			str << "-1";
		}
	}

	return str.str();
}
#endif //#if USE_GUI

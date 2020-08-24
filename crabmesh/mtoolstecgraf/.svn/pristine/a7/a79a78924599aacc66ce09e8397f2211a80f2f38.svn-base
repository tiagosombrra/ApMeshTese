#ifndef _DATA_TRIANGLE_H_
#define _DATA_TRIANGLE_H_

#include "Data/Definitions.h"
#include "Data/Polygon.h"

namespace Data
{
	class Triangle : public Data::Polygon
	{
	public:

		Triangle(Point *p1, Point *p2, Point *p3);
		Triangle(const Point &p1, const Point &p2, const Point &p3);
		virtual ~Triangle();

#if USE_EXPERIMENTAL_4
        virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;
#endif //#if USE_EXPERIMENTAL_4

		virtual UInt numPoints() const;

		virtual void setPoints(Point *p1, Point *p2, Point *p3);

		virtual void setP1(Point *p);
		virtual Point *getP1() const;

		virtual void setP2(Point *p);
		virtual Point *getP2() const;

		virtual void setP3(Point *p);
		virtual Point *getP3() const;

#if USE_EXPERIMENTAL
		virtual UInt dimension() const;
#endif //#if USE_EXPERIMENTAL

		virtual UInt numSides() const;

		virtual bool in(const Point &p) const = 0;
		virtual bool in(const Point *p) const = 0;

		virtual bool on(const Point &p) const = 0;
		virtual bool on(const Point *p) const = 0;

		virtual bool out(const Point &p) const = 0;
		virtual bool out(const Point *p) const = 0;

		virtual Real circumradius() const;
#if USE_EXPERIMENTAL_3
		virtual Point2D circumcenter2D() const = 0;
		virtual Point3D circumcenter3D() const = 0;
#else
		virtual Point circumcenter() const = 0;
#endif //#if USE_EXPERIMENTAL_3

		virtual Real inradius() const;
#if USE_EXPERIMENTAL_3
		virtual Point2D incenter2D() const = 0;
		virtual Point3D incenter3D() const = 0;
#else
		virtual Point incenter() const = 0;
#endif //#if USE_EXPERIMENTAL_3

		virtual R3Tuple barycentricCoordinates(const Point &p) const = 0;
		virtual R3Tuple barycentricCoordinates(const Point *p) const = 0;

		virtual Real surface() const = 0;
		virtual Real orientedSurface() const = 0;

		virtual Real gamma() const;

		using Polygon::match;
		virtual bool match(const Triangle &t) const;
		virtual bool match(const Triangle *t) const;
		virtual bool match(const Point &p1, const Point &p2, const Point &p3) const;
		virtual bool match(const Point *p1, const Point *p2, const Point *p3) const;

#if USE_ARRAY
		using Polygon::operator=;
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
		virtual Triangle &operator=(const Triangle &t);
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
		virtual bool isInVisibleSpace() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	protected:

		Point *points[3];
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	};
}

#endif //#ifndef _DATA_TRIANGLE_H_

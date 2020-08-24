#ifndef _DATA_POINT2D_H_
#define _DATA_POINT2D_H_

#include "Data/Definitions.h"
#include "Data/Point.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

namespace Data
{
	class Point2D : public Data::Point
#if USE_POOL
		, public Data::Poolable<Data::Point2D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(Data::Point2D)
#endif //#if USE_POOL

	public:

		Point2D(Real x = 0.0, Real y = 0.0);
		Point2D(const Point2D &p);
		Point2D(const Point &p);
		Point2D(const Point *p);
		Point2D(const Vector &v);
		Point2D(const Vector *v);
		Point2D(const Point &p, const Vector &v);
		Point2D(const Point *p, const Vector *v);
		virtual ~Point2D();

		virtual void setPosition(Real x, Real y);

		virtual void setX(Real x);
		virtual Real getX() const;

		virtual void setY(Real y);
		virtual Real getY() const;

		virtual UInt dimension() const;

#if USE_EXPERIMENTAL_3
        virtual void setCoords(const Real coords[]);
		virtual void setCoords(const RealVector &coords);
		virtual RealVector getCoords() const;
		virtual const Real *getCoordsArray() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
        virtual void setCoord(UInt i, Real c);
		virtual Real getCoord(UInt i) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual Real surface(const Point2D &p1, const Point2D &p2) const;
		virtual Real surface(const Point2D *p1, const Point2D *p2) const;

		virtual Real orientedSurface(const Point2D &p1, const Point2D &p2) const;
		virtual Real orientedSurface(const Point2D *p1, const Point2D *p2) const;

		virtual bool clockWise(const Point2D &p1, const Point2D &p2) const;
		virtual bool clockWise(const Point2D *p1, const Point2D *p2) const;

		virtual bool counterClockWise(const Point2D &p1, const Point2D &p2) const;
		virtual bool counterClockWise(const Point2D *p1, const Point2D *p2) const;

		using Point::match;
		/*virtual bool match(const Point2D &p) const;
		virtual bool match(const Point2D *p) const;*/
		virtual bool match(Real x, Real y) const;

		using Point::equal;
		virtual bool equal(Real x, Real y) const;

#if USE_EXACT
		virtual EPoint2D epoint2D() const;
#endif //#if USE_EXACT

#if USE_ARRAY
		using Point::operator=;
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
		virtual Point2D &operator=(const Point2D &p);
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
#if USE_EXPERIMENTAL_3
        virtual Point2D &operator=(const Point &p);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
		using Point::draw;
		virtual void draw() const;

		using Point::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	protected:

		Real coords[2];
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	};

#if USE_EXACT
	class EPoint2D : public Data::EPoint
//#if USE_POOL
//		, public Data::Poolable<Data::EPoint2D>
//#endif //#if USE_POOL
	{
//#if USE_POOL
//		USING_POOLABLE(Data::EPoint2D)
//#endif //#if USE_POOL

	public:

		EPoint2D(EReal x = 0.0, EReal y = 0.0);
		EPoint2D(const EPoint &p);
		EPoint2D(const EPoint *p);
		EPoint2D(const EVector &v);
		EPoint2D(const EVector *v);
		EPoint2D(const EPoint &p, const EVector &v);
		EPoint2D(const EPoint *p, const EVector *v);
		virtual ~EPoint2D();

		virtual void setPosition(EReal x, EReal y);

		virtual void setX(EReal x);
		virtual EReal getX() const;

		virtual void setY(EReal y);
		virtual EReal getY() const;

		virtual UInt dimension() const;

		virtual EReal surface(const EPoint2D &p1, const EPoint2D &p2) const;
		virtual EReal surface(const EPoint2D *p1, const EPoint2D *p2) const;

		virtual EReal orientedSurface(const EPoint2D &p1, const EPoint2D &p2) const;
		virtual EReal orientedSurface(const EPoint2D *p1, const EPoint2D *p2) const;

		virtual bool clockWise(const EPoint2D &p1, const EPoint2D &p2) const;
		virtual bool clockWise(const EPoint2D *p1, const EPoint2D *p2) const;

		virtual bool counterClockWise(const EPoint2D &p1, const EPoint2D &p2) const;
		virtual bool counterClockWise(const EPoint2D *p1, const EPoint2D *p2) const;

		using EPoint::match;
		/*virtual bool match(const EPoint2D &p) const;
		virtual bool match(const EPoint2D *p) const;*/
		virtual bool match(EReal x, EReal y) const;

		//using EPoint::equal;
		//virtual bool equal(EReal x, EReal y) const;

		virtual Point2D point2D() const;

#if USE_ARRAY
		using EPoint::operator=;
#endif //#if USE_ARRAY

#if USE_GUI
		using EPoint::draw;
		virtual void draw() const;

		using EPoint::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
#endif //#if USE_GUI
	};
#endif //#if USE_EXACT
}

#endif //#ifndef _DATA_POINT2D_H_

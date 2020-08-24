#ifndef _DATA_GEOMETRIC_SHAPE_H_
#define _DATA_GEOMETRIC_SHAPE_H_

#include "Data/Definitions.h"
#include "Data/Point.h"

namespace Data
{
	class GeometricShape
#if USE_GUI
		: public Data::Colorable
#endif //#if USE_GUI
	{
	public:

		GeometricShape();
#if (!USE_EXPERIMENTAL_4)
		GeometricShape(UInt numPoints);
#endif //#if (!USE_EXPERIMENTAL_4)
		virtual ~GeometricShape();

#if USE_EXPERIMENTAL_4
        virtual void setPoint(UInt i, Point *p) = 0;
		virtual Point *getPoint(UInt i) const = 0;
#else
		virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;
#endif //#if USE_EXPERIMENTAL_4

#if (!USE_EXPERIMENTAL_4)
		virtual void add(Point *p);
		virtual void remove(Point *p);
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_EXPERIMENTAL_4
		virtual UInt numPoints() const = 0;
#else
		virtual UInt numPoints() const;
#endif //#if USE_EXPERIMENTAL_4

		virtual UInt dimension() const;

		virtual Real size() const;
		virtual Real orientedSize() const = 0;

#if USE_EXPERIMENTAL_3
		virtual Point2D mid2D() const;
		virtual Point3D mid3D() const;
#else
		virtual Point mid() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual Box box() const;

		virtual bool in(const Point &p) const = 0;
		virtual bool in(const Point *p) const = 0;

		virtual bool on(const Point &p) const = 0;
		virtual bool on(const Point *p) const = 0;

		virtual bool out(const Point &p) const = 0;
		virtual bool out(const Point *p) const = 0;

		virtual bool haveMatching(const Point &p) const;
		virtual bool have(const Point &p) const;
		virtual bool have(const Point *p) const;

		virtual bool match(const GeometricShape &s) const;
		virtual bool match(const GeometricShape *s) const;

		virtual bool equal(const GeometricShape &s) const;
		virtual bool equal(const GeometricShape *s) const;

		virtual bool smaller(const GeometricShape *s) const;
		virtual bool larger(const GeometricShape *s) const;

		virtual bool translate(const Vector &v);
		virtual bool translate(const Vector *v);
		virtual bool scale(const Vector &v);
		virtual bool scale(const Vector *v);
		virtual bool scale(Real s);
		virtual bool localScale(const Vector &v);
		virtual bool localScale(const Vector *v);
		virtual bool localScale(Real s);

#if USE_ARRAY
#if (!USE_EXPERIMENTAL_4)
		virtual GeometricShape &operator=(const GeometricShape &s);
#endif //#if (!USE_EXPERIMENTAL_4)
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void highlight() const;
		virtual void unhighlight() const;

		using Colorable::draw;
		//virtual void draw(bool fill) const = 0;
		virtual void draw() const = 0;

		virtual bool isInVisibleSpace() const;
		virtual bool isBoxInVisibleSpace() const;
		virtual bool arePointsInVisibleSpace() const;
		virtual bool isPointInVisibleSpace() const;
#endif //#if USE_GUI

	protected:

#if (!USE_EXPERIMENTAL_4)
#if USE_ARRAY
		UShort numPts;
		Point **points;
#else
		PointVector points;
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_4)
	};
}

#endif //#ifndef _DATA_GEOMETRIC_SHAPE_H_

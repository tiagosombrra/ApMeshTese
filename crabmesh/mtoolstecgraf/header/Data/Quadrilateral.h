#ifndef _DATA_QUADRILATERAL_H_
#define _DATA_QUADRILATERAL_H_

#include "Data/Definitions.h"
#include "Data/Polygon.h"

namespace Data
{
	class Quadrilateral : public Data::Polygon
	{
	public:

		Quadrilateral(Point *p1, Point *p2, Point *p3, Point *p4);
		Quadrilateral(const Point &p1, const Point &p2, const Point &p3, const Point &p4);
		virtual ~Quadrilateral();

#if USE_EXPERIMENTAL_4
        virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;
#endif //#if USE_EXPERIMENTAL_4

		virtual UInt numPoints() const;

		virtual void setPoints(Point *p1, Point *p2, Point *p3, Point *p4);

		virtual void setP1(Point *p);
		virtual Point *getP1() const;

		virtual void setP2(Point *p);
		virtual Point *getP2() const;

		virtual void setP3(Point *p);
		virtual Point *getP3() const;

		virtual void setP4(Point *p);
		virtual Point *getP4() const;

		virtual UInt numSides() const;

#if USE_EXPERIMENTAL
		virtual UInt dimension() const;
#endif //#if USE_EXPERIMENTAL

		///TODO
		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		///TODO
		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		///TODO
		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual Real surface() const;
		virtual Real orientedSurface() const;

		using Polygon::match;
		virtual bool match(const Quadrilateral &q) const;
		virtual bool match(const Quadrilateral *q) const;
		virtual bool match(const Point &p1, const Point &p2, const Point &p3, const Point &p4) const;
		virtual bool match(const Point *p1, const Point *p2, const Point *p3, const Point *p4) const;

#if USE_ARRAY
		using Polygon::operator=;
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
		virtual Quadrilateral &operator=(const Quadrilateral &q);
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
		using Data::Polygon::draw;
		virtual void draw() const;
		virtual UInt fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const;
		virtual UInt fill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const;
		virtual UInt qfill(GLfloat *coord, GLfloat *color, GLfloat *normal) const;
		virtual UInt qfill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const;

		virtual bool isInVisibleSpace() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	protected:

		Point *points[4];
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	};
}

#endif //#ifndef _DATA_QUADRILATERAL_H_

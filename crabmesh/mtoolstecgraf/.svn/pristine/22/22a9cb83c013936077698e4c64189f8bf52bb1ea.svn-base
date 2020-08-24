#ifndef _DATA_EDGE_H_
#define _DATA_EDGE_H_

#include "Data/Definitions.h"
#include "Data/GeometricShape.h"

namespace Data
{
	class Edge : public Data::GeometricShape
	{
	public:

		Edge(Point *p1 = NULL, Point *p2 = NULL);
		Edge(const Point &p1, const Point &p2);
		virtual ~Edge();

#if USE_EXPERIMENTAL_4
        virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;
#endif //#if USE_EXPERIMENTAL_4

		virtual UInt numPoints() const;

		virtual void setPoints(Point *p1, Point *p2);

		virtual void setP1(Point *p);
		virtual Point *getP1() const;

		virtual void setP2(Point *p);
		virtual Point *getP2() const;

#if USE_EXPERIMENTAL
		virtual UInt dimension() const;
#endif //#if USE_EXPERIMENTAL

		virtual Real length() const;

		virtual Real size() const;
		virtual Real orientedSize() const;

 #if USE_EXPERIMENTAL_3
        virtual Vector2D vector2D() const;
        virtual Vector3D vector3D() const;
 #else
		virtual Vector vector() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual Real height(const Point &p) const;
		virtual Real height(const Point *p) const;

		virtual Real straightDistance(const Point &p) const;
		virtual Real straightDistance(const Point *p) const;

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual Real angle(const Point &p) const;
		virtual Real angle(const Point *p) const;

		using GeometricShape::match;
		virtual bool match(const Edge &e) const;
		virtual bool match(const Edge *e) const;
		virtual bool match(const Point &p1, const Point &p2) const;
		virtual bool match(const Point *p1, const Point *p2) const;

		using GeometricShape::equal;
		virtual bool equal(const Edge &e) const;
		virtual bool equal(const Edge *e) const;
		virtual bool equal(const Point &p1, const Point &p2) const;
		virtual bool equal(const Point *p1, const Point *p2) const;

#if USE_ARRAY
		using GeometricShape::operator=;
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
		virtual Edge &operator=(const Edge &e);
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void highlight() const;
		virtual void unhighlight() const;

		using GeometricShape::draw;
		//virtual void draw(bool fill) const;
		virtual void draw() const;
		using GeometricShape::fill;
		virtual UInt fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const;

		virtual std::string text() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	protected:

		Point *points[2];
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))

	};
}

#endif //#ifndef _DATA_EDGE_H_

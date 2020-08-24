#ifndef _DATA_EDGE2D_H_
#define _DATA_EDGE2D_H_

#include "Data/Definitions.h"
#include "Data/Edge.h"

#include "Data/Vector2D.h"
#include "Data/Point2D.h"

namespace Data
{
	class Edge2D : public Data::Edge
	{
	public:

		Edge2D(Point2D *p1 = NULL, Point2D *p2 = NULL);
		Edge2D(const Point2D &p1, const Point2D &p2);
		virtual ~Edge2D();

		virtual UInt dimension() const;

		virtual bool intercept(const Edge2D &e) const;
		virtual bool intercept(const Edge2D *e) const;
		virtual bool intercept(const Point2D &p1, const Point2D &p2) const;
		virtual bool intercept(const Point2D *p1, const Point2D *p2) const;

#if USE_EXACT
		virtual bool eintercept(const Edge2D &e) const;
		virtual bool eintercept(const Edge2D *e) const;
		virtual bool eintercept(const Point2D &p1, const Point2D &p2) const;
		virtual bool eintercept(const Point2D *p1, const Point2D *p2) const;
#endif //#if USE_EXACT

		virtual Real distance(const Point2D &p) const;
		virtual Real distance(const Point2D *p) const;
		virtual Real distance(Real x, Real y) const;

		virtual Real straightDistance(const Point &p) const;
		virtual Real straightDistance(const Point *p) const;
		virtual Real straightDistance(const Point2D &p) const;
		virtual Real straightDistance(const Point2D *p) const;
		virtual Real straightDistance(Real x, Real y) const;

		virtual Real height(const Point &p) const;
		virtual Real height(const Point *p) const;
		virtual Real height(const Point2D &p) const;
		virtual Real height(const Point2D *p) const;
		virtual Real height(Real x, Real y) const;

		virtual bool left(const Point2D &p) const;
		virtual bool left(const Point2D *p) const;
		virtual bool left(Real x, Real y) const;

		virtual bool right(const Point2D &p) const;
		virtual bool right(const Point2D *p) const;
		virtual bool right(Real x, Real y) const;

		virtual bool accordingToNormal(const Point2D &p, bool insideTest = false) const;
		virtual bool accordingToNormal(const Point2D *p, bool insideTest = false) const;

		using Edge::angle;
		virtual Real angle(const Point2D &p1, const Point2D &p2) const;
		virtual Real angle(const Point2D *p1, const Point2D *p2) const;
		virtual Real angle(const Edge2D &e) const;
		virtual Real angle(const Edge2D *e) const;
		virtual Real angle(const Point2D &p) const;
		virtual Real angle(const Point2D *p) const;
		virtual Real orientedAngle(const Point2D &p, bool insideTest = false) const;
		virtual Real orientedAngle(const Point2D *p, bool insideTest = false) const;

		virtual Vector2D normal() const;

#if USE_ARRAY
		using Edge::operator=;
#endif //#if USE_ARRAY

#if USE_GUI
		using Edge::draw;
		//virtual void draw(bool fill) const;
		virtual void draw() const;

		using Edge::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
#endif //#if USE_GUI
	};
}

#endif //#ifndef _DATA_EDGE2D_H_

#ifndef _DATA_EDGE3D_H_
#define _DATA_EDGE3D_H_

#include "Data/Definitions.h"
#include "Data/Edge.h"

#include "Data/Vector3D.h"
#include "Data/Point3D.h"

namespace Data
{
	class Edge3D : public Data::Edge
	{
	public:

		Edge3D(Point3D *p1 = NULL, Point3D *p2 = NULL);
		Edge3D(const Point3D &p1, const Point3D &p2);
		virtual ~Edge3D();

		virtual UInt dimension() const;

		virtual bool intercept(const Edge3D &e) const;
		virtual bool intercept(const Edge3D *e) const;
		virtual bool intercept(const Point3D &p1, const Point3D &p2) const;
		virtual bool intercept(const Point3D *p1, const Point3D *p2) const;

#if USE_EXACT
		virtual bool eintercept(const Edge3D &e) const;
		virtual bool eintercept(const Edge3D *e) const;
		virtual bool eintercept(const Point3D &p1, const Point3D &p2) const;
		virtual bool eintercept(const Point3D *p1, const Point3D *p2) const;
#endif //#if USE_EXACT

		virtual Real distance(const Point3D &p) const;
		virtual Real distance(const Point3D *p) const;
		virtual Real distance(Real x, Real y, Real z) const;

		virtual Real straightDistance(const Point &p) const;
		virtual Real straightDistance(const Point *p) const;
		virtual Real straightDistance(const Point3D &p) const;
		virtual Real straightDistance(const Point3D *p) const;
		virtual Real straightDistance(Real x, Real y, Real z) const;

		virtual Real height(const Point &p) const;
		virtual Real height(const Point *p) const;
		virtual Real height(const Point3D &p) const;
		virtual Real height(const Point3D *p) const;
		virtual Real height(Real x, Real y, Real z) const;

		using Edge::angle;
		virtual Real angle(const Point3D &p1, const Point3D &p2) const;
		virtual Real angle(const Point3D *p1, const Point3D *p2) const;
		virtual Real angle(const Edge3D &e) const;
		virtual Real angle(const Edge3D *e) const;
		virtual Real angle(const Point3D &p) const;
		virtual Real angle(const Point3D *p) const;

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

#endif //#ifndef _DATA_EDGE3D_H_

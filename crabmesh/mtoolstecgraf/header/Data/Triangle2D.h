#ifndef _DATA_TRIANGLE2D_H_
#define _DATA_TRIANGLE2D_H_

#include "Data/Definitions.h"
#include "Data/Triangle.h"
#include "Data/Box.h"
#include "Data/Point2D.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace Data
{
	class Triangle2D : public Data::Triangle
#if USE_POOL
		, public Data::Poolable<Data::Triangle2D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(Data::Triangle2D)
#endif //#if USE_POOL

	public:

		Triangle2D(Point2D *p1 = NULL, Point2D *p2 = NULL, Point2D *p3 = NULL);
		Triangle2D(const Point2D &p1, const Point2D &p2, const Point2D &p3);
		virtual ~Triangle2D();

		virtual UInt dimension() const;

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;
		virtual bool in(const Point2D &p) const;
		virtual bool in(const Point2D *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;
		virtual bool on(const Point2D &p) const;
		virtual bool on(const Point2D *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;
		virtual bool out(const Point2D &p) const;
		virtual bool out(const Point2D *p) const;

#if USE_EXPERIMENTAL_3
		virtual Point2D circumcenter2D() const;
#else
		virtual Point circumcenter() const;
#endif //#if USE_EXPERIMENTAL_3
		///virtual Real circumradius() const;
#if USE_EXPERIMENTAL_3
		virtual Point2D incenter2D() const;
#else
		virtual Point incenter() const;
#endif //#if USE_EXPERIMENTAL_3
		///virtual Real inradius() const;

		virtual R3Tuple barycentricCoordinates(const Point &p) const;
		virtual R3Tuple barycentricCoordinates(const Point *p) const;
		virtual R3Tuple barycentricCoordinates(const Point2D &p) const;
		virtual R3Tuple barycentricCoordinates(const Point2D *p) const;

		virtual Real surface() const;
		virtual Real orientedSurface() const;

#if USE_ARRAY
		using Triangle::operator=;
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void highlight() const;
		virtual void unhighlight() const;

		using Triangle::draw;
		//virtual void draw(bool fill) const;
		virtual void draw() const;

		using Triangle::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
#endif //#if USE_GUI

#if USE_EXPERIMENTAL_3
    protected:

        virtual Point3D circumcenter3D() const;
        virtual Point3D incenter3D() const;
#endif //#if USE_EXPERIMENTAL_3
	};
}

#endif //#ifndef _DATA_TRIANGLE2D_H_

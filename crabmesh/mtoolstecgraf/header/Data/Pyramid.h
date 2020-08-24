#ifndef _DATA_PYRAMID_H_
#define _DATA_PYRAMID_H_

#include "Data/Definitions.h"
#include "Data/Polyhedron.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace Data
{
	class Pyramid : public Data::Polyhedron
#if USE_POOL
		, public Data::Poolable<Data::Pyramid>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(Data::Pyramid)
#endif //#if USE_POOL

	public:

		Pyramid(Point *p1 = NULL, Point *p2 = NULL, Point *p3 = NULL, Point *p4 = NULL,
			Point *p5 = NULL);
		virtual ~Pyramid();

#if USE_EXPERIMENTAL_4
        virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;
#endif //#if USE_EXPERIMENTAL_4

		virtual UInt numPoints() const;

		virtual void setPoints(Point *p1, Point *p2, Point *p3, Point *p4, Point *p5);

		virtual void setP1(Point *p);
		virtual Point *getP1() const;

		virtual void setP2(Point *p);
		virtual Point *getP2() const;

		virtual void setP3(Point *p);
		virtual Point *getP3() const;

		virtual void setP4(Point *p);
		virtual Point *getP4() const;

		virtual void setP5(Point *p);
		virtual Point *getP5() const;

#if USE_EXPERIMENTAL_4
		virtual Real surface() const;
#endif //#if USE_EXPERIMENTAL_4

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

		///TODO
		virtual Real volume() const;
		virtual Real orientedVolume() const;

#if USE_ARRAY
		using Polyhedron::operator=;
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
		virtual Pyramid &operator=(const Pyramid &b);
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
		using Polyhedron::draw;
		//virtual void draw(bool fill) const;
		virtual void draw() const;

		using Polyhedron::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		using Polyhedron::qfill;
		virtual UInt qfill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		using Polyhedron::tfill;
		virtual UInt tfill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;

		virtual bool isInVisibleSpace() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	protected:

		Point *points[5];
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))

	};
}

#endif //#ifndef _DATA_PYRAMID_H_

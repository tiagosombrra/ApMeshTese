#ifndef _DATA_TETRAHEDRON_H_
#define _DATA_TETRAHEDRON_H_

#include "Data/Definitions.h"
#include "Data/Polyhedron.h"
#include "Data/Triangle3D.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace Data
{
	class Tetrahedron : public Data::Polyhedron
#if USE_POOL
		, public Data::Poolable<Data::Tetrahedron>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(Data::Tetrahedron)
#endif //#if USE_POOL

	public:

		Tetrahedron(Point *p1 = NULL, Point *p2 = NULL, Point *p3 = NULL, Point *p4 = NULL);
#if (!USE_EXPERIMENTAL_4)
		Tetrahedron(Point *p1, Point *p2, Point *p3, Point *p4,
			Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4);
#endif //#if (!USE_EXPERIMENTAL_4)
		virtual ~Tetrahedron();

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

#if USE_EXPERIMENTAL_4
		virtual Real surface() const;
#endif //#if USE_EXPERIMENTAL_4

#if (!USE_EXPERIMENTAL_4)
		virtual void setTriangles(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4);

		virtual void setT1(Triangle *t);
		virtual Triangle *getT1() const;

		virtual void setT2(Triangle *t);
		virtual Triangle *getT2() const;

		virtual void setT3(Triangle *t);
		virtual Triangle *getT3() const;

		virtual void setT4(Triangle *t);
		virtual Triangle *getT4() const;
#endif //#if (!USE_EXPERIMENTAL_4)

        virtual UInt numSides() const;

#if USE_EXPERIMENTAL
		virtual UInt dimension() const;
#endif //#if USE_EXPERIMENTAL

#if (!USE_EXPERIMENTAL_4)
		//builds the 4 triangles. assuming the 4 points were given following the
		// right hand rule, the normals are pointing out of the tetrahedron.
		virtual void makeTriangles();
#endif //#if (!USE_EXPERIMENTAL_4)

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual Real circumradius() const;
#if USE_EXPERIMENTAL_3
		virtual Point3D circumcenter() const;
#else
		virtual Point circumcenter() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual Real inradius() const;
#if USE_EXPERIMENTAL_3
		virtual Point3D incenter() const;
#else
		virtual Point incenter() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual R4Tuple barycentricCoordinates(const Point &p) const;
		virtual R4Tuple barycentricCoordinates(const Point *p) const;

		virtual Real volume() const;
		virtual Real orientedVolume() const;

		virtual Real gamma();

#if USE_ARRAY
		using Polyhedron::operator=;
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
		virtual Tetrahedron &operator=(const Tetrahedron &t);
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
#if (!USE_EXPERIMENTAL_4)
		virtual void setColor(Real r, Real g, Real b) const;
#endif //#if (!USE_EXPERIMENTAL_4)

		using Polyhedron::draw;
		//virtual void draw(bool fill) const;
		virtual void draw() const;

		using Polyhedron::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		using Polyhedron::tfill;
		virtual UInt tfill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;

		virtual bool isInVisibleSpace() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	protected:

		Point *points[4];
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	};
}

#endif //#ifndef _DATA_TETRAHEDRON_H_

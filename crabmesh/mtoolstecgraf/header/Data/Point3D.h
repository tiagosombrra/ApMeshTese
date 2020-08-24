#ifndef _DATA_POINT3D_H_
#define _DATA_POINT3D_H_

#include "Data/Definitions.h"
#include "Data/Point.h"
#include "Data/Vector3D.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

namespace Data
{
	class Point3D : public Data::Point
#if USE_POOL
		, public Data::Poolable<Data::Point3D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(Data::Point3D)
#endif //#if USE_POOL

	public:

		Point3D(Real x = 0.0, Real y = 0.0, Real z = 0.0);
		Point3D(const Point3D &p);
		Point3D(const Point &p);
		Point3D(const Point *p);
		Point3D(const Vector &v);
		Point3D(const Vector *v);
		Point3D(const Point &p, const Vector &v);
		Point3D(const Point *p, const Vector *v);
		virtual ~Point3D();

		virtual void setPosition(Real x, Real y, Real z);

		virtual void setX(Real x);
		virtual Real getX() const;

		virtual void setY(Real y);
		virtual Real getY() const;

		virtual void setZ(Real z);
		virtual Real getZ() const;

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

		virtual Real surface(const Point3D &p1, const Point3D &p2) const;
		virtual Real surface(const Point3D *p1, const Point3D *p2) const;

		virtual Real volume(const Point3D &p1, const Point3D &p2, const Point3D &p3) const;
		virtual Real volume(const Point3D *p1, const Point3D *p2, const Point3D *p3) const;

		virtual Real orientedVolume(const Point3D &p1, const Point3D &p2, const Point3D &p3) const;
		virtual Real orientedVolume(const Point3D *p1, const Point3D *p2, const Point3D *p3) const;

		using Point::match;
		virtual bool match(Real x, Real y, Real z);

		using Point::equal;
		virtual bool equal(Real x, Real y, Real z);

		//changes the coordinates of this point so that it projects to the
		//  plane given as the input parameters (normal is assumed to be
		//  normalized)
		virtual void project(const Point3D &point, const Vector3D &normal);

#if USE_EXACT
		virtual EPoint3D epoint3D() const;
#endif //#if USE_EXACT

#if USE_ARRAY
		using Point::operator=;
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
		virtual Point3D &operator=(const Point3D &p);
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
#if USE_EXPERIMENTAL_3
        virtual Point3D &operator=(const Point &p);
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

		Real coords[3];
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	};

#if USE_EXACT
	class EPoint3D : public Data::EPoint
//#if USE_POOL
//		, public Data::Poolable<Data::EPoint3D>
//#endif //#if USE_POOL
	{
//#if USE_POOL
//		USING_POOLABLE(Data::EPoint3D)
//#endif //#if USE_POOL

	public:

		EPoint3D(EReal x = 0.0, EReal y = 0.0, EReal z = 0.0);
		EPoint3D(const EPoint &p);
		EPoint3D(const EPoint *p);
		EPoint3D(const EVector &v);
		EPoint3D(const EVector *v);
		EPoint3D(const EPoint &p, const EVector &v);
		EPoint3D(const EPoint *p, const EVector *v);
		virtual ~EPoint3D();

		virtual void setPosition(EReal x, EReal y, EReal z);

		virtual void setX(EReal x);
		virtual EReal getX() const;

		virtual void setY(EReal y);
		virtual EReal getY() const;

		virtual void setZ(EReal z);
		virtual EReal getZ() const;

		virtual UInt dimension() const;

		virtual EReal surface(const EPoint3D &p1, const EPoint3D &p2) const;
		virtual EReal surface(const EPoint3D *p1, const EPoint3D *p2) const;

		virtual EReal volume(const EPoint3D &p1, const EPoint3D &p2, const EPoint3D &p3) const;
		virtual EReal volume(const EPoint3D *p1, const EPoint3D *p2, const EPoint3D *p3) const;

		virtual EReal orientedVolume(const EPoint3D &p1, const EPoint3D &p2, const EPoint3D &p3) const;
		virtual EReal orientedVolume(const EPoint3D *p1, const EPoint3D *p2, const EPoint3D *p3) const;

		using EPoint::match;
		virtual bool match(EReal x, EReal y, EReal z);

		//using EPoint::equal;
		//virtual bool equal(EReal x, EReal y, EReal z);

		virtual Point3D point3D() const;

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

#endif //#ifndef _DATA_POINT3D_H_

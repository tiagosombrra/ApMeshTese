#ifndef _DATA_TRIANGLE3D_H_
#define _DATA_TRIANGLE3D_H_

#include "Data/Definitions.h"
#include "Data/Triangle2D.h"
#include "Data/Triangle.h"
#include "Data/Point3D.h"

namespace Data
{
	class Triangle3D : public Data::Triangle
	{
	public:

		Triangle3D(Point3D *p1 = NULL, Point3D *p2 = NULL, Point3D *p3 = NULL);
		Triangle3D(const Point3D &p1, const Point3D &p2, const Point3D &p3);
		virtual ~Triangle3D();

		virtual UInt dimension() const;

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;
		virtual bool in(const Point3D &p) const;
		virtual bool in(const Point3D *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;
		virtual bool on(const Point3D &p) const;
		virtual bool on(const Point3D *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;
		virtual bool out(const Point3D &p) const;
		virtual bool out(const Point3D *p) const;

		virtual bool intercept(const Triangle3D &t) const;
		virtual bool intercept(const Triangle3D *t) const;
		virtual bool intercept(const Edge &e) const;
		virtual bool intercept(const Edge *e) const;
		virtual bool intercept(const Point3D &p1, const Point3D &p2, const Point3D &p3) const;
		virtual bool intercept(const Point3D *p1, const Point3D *p2, const Point3D *p3) const;

#if USE_EXACT
		virtual bool eintercept(const Triangle3D &t) const;
		virtual bool eintercept(const Triangle3D *t) const;
		virtual bool eintercept(const Edge &e) const;
		virtual bool eintercept(const Edge *e) const;
		virtual bool eintercept(const Point3D &p1, const Point3D &p2, const Point3D &p3) const;
		virtual bool eintercept(const Point3D *p1, const Point3D *p2, const Point3D *p3) const;
#endif //#if USE_EXACT

		virtual Point3D project(const Point3D &p) const;
		virtual Point3D project(const Point3D *p) const;

		/*virtual Real squaredDistance(const Point3D &p) const;
		virtual Real squaredDistance(const Point3D *p) const;
		virtual Real squaredDistance(Real x, Real y, Real z) const;*/

		virtual Real signedPlaneDistance(const Point3D &p) const;
		virtual Real signedPlaneDistance(const Point3D *p) const;
		virtual Real signedPlaneDistance(Real x, Real y, Real z) const;

#if USE_EXACT
		virtual EReal esignedPlaneDistance(const EPoint3D &p) const;
		virtual EReal esignedPlaneDistance(const EPoint3D *p) const;
		virtual EReal esignedPlaneDistance(EReal x, EReal y, EReal z) const;
#endif //#if USE_EXACT

		virtual Real straightDistanceEdge(Point3D *p1, Point3D *p2, const Point3D &p, Point2D *p12D, Point2D *p22D, const Point2D &p2D) const;
		virtual Real straightDistanceEdge(const Point3D *p, UInt p1, UInt p2) const;
		virtual Real distance(const Point3D &p) const;
		virtual Real distance(const Point3D *p) const;
		virtual Real distance(Real x, Real y, Real z) const;

		virtual Real straightDistance(const Point3D &p) const;
		virtual Real straightDistance(const Point3D *p) const;
		virtual Real straightDistance(Real x, Real y, Real z) const;

		virtual Real height(const Point3D &p) const;
		virtual Real height(const Point3D *p) const;
		virtual Real height(Real x, Real y, Real z) const;

		virtual bool accordingToNormal(const Point3D &p, bool insideTest = false) const;
		virtual bool accordingToNormal(const Point3D *p, bool insideTest = false) const;

		virtual Real solidAngle(const Point3D &p) const;
		virtual Real solidAngle(const Point3D *p) const;
		virtual Real orientedSolidAngle(const Point3D &p, bool insideTest = false) const;
		virtual Real orientedSolidAngle(const Point3D *p, bool insideTest = false) const;

		//back shoud be allocated as a 4x4 matrix;
		virtual Triangle2D convert(Real **back = NULL, Real **coordChange = NULL) const;

#if USE_EXPERIMENTAL_3
		virtual Point3D circumcenter3D() const;
		virtual Point3D incenter3D() const;
#else
		virtual Point circumcenter() const;
		virtual Point incenter() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual R3Tuple barycentricCoordinates(const Point &p) const;
		virtual R3Tuple barycentricCoordinates(const Point *p) const;
		virtual R3Tuple barycentricCoordinates(const Point3D &p) const;
		virtual R3Tuple barycentricCoordinates(const Point3D *p) const;

		virtual Real surface() const;
		virtual Real orientedSurface() const;

#if USE_ARRAY
		using Triangle::operator=;
#endif //#if USE_ARRAY

#if USE_GUI
		using Triangle::draw;
		//virtual void draw(bool fill) const;
		virtual void draw() const;

		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		virtual UInt fill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const;
#endif //#if USE_GUI

	protected:

		///in this method, p is the point we want to calculate the signed
		///  distance to a plane, n is the normalized normal of a plane,
		///  and d is such that n.dot(P) + d = 0, where P is any point in that
		///  plane
		static Real signedPlaneDistance(const Point3D &p, const Vector3D &n, Real d);
		static Real signedPlaneDistance(const Point3D *p, const Vector3D *n, Real d);
#if USE_EXACT
		static EReal esignedPlaneDistance(const EPoint3D &p, const EVector3D &n, EReal d);
		static EReal esignedPlaneDistance(const EPoint3D *p, const EVector3D *n, EReal d);
#endif //#if USE_EXACT

#if USE_EXPERIMENTAL_3
    protected:

        virtual Point2D circumcenter2D() const;
        virtual Point2D incenter2D() const;
#endif //#if USE_EXPERIMENTAL_3
	};
}

#endif //#ifndef _DATA_TRIANGLE3D_H_

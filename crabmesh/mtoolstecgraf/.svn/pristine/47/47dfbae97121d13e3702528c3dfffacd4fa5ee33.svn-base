#ifndef _DATA_SPLINE_ICURVE_H_
#define _DATA_SPLINE_ICURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/GeometricShape.h"

namespace Data
{
namespace Spline
{
	class ICurve : public Data::GeometricShape
	{
	public:

		ICurve();
		virtual ~ICurve();

		virtual void add(Point *p) = 0;

		virtual UInt numPoints() const = 0;
		virtual UInt numPoints(UInt segment) const;

		virtual void setPoint(UInt i, Point *p) = 0;
		virtual Point *getPoint(UInt i) const = 0;

#if USE_NUMERICAL
		virtual Box box(UInt segment) const;
		virtual Box box() const;
#endif //#if USE_NUMERICAL

		virtual Real orientedSize() const;

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual UInt numSegments() const;
		virtual Point *point(UInt segment, UInt i) const;

		virtual void move(UInt i, Real x, Real y, Real z = 0.0);

		virtual Point3D p(UInt segment, Real t) const;
#if USE_NUMERICAL
		virtual Point3D dp(UInt segment, Real t) const;
		virtual Point3D d2p(UInt segment, Real t) const;
#endif //#if USE_NUMERICAL

		virtual Real blend(UInt segment, UInt i, Real t) const;
#if USE_NUMERICAL
		virtual Real dBlend(UInt segment, UInt i, Real t) const;
		virtual Real d2Blend(UInt segment, UInt i, Real t) const;
#endif //#if USE_NUMERICAL

		virtual UInt degree(UInt segment) const;
		virtual UInt minDegree() const;
		virtual UInt maxDegree() const;
		virtual void degrees(UInt &min, UInt &max) const;

#if USE_NUMERICAL
		///Calculates the length using the Gauss Legendre Quadrature method
		/// tmin and tmax should satisfy: 0 <= tmin <= tmax <= 1.0
		/// parts should be at least 1; default value: degree()*degree()
		/// points should be between 2 and 8; default value: degree() + 1
		virtual Real length(UInt segment, Real tmin, Real tmax, UInt parts, UInt points) const;
		virtual Real length(UInt segment, Real tmin, Real tmax) const;
		virtual Real length(UInt segment) const;
		virtual Real length() const;
#endif //#if USE_NUMERICAL

#if USE_NUMERICAL
		virtual Real parameterInHalfLength(UInt segment, Real tmin = 0.0, Real tmax = 1.0) const;
		virtual Real parameterInLength(UInt segment, Real length) const;
		virtual Real parameterInParameterizedLength(UInt segment, Real parameterizedLength) const;

		virtual Point3D pointInHalfLength(UInt segment, Real tmin = 0.0, Real tmax = 1.0) const;
		virtual Point3D pointInLength(UInt segment, Real length) const;
		virtual Point3D pointInParameterInParameterizedLength(UInt segment, Real parameterizedLength) const;
#endif //#if USE_NUMERICAL

#if USE_NUMERICAL
		virtual Real parameter(UInt segment, const Point3D &pt) const;
#endif //#if USE_NUMERICAL

#if USE_NUMERICAL
		virtual Real curvature(UInt segment, Real t) const;
#endif //#if USE_NUMERICAL

		virtual Point3D pt3d(const Point *p) const;

#if USE_GUI
		virtual void setR(Real r) const;
		virtual void setG(Real g) const;
		virtual void setB(Real b) const;

		virtual void highlight() const;
		virtual void unhighlight() const;

		virtual void setDiscretization(UInt discretization) const;
		virtual UInt getDiscretization() const;

		using GeometricShape::draw;
		virtual void draw() const;

#if USE_NUMERICAL
		virtual void drawSpacedPoints() const;
#endif //#if USE_NUMERICAL
		virtual void drawPoints() const;
		virtual void drawPolygon() const;
#if USE_NUMERICAL
		virtual void drawBases() const;
#endif //#if USE_NUMERICAL

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeSplineFunction() const = 0;

	protected:

		SplineFunction *function;

#if USE_GUI
		mutable UInt discretization;
#endif //#if USE_GUI

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_ICURVE_H_

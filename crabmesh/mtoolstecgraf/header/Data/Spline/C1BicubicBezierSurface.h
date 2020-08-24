#ifndef _DATA_SPLINE_C1_BICUBIC_BEZIER_SURFACE_H_
#define _DATA_SPLINE_C1_BICUBIC_BEZIER_SURFACE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/BicubicBezierSurface.h"

namespace Data
{
namespace Spline
{
	class C1BicubicBezierSurface : public Data::Spline::BicubicBezierSurface
	{
	public:

		C1BicubicBezierSurface();
		virtual ~C1BicubicBezierSurface();

		virtual UInt add(UInt row, Point *p);

		using Data::Spline::BicubicBezierSurface::setPoint;
		virtual void setPoint(UInt i, UInt j, Point *p);

		virtual void move(UInt i, UInt j, Real x, Real y, Real z = 0.0);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual void correct(UInt i, UInt j, const Point3D &old, const Point3D &cur, bool vdirection = true) const;

		virtual Point3D findC1Point(const Point3D &p1, const Point3D &p2) const;
		virtual Point3D findC1Point(const Point3D &p, const Vector3D &v) const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_C1_BICUBIC_BEZIER_SURFACE_H_

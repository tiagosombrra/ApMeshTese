#ifndef _DATA_SPLINE_C1_CUBIC_BEZIER_CURVE_H_
#define _DATA_SPLINE_C1_CUBIC_BEZIER_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/CubicBezierCurve.h"

namespace Data
{
namespace Spline
{
	class C1CubicBezierCurve : public Data::Spline::CubicBezierCurve
	{
	public:

		C1CubicBezierCurve();
		virtual ~C1CubicBezierCurve();

		virtual void add(Point *p);

		virtual void setPoint(UInt i, Point *p);

		virtual void move(UInt i, Real x, Real y, Real z = 0.0);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual void correct(UInt i, const Point3D &old, const Point3D &cur) const;

		virtual Point3D findC1Point(const Point3D &p1, const Point3D &p2) const;
		virtual Point3D findC1Point(const Point3D &p, const Vector3D &v) const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_C1_CUBIC_BEZIER_CURVE_H_

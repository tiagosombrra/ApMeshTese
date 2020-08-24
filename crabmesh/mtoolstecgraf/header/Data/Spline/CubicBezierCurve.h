#ifndef _DATA_SPLINE_CUBIC_BEZIER_CURVE_H_
#define _DATA_SPLINE_CUBIC_BEZIER_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/BezierCurve.h"

namespace Data
{
namespace Spline
{
	class CubicBezierCurve : public Data::Spline::BezierCurve
	{
	public:

		CubicBezierCurve();
		virtual ~CubicBezierCurve();

		virtual void add(Point *p);

		using Data::Spline::Curve::numPoints;
		virtual UInt numPoints(UInt segment) const;

		virtual UInt numSegments() const;
		virtual Point *point(UInt segment, UInt i) const;

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_CUBIC_BEZIER_CURVE_H_

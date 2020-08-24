#ifndef _DATA_SPLINE_HERMITE_CURVE_H_
#define _DATA_SPLINE_HERMITE_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/Curve.h"

namespace Data
{
namespace Spline
{
	class HermiteCurve : public Data::Spline::Curve
	{
	public:

		HermiteCurve();
		virtual ~HermiteCurve();

		using Data::Spline::Curve::numPoints;
		virtual UInt numPoints(UInt segment) const;

		virtual UInt numSegments() const;
		virtual Point *point(UInt segment, UInt i) const;

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeSplineFunction() const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_HERMITE_CURVE_H_

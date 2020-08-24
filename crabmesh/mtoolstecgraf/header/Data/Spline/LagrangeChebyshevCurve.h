#ifndef _DATA_SPLINE_LAGRANGE_CHEBYSHEV_CURVE_H_
#define _DATA_SPLINE_LAGRANGE_CHEBYSHEV_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES
#if USE_NUMERICAL

#include "Data/Spline/Curve.h"

namespace Data
{
namespace Spline
{
	class LagrangeChebyshevCurve : public Data::Spline::Curve
	{
	public:

		LagrangeChebyshevCurve();
		virtual ~LagrangeChebyshevCurve();

		virtual void add(Point *p);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeSplineFunction() const;

	};
}
}

#endif //#if USE_NUMERICAL
#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_LAGRANGE_CHEBYSHEV_CURVE_H_

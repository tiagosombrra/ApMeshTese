#ifndef _DATA_SPLINE_LAGRANGE_CURVE_H_
#define _DATA_SPLINE_LAGRANGE_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/Curve.h"

namespace Data
{
namespace Spline
{
	class LagrangeCurve : public Data::Spline::Curve
	{
	public:

		LagrangeCurve();
		virtual ~LagrangeCurve();

		virtual void add(Point *p);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeSplineFunction() const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_LAGRANGE_CURVE_H_

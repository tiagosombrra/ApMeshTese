#ifndef _DATA_SPLINE_BEZIER_SPLINE_FUNCTION_H_
#define _DATA_SPLINE_BEZIER_SPLINE_FUNCTION_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/SplineFunction.h"

namespace Data
{
namespace Spline
{
	class BezierSplineFunction : public Data::Spline::SplineFunction
	{
	public:

		BezierSplineFunction();
		virtual ~BezierSplineFunction();

		virtual UInt degree() const;

		virtual Real blend(UInt i, UInt n, Real t) const;
		virtual Real dBlend(UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt i, UInt n, Real t) const;

		virtual void findCoefficients(UInt n) const;

	protected:

		mutable RealVector coefficients;
	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_BEZIER_SPLINE_FUNCTION_H_

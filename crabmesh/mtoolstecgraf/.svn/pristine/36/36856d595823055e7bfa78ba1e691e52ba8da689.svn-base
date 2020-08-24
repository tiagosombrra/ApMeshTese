#ifndef _DATA_SPLINE_CUBIC_B_SPLINE_FUNCTION_H_
#define _DATA_SPLINE_CUBIC_B_SPLINE_FUNCTION_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/BSplineFunction.h"

namespace Data
{
namespace Spline
{
	class CubicBSplineFunction : public Data::Spline::BSplineFunction
	{
	public:

		CubicBSplineFunction();
		virtual ~CubicBSplineFunction();

		virtual void setDegree(UInt degree, UInt n);

		virtual void changeNumPoints(UInt n);

		virtual Real blend(UInt i, UInt n, Real t) const;
		virtual Real dBlend(UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt i, UInt n, Real t) const;

		virtual Real blend(UInt min, UInt i, UInt n, Real t) const;
		virtual Real dBlend(UInt min, UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt min, UInt i, UInt n, Real t) const;

		virtual Real interpolate(UInt n, Real t) const;
		virtual Real interpolate(UInt min, UInt n, Real t) const;

	protected:

		virtual Real b(UInt i, UInt d, UInt n, Real t) const;
		virtual Real db(UInt i, UInt d, UInt n, Real t) const;
		virtual Real d2b(UInt i, UInt d, UInt n, Real t) const;

		virtual Real b(UInt min, UInt i, UInt d, UInt n, Real t) const;
		virtual Real db(UInt min, UInt i, UInt d, UInt n, Real t) const;
		virtual Real d2b(UInt min, UInt i, UInt d, UInt n, Real t) const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_CUBIC_B_SPLINE_FUNCTION_H_

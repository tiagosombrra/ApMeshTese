#ifndef _DATA_SPLINE_HERMITE_SPLINE_FUNCTION_H_
#define _DATA_SPLINE_HERMITE_SPLINE_FUNCTION_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/SplineFunction.h"

namespace Data
{
namespace Spline
{
	class HermiteSplineFunction : public Data::Spline::SplineFunction
	{
	public:

		HermiteSplineFunction();
		virtual ~HermiteSplineFunction();

		virtual UInt degree() const;

		virtual Real blend(UInt i, UInt n, Real t) const;
		virtual Real dBlend(UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt i, UInt n, Real t) const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_HERMITE_SPLINE_FUNCTION_H_

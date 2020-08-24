#ifndef _DATA_SPLINE_SPLINE_FUNCTION_H_
#define _DATA_SPLINE_SPLINE_FUNCTION_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

namespace Data
{
namespace Spline
{
	class SplineFunction
	{
	public:

		SplineFunction();
		virtual ~SplineFunction();

		virtual UInt degree() const = 0;

		virtual Real blend(UInt i, UInt n, Real t) const = 0;
#if USE_NUMERICAL
		///For the derivatives, the Newton method is used (central by default,
		///  forward in t = 0, and backward in t = 1)
		virtual Real dBlend(UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt i, UInt n, Real t) const;
#endif //#if USE_NUMERICAL
	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_SPLINE_FUNCTION_H_

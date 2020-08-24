#ifndef _DATA_SPLINE_KOCHANEK_BARTELS_SPLINE_FUNCTION_H_
#define _DATA_SPLINE_KOCHANEK_BARTELS_SPLINE_FUNCTION_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/SplineFunction.h"

namespace Data
{
namespace Spline
{
	class KochanekBartelsSplineFunction : public Data::Spline::SplineFunction
	{
	public:

		KochanekBartelsSplineFunction();
		virtual ~KochanekBartelsSplineFunction();

		virtual UInt degree() const;

		virtual void setTension(Real tension);
		virtual Real getTension() const;

		virtual void setBias(Real bias);
		virtual Real getBias() const;

		virtual void setContinuity(Real continuity);
		virtual Real getContinuity() const;

		virtual Real blend(UInt i, UInt n, Real t) const;
		virtual Real dBlend(UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt i, UInt n, Real t) const;

	protected:

		Real tension;
		Real bias;
		Real continuity;
	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_KOCHANEK_BARTELS_SPLINE_FUNCTION_H_

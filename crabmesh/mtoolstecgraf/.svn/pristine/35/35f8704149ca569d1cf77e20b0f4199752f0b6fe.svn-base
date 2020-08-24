#ifndef _DATA_SPLINE_B_SPLINE_FUNCTION_H_
#define _DATA_SPLINE_B_SPLINE_FUNCTION_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/SplineFunction.h"

namespace Data
{
namespace Spline
{
	class BSplineFunction : public Data::Spline::SplineFunction
	{
	public:

		BSplineFunction();
		virtual ~BSplineFunction();

		virtual UInt degree() const;

		virtual void setDegree(UInt degree, UInt n);
		virtual UInt getDegree() const;

		virtual void setKnot(UInt i, Real knot);
		virtual Real getKnot(UInt i) const;
		virtual UInt getKnotSize(UInt n) const;
		virtual UInt getKnotSize() const;

		virtual void changeNumPoints(UInt n);

		virtual Real blend(UInt i, UInt n, Real t) const;
		virtual Real dBlend(UInt i, UInt n, Real t) const;
		virtual Real d2Blend(UInt i, UInt n, Real t) const;

	protected:

		virtual Real interpolate(UInt n, Real t) const;

		virtual void refill(UInt oldMin, UInt oldMax, Real *old, UInt newMin, UInt newMax, bool remake);

		virtual Real b(UInt i, UInt d, UInt n, Real t) const;
		virtual Real db(UInt i, UInt d, UInt n, Real t) const;
		virtual Real d2b(UInt i, UInt d, UInt n, Real t) const;

	protected:

		UInt deg;
		RealVector knot;
	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_B_SPLINE_FUNCTION_H_

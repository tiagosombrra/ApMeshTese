#include "Data/Spline/HermiteSplineFunction.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::HermiteSplineFunction::HermiteSplineFunction() :
	Data::Spline::SplineFunction::SplineFunction()
{

}

Data::Spline::HermiteSplineFunction::~HermiteSplineFunction()
{

}

UInt Data::Spline::HermiteSplineFunction::degree() const
{
	return 3;
}

Real Data::Spline::HermiteSplineFunction::blend(UInt i, UInt n, Real t) const
{
	return (n != 4) ? 0.0 :
		   (i == 0) ?      t*t*t -     t*t - t + 1.0 :
		   (i == 1) ?      t*t*t - 2.0*t*t + t :
		   (i == 2) ? -3.0*t*t*t + 4.0*t*t :
					       t*t*t -     t*t;
}

Real Data::Spline::HermiteSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	return (n != 4) ? 0.0 :
		   (i == 0) ?      3.0*t*t -     2.0*t - 1.0 :
		   (i == 1) ?      3.0*t*t - 2.0*2.0*t + 1.0 :
		   (i == 2) ? -3.0*3.0*t*t + 4.0*2.0*t :
					       3.0*t*t -     2.0*t;
}

Real Data::Spline::HermiteSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	return (n != 4) ? 0.0 :
		   (i == 0) ?      3.0*2.0*t -     2.0 :
		   (i == 1) ?      3.0*2.0*t - 2.0*2.0 :
		   (i == 2) ? -3.0*3.0*2.0*t + 4.0*2.0 :
					       3.0*2.0*t -     2.0;
}

#endif //#if USE_SPLINES

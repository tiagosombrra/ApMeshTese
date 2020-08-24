#include "Data/Spline/KochanekBartelsSplineFunction.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::KochanekBartelsSplineFunction::KochanekBartelsSplineFunction() :
	Data::Spline::SplineFunction::SplineFunction()
{
	this->tension = 1.0;
	this->bias = 1.0;
	this->continuity = 1.0;
}

Data::Spline::KochanekBartelsSplineFunction::~KochanekBartelsSplineFunction()
{

}

UInt Data::Spline::KochanekBartelsSplineFunction::degree() const
{
	return 3;
}

void Data::Spline::KochanekBartelsSplineFunction::setTension(Real tension)
{
	this->tension = tension;
}

Real Data::Spline::KochanekBartelsSplineFunction::getTension() const
{
	return this->tension;
}

void Data::Spline::KochanekBartelsSplineFunction::setBias(Real bias)
{
	this->bias = bias;
}

Real Data::Spline::KochanekBartelsSplineFunction::getBias() const
{
	return this->bias;
}

void Data::Spline::KochanekBartelsSplineFunction::setContinuity(Real continuity)
{
	this->continuity = continuity;
}

Real Data::Spline::KochanekBartelsSplineFunction::getContinuity() const
{
	return this->continuity;
}

Real Data::Spline::KochanekBartelsSplineFunction::blend(UInt i, UInt n, Real t) const
{
	if (n != 4)
	{
		return 0.0;
	}

	Real s  = (1.0 - this->tension)/2.0;
	Real q1 = (1.0 + this->bias)*(1.0 - this->continuity);
	Real q2 = (1.0 - this->bias)*(1.0 + this->continuity);
	Real q3 = (1.0 + this->bias)*(1.0 + this->continuity);
	Real q4 = (1.0 - this->bias)*(1.0 - this->continuity);

	Real sq1 = s*q1;
	Real sq2 = s*q2;
	Real sq3 = s*q3;
	Real sq4 = s*q4;

	return (i == 0) ?                     -sq1*t*t*t +                          2.0*sq1*t*t -         sq1*t :
		   (i == 1) ? ( 2.0 + sq1 - sq2 - sq3)*t*t*t + (-3.0 - 2.0*sq1 + 2.0*sq2 + sq3)*t*t + (sq1 - sq2)*t + 1.0 :
		   (i == 2) ? (-2.0 + sq2 + sq3 - sq4)*t*t*t +      (3.0 - 2.0*sq2 - sq3 + sq4)*t*t +         sq2*t :
		   	                               sq4*t*t*t -                              sq4*t*t;
}

Real Data::Spline::KochanekBartelsSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	if (n != 4)
	{
		return 0.0;
	}

	Real s  = (1.0 - this->tension)/2.0;
	Real q1 = (1.0 + this->bias)*(1.0 - this->continuity);
	Real q2 = (1.0 - this->bias)*(1.0 + this->continuity);
	Real q3 = (1.0 + this->bias)*(1.0 + this->continuity);
	Real q4 = (1.0 - this->bias)*(1.0 - this->continuity);

	Real sq1 = s*q1;
	Real sq2 = s*q2;
	Real sq3 = s*q3;
	Real sq4 = s*q4;

	return (i == 0) ?                     -sq1*3.0*t*t +                          2.0*sq1*2.0*t -         sq1 :
		   (i == 1) ? ( 2.0 + sq1 - sq2 - sq3)*3.0*t*t + (-3.0 - 2.0*sq1 + 2.0*sq2 + sq3)*2.0*t + (sq1 - sq2) :
		   (i == 2) ? (-2.0 + sq2 + sq3 - sq4)*3.0*t*t +      (3.0 - 2.0*sq2 - sq3 + sq4)*2.0*t +         sq2 :
		   	                               sq4*3.0*t*t -                              sq4*2.0*t;
}

Real Data::Spline::KochanekBartelsSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	if (n != 4)
	{
		return 0.0;
	}

	Real s  = (1.0 - this->tension)/2.0;
	Real q1 = (1.0 + this->bias)*(1.0 - this->continuity);
	Real q2 = (1.0 - this->bias)*(1.0 + this->continuity);
	Real q3 = (1.0 + this->bias)*(1.0 + this->continuity);
	Real q4 = (1.0 - this->bias)*(1.0 - this->continuity);

	Real sq1 = s*q1;
	Real sq2 = s*q2;
	Real sq3 = s*q3;
	Real sq4 = s*q4;

	return (i == 0) ?                     -sq1*3.0*2.0*t +                          2.0*sq1*2.0 :
		   (i == 1) ? ( 2.0 + sq1 - sq2 - sq3)*3.0*2.0*t + (-3.0 - 2.0*sq1 + 2.0*sq2 + sq3)*2.0 :
		   (i == 2) ? (-2.0 + sq2 + sq3 - sq4)*3.0*2.0*t +      (3.0 - 2.0*sq2 - sq3 + sq4)*2.0 :
		   	                               sq4*3.0*2.0*t -                              sq4*2.0;
}

#endif //#if USE_SPLINES

#include "Data/Spline/CardinalCurve.h"

#if USE_SPLINES

#include "Data/Spline/KochanekBartelsSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::CardinalCurve::CardinalCurve() :
	Data::Spline::KochanekBartelsCurve::KochanekBartelsCurve()
{

}

Data::Spline::CardinalCurve::~CardinalCurve()
{

}

void Data::Spline::CardinalCurve::add(Point *p)
{
	Data::Spline::KochanekBartelsCurve::add(p);

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setBias(0.0);
	static_cast<KochanekBartelsSplineFunction *>(this->function)->setContinuity(0.0);
}

void Data::Spline::CardinalCurve::setBias(Real /*bias*/)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setBias(0.0);
}

void Data::Spline::CardinalCurve::setContinuity(Real /*continuity*/)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setContinuity(0.0);
}

#if USE_GUI
std::string Data::Spline::CardinalCurve::text() const
{
	std::stringstream s;

	s << "Cardinal, " << Data::Spline::Curve::text()
	  << ", tension: " << this->getTension();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

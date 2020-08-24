#include "Data/Spline/KochanekBartelsCurve.h"

#if USE_SPLINES

#include "Data/Spline/KochanekBartelsSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::KochanekBartelsCurve::KochanekBartelsCurve() :
	Data::Spline::Curve::Curve()
{

}

Data::Spline::KochanekBartelsCurve::~KochanekBartelsCurve()
{

}

void Data::Spline::KochanekBartelsCurve::setTension(Real tension)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setTension(tension);
}

Real Data::Spline::KochanekBartelsCurve::getTension() const
{
	return (!this->function) ? 0.0 : static_cast<KochanekBartelsSplineFunction *>(this->function)->getTension();
}

void Data::Spline::KochanekBartelsCurve::setBias(Real bias)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setBias(bias);
}

Real Data::Spline::KochanekBartelsCurve::getBias() const
{
	return (!this->function) ? 0.0 : static_cast<KochanekBartelsSplineFunction *>(this->function)->getBias();
}

void Data::Spline::KochanekBartelsCurve::setContinuity(Real continuity)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setContinuity(continuity);
}

Real Data::Spline::KochanekBartelsCurve::getContinuity() const
{
	return (!this->function) ? 0.0 : static_cast<KochanekBartelsSplineFunction *>(this->function)->getContinuity();
}

UInt Data::Spline::KochanekBartelsCurve::numPoints(UInt segment) const
{
	return (segment < this->numSegments()) ? 4 : 0;
}

UInt Data::Spline::KochanekBartelsCurve::numSegments() const
{
	return (this->numPoints() < 4) ? 0 : (this->numPoints() - 3);
}

Point *Data::Spline::KochanekBartelsCurve::point(UInt segment, UInt i) const
{
	return this->points[segment + i];
}

#if USE_GUI
std::string Data::Spline::KochanekBartelsCurve::text() const
{
	std::stringstream s;

	s << "KochanekBartels, " << Data::Spline::Curve::text()
	  << ", tension: " << this->getTension()
	  << ", bias: " << this->getBias()
	  << ", continuity: " << this->getContinuity();

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::KochanekBartelsCurve::makeSplineFunction() const
{
	return new KochanekBartelsSplineFunction();
}

#endif //#if USE_SPLINES

#include "Data/Spline/BezierCurve.h"

#if USE_SPLINES

#include "Data/Spline/BezierSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::BezierCurve::BezierCurve() :
	Data::Spline::Curve::Curve()
{

}

Data::Spline::BezierCurve::~BezierCurve()
{

}

void Data::Spline::BezierCurve::add(Point *p)
{
	Data::Spline::Curve::add(p);

	static_cast<BezierSplineFunction *>(this->function)->findCoefficients(this->numPoints());
}

#if USE_GUI
std::string Data::Spline::BezierCurve::text() const
{
	std::stringstream s;

	s << "Bezier, " << Data::Spline::Curve::text();

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::BezierCurve::makeSplineFunction() const
{
	return new BezierSplineFunction();
}

#endif //#if USE_SPLINES

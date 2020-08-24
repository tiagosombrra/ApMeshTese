#include "Data/Spline/LagrangeChebyshevCurve.h"

#if USE_SPLINES
#if USE_NUMERICAL

#include "Data/Point3D.h"
#include "Data/Spline/LagrangeChebyshevSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::LagrangeChebyshevCurve::LagrangeChebyshevCurve() :
	Data::Spline::Curve::Curve()
{

}

Data::Spline::LagrangeChebyshevCurve::~LagrangeChebyshevCurve()
{

}

void Data::Spline::LagrangeChebyshevCurve::add(Point *p)
{
	Data::Spline::Curve::add(p);

	static_cast<LagrangeChebyshevSplineFunction *>(this->function)->setDeg(this->numPoints() - 1);
}

#if USE_GUI
std::string Data::Spline::LagrangeChebyshevCurve::text() const
{
	std::stringstream s;

	s << "LagrangeChebyshev, " << Data::Spline::Curve::text();

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::LagrangeChebyshevCurve::makeSplineFunction() const
{
	return new LagrangeChebyshevSplineFunction();
}

#endif //#if USE_NUMERICAL
#endif //#if USE_SPLINES

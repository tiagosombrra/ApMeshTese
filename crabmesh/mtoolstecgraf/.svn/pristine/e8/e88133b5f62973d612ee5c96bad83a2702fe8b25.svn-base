#include "Data/Spline/LagrangeCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"
#include "Data/Spline/LagrangeSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::LagrangeCurve::LagrangeCurve() :
	Data::Spline::Curve::Curve()
{

}

Data::Spline::LagrangeCurve::~LagrangeCurve()
{

}

void Data::Spline::LagrangeCurve::add(Point *p)
{
	Data::Spline::Curve::add(p);

	static_cast<LagrangeSplineFunction *>(this->function)->setDeg(this->numPoints() - 1);
}

#if USE_GUI
std::string Data::Spline::LagrangeCurve::text() const
{
	std::stringstream s;

	s << "Lagrange, " << Data::Spline::Curve::text();

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::LagrangeCurve::makeSplineFunction() const
{
	return new LagrangeSplineFunction();
}

#endif //#if USE_SPLINES

#include "Data/Spline/CubicLagrangeCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"
#include "Data/Spline/LagrangeSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::CubicLagrangeCurve::CubicLagrangeCurve() :
	Data::Spline::LagrangeCurve::LagrangeCurve()
{

}

Data::Spline::CubicLagrangeCurve::~CubicLagrangeCurve()
{

}

void Data::Spline::CubicLagrangeCurve::add(Point *p)
{
	Data::Spline::Curve::add(p);

	static_cast<LagrangeSplineFunction *>(this->function)->setDeg(3);
}

UInt Data::Spline::CubicLagrangeCurve::numPoints(UInt segment) const
{
	return (segment < this->numSegments()) ? 4 : 0;
}

UInt Data::Spline::CubicLagrangeCurve::numSegments() const
{
	return (this->numPoints() == 0) ? 0 : (this->numPoints() - 1)/3;
}

Point *Data::Spline::CubicLagrangeCurve::point(UInt segment, UInt i) const
{
	return this->points[3*segment + i];
}

#if USE_GUI
std::string Data::Spline::CubicLagrangeCurve::text() const
{
	std::stringstream s;

	s << "Cubic" << Data::Spline::LagrangeCurve::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

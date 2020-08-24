#include "Data/Spline/CubicBezierCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"
#include "Data/Spline/BezierSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::CubicBezierCurve::CubicBezierCurve() :
	Data::Spline::BezierCurve::BezierCurve()
{

}

Data::Spline::CubicBezierCurve::~CubicBezierCurve()
{

}

void Data::Spline::CubicBezierCurve::add(Point *p)
{
    Data::Spline::Curve::add(p);

    static_cast<BezierSplineFunction *>(this->function)->findCoefficients(4);
}

UInt Data::Spline::CubicBezierCurve::numPoints(UInt segment) const
{
	return (segment < this->numSegments()) ? 4 : 0;
}

UInt Data::Spline::CubicBezierCurve::numSegments() const
{
	return (this->numPoints() == 0) ? 0 : (this->numPoints() - 1)/3;
}

Point *Data::Spline::CubicBezierCurve::point(UInt segment, UInt i) const
{
	return this->points[3*segment + i];
}

#if USE_GUI
std::string Data::Spline::CubicBezierCurve::text() const
{
	std::stringstream s;

	s << "Cubic" << Data::Spline::BezierCurve::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

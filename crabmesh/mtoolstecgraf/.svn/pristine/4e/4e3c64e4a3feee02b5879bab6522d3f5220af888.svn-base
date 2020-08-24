#include "Data/Spline/HermiteCurve.h"

#if USE_SPLINES

#include "Data/State.h"
#include "Data/Point2D.h"
#include "Data/Spline/HermiteSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::HermiteCurve::HermiteCurve() :
	Data::Spline::Curve::Curve()
{

}

Data::Spline::HermiteCurve::~HermiteCurve()
{

}

UInt Data::Spline::HermiteCurve::numPoints(UInt segment) const
{
	return (segment < this->numSegments()) ? 4 : 0;
}

UInt Data::Spline::HermiteCurve::numSegments() const
{
	return (this->numPoints() == 0) ? 0 : (this->numPoints() - 1)/3;
}

Point *Data::Spline::HermiteCurve::point(UInt segment, UInt i) const
{
	return ((i == 0) && (segment != 0)) ? this->points[3*segment + i - 1] :
		this->points[3*segment + i];
}

#if USE_GUI

std::string Data::Spline::HermiteCurve::text() const
{
	std::stringstream s;

	s << "Hermite, " << Data::Spline::Curve::text();

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::HermiteCurve::makeSplineFunction() const
{
	return new HermiteSplineFunction();
}

#endif //#if USE_SPLINES

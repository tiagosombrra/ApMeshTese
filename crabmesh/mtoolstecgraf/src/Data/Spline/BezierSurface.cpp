#include "Data/Spline/BezierSurface.h"

#if USE_SPLINES

#include "Data/Spline/BezierSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::BezierSurface::BezierSurface() :
	Data::Spline::Surface::Surface()
{

}

Data::Spline::BezierSurface::~BezierSurface()
{

}

UInt Data::Spline::BezierSurface::add(UInt row, Point *p)
{
	UInt col = Data::Spline::Surface::add(row, p);

	static_cast<BezierSplineFunction *>(this->uFunction)->findCoefficients(this->numPointsU());
	static_cast<BezierSplineFunction *>(this->vFunction)->findCoefficients(this->numPointsV());

	return col;
}

#if USE_GUI
std::string Data::Spline::BezierSurface::text() const
{
	std::stringstream s;

	s << "Bezier, " << Data::Spline::Surface::text();

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::BezierSurface::makeUSplineFunction() const
{
	return new BezierSplineFunction();
}

SplineFunction *Data::Spline::BezierSurface::makeVSplineFunction() const
{
	return new BezierSplineFunction();
}

#endif //#if USE_SPLINES

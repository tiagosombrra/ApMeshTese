#include "Data/Spline/CubicBCurve.h"

#if USE_SPLINES

#include "Data/Spline/CubicBSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::CubicBCurve::CubicBCurve() :
	Data::Spline::BCurve::BCurve()
{
	this->setDegree(3);
}

Data::Spline::CubicBCurve::~CubicBCurve()
{

}

UInt Data::Spline::CubicBCurve::numPoints(UInt segment) const
{
	return (segment >= this->numSegments()) ? 0 :
		((this->numPoints() < 4) && (segment == 0)) ? this->numPoints() : 4;
}

UInt Data::Spline::CubicBCurve::numSegments() const
{
	return (this->numPoints() < 4) ? 1 : (this->numPoints() - 3);
}

Point *Data::Spline::CubicBCurve::point(UInt segment, UInt i) const
{
	return this->points[segment + i];
}

void Data::Spline::CubicBCurve::setKnot(UInt segment, UInt i, Real knot)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<BSplineFunction *>(this->function)->setKnot(this->getMinKnot(segment) + i, knot);
}

Real Data::Spline::CubicBCurve::getKnot(UInt segment, UInt i) const
{
	return (!this->function) ? 0.0 : static_cast<BSplineFunction *>(this->function)->getKnot(this->getMinKnot(segment) + i);
}

UInt Data::Spline::CubicBCurve::getMinKnot(UInt segment) const
{
	return segment;
}

Real Data::Spline::CubicBCurve::blend(UInt segment, UInt i, Real t) const
{
	return (!this->function) ? 0.0 : static_cast<CubicBSplineFunction *>(this->function)->blend(this->getMinKnot(segment), i, this->numPoints(segment), t);
}

Real Data::Spline::CubicBCurve::dBlend(UInt segment, UInt i, Real t) const
{
	return (!this->function) ? 0.0 : static_cast<CubicBSplineFunction *>(this->function)->dBlend(this->getMinKnot(segment), i, this->numPoints(segment), t);
}

Real Data::Spline::CubicBCurve::d2Blend(UInt segment, UInt i, Real t) const
{
	return (!this->function) ? 0.0 : static_cast<CubicBSplineFunction *>(this->function)->d2Blend(this->getMinKnot(segment), i, this->numPoints(segment), t);
}

#if USE_GUI
std::string Data::Spline::CubicBCurve::text() const
{
	std::stringstream str;

	str << "Cubic" << BCurve::text();

	return str.str();
}
#endif //#if USE_GUI

void Data::Spline::CubicBCurve::setDegree(UInt segment, UInt /*degree*/)
{
	Data::Spline::BCurve::setDegree(segment, 3);
}

void Data::Spline::CubicBCurve::setDegree(UInt /*degree*/)
{
	Data::Spline::BCurve::setDegree(3);
}

SplineFunction *Data::Spline::CubicBCurve::makeSplineFunction() const
{
	return new CubicBSplineFunction();
}

#endif //#if USE_SPLINES

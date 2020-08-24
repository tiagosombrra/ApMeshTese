#include "Data/Spline/BicubicBezierSurface.h"

#if USE_SPLINES

#include "Data/Point3D.h"
#include "Data/Spline/BezierSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::BicubicBezierSurface::BicubicBezierSurface() :
	Data::Spline::BezierSurface::BezierSurface()
{

}

Data::Spline::BicubicBezierSurface::~BicubicBezierSurface()
{

}

UInt Data::Spline::BicubicBezierSurface::add(UInt row, Point *p)
{
	UInt col = Data::Spline::Surface::add(row, p);

	static_cast<BezierSplineFunction *>(this->uFunction)->findCoefficients(4);
	static_cast<BezierSplineFunction *>(this->vFunction)->findCoefficients(4);

	return col;
}

UInt Data::Spline::BicubicBezierSurface::numPointsU(UInt segmentCol) const
{
	return (segmentCol < this->numSegmentsCol()) ? 4 : 0;
}

UInt Data::Spline::BicubicBezierSurface::numPointsV(UInt segmentRow) const
{
	return (segmentRow < this->numSegmentsRow()) ? 4 : 0;
}

UInt Data::Spline::BicubicBezierSurface::numSegmentsRow() const
{
	return (this->numPointsV() == 0) ? 0 : (this->numPointsV() - 1)/3;
}

UInt Data::Spline::BicubicBezierSurface::numSegmentsCol() const
{
	return (this->numPointsU() == 0) ? 0 : (this->numPointsU() - 1)/3;
}

Point *Data::Spline::BicubicBezierSurface::point(UInt segmentCol, UInt segmentRow, UInt i, UInt j) const
{
	return this->getPoint(3*segmentRow + i, 3*segmentCol + j);
}

#if USE_GUI
std::string Data::Spline::BicubicBezierSurface::text() const
{
	std::stringstream s;

	s << "Bicubic" << Data::Spline::BezierSurface::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

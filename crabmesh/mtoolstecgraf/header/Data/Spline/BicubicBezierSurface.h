#ifndef _DATA_SPLINE_BICUBIC_BEZIER_SURFACE_H_
#define _DATA_SPLINE_BICUBIC_BEZIER_SURFACE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/BezierSurface.h"

namespace Data
{
namespace Spline
{
	class BicubicBezierSurface : public Data::Spline::BezierSurface
	{
	public:

		BicubicBezierSurface();
		virtual ~BicubicBezierSurface();

		virtual UInt add(UInt row, Point *p);

		using Data::Spline::Surface::numPointsU;
		virtual UInt numPointsU(UInt segmentCol) const;
		using Data::Spline::Surface::numPointsV;
		virtual UInt numPointsV(UInt segmentRow) const;

		virtual UInt numSegmentsRow() const;
		virtual UInt numSegmentsCol() const;

		virtual Point *point(UInt segmentCol, UInt segmentRow, UInt i, UInt j) const;

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_BICUBIC_BEZIER_SURFACE_H_

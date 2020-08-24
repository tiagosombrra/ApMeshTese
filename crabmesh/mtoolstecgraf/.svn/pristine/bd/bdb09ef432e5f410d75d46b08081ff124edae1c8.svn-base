#ifndef _DATA_SPLINE_SURFACE_H_
#define _DATA_SPLINE_SURFACE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/ISurface.h"

namespace Data
{
namespace Spline
{
	class Surface : virtual public Data::Spline::ISurface
	{
	public:

		Surface();
		virtual ~Surface();

		virtual UInt add(UInt row, Point *p);

		using Data::Spline::ISurface::numPointsU;
		virtual UInt numPointsU() const;
		using Data::Spline::ISurface::numPointsV;
		virtual UInt numPointsV() const;

		using Data::Spline::ISurface::setPoint;
		virtual void setPoint(UInt i, UInt j, Point *p);
		using Data::Spline::ISurface::getPoint;
		virtual Point *getPoint(UInt i, UInt j) const;

	protected:

		virtual UInt addResizingPoints(UInt row, Point *p);

	protected:

		PointMatrix points;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_SURFACE_H_

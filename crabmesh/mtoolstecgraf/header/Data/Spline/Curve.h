#ifndef _DATA_SPLINE_CURVE_H_
#define _DATA_SPLINE_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/ICurve.h"

namespace Data
{
namespace Spline
{
	class Curve : virtual public Data::Spline::ICurve
	{
	public:

		Curve();
		virtual ~Curve();

		virtual void add(Point *p);

		using Data::Spline::ICurve::numPoints;
		virtual UInt numPoints() const;

		virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;

		virtual void degrees(UInt &min, UInt &max) const;

	protected:

		PointVector points;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_CURVE_H_

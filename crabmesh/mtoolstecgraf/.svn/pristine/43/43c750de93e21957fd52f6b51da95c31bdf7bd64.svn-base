#ifndef _DATA_SPLINE_C1_HERMITE_CURVE_H_
#define _DATA_SPLINE_C1_HERMITE_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/HermiteCurve.h"

namespace Data
{
namespace Spline
{
	class C1HermiteCurve : public Data::Spline::HermiteCurve
	{
	public:

		C1HermiteCurve();
		virtual ~C1HermiteCurve();

		virtual void add(Point *p);

		virtual void setPoint(UInt i, Point *p);

		virtual void move(UInt i, Real x, Real y, Real z = 0.0);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual void correct(UInt i, const Point3D &old, const Point3D &cur) const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_C1_HERMITE_CURVE_H_

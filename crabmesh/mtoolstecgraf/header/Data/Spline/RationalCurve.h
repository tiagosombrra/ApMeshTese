#ifndef _DATA_SPLINE_RATIONAL_CURVE_H_
#define _DATA_SPLINE_RATIONAL_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/ICurve.h"

namespace Data
{
namespace Spline
{
	class RationalCurve : virtual public Data::Spline::ICurve
	{
	public:

		RationalCurve();
		virtual ~RationalCurve();

		virtual void add(Point *p);
		virtual void add(Point *p, Real weight) = 0;

		virtual void setWeight(UInt i, Real weight);
		virtual Real getWeight(UInt i) const;

		virtual void setWeight(UInt segment, UInt i, Real weight);
		virtual Real getWeight(UInt segment, UInt i) const;

		virtual Point3D p(UInt segment, Real t) const;
#if USE_NUMERICAL
		virtual Point3D dp(UInt segment, Real t) const;
		virtual Point3D d2p(UInt segment, Real t) const;
#endif //#if USE_NUMERICAL

	protected:

		RealVector weights;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_RATIONAL_CURVE_H_

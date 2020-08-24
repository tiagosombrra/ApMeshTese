#ifndef _DATA_SPLINE_CUBIC_B_CURVE_H_
#define _DATA_SPLINE_CUBIC_B_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/BCurve.h"

namespace Data
{
namespace Spline
{
	class CubicBCurve : public Data::Spline::BCurve
	{
	public:

		CubicBCurve();
		virtual ~CubicBCurve();

		using Data::Spline::Curve::numPoints;
		virtual UInt numPoints(UInt segment) const;

		virtual UInt numSegments() const;
		virtual Point *point(UInt segment, UInt i) const;

		using Data::Spline::BCurve::setKnot;
		virtual void setKnot(UInt segment, UInt i, Real knot);
		using Data::Spline::BCurve::getKnot;
		virtual Real getKnot(UInt segment, UInt i) const;

		virtual UInt getMinKnot(UInt segment) const;

		virtual Real blend(UInt segment, UInt i, Real t) const;
		virtual Real dBlend(UInt segment, UInt i, Real t) const;
		virtual Real d2Blend(UInt segment, UInt i, Real t) const;

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual void setDegree(UInt segment, UInt degree);
		virtual void setDegree(UInt degree);

		virtual SplineFunction *makeSplineFunction() const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_CUBIC_B_CURVE_H_

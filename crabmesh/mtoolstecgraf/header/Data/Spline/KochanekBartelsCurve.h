#ifndef _DATA_SPLINE_KOCHANEK_BARTELS_CURVE_H_
#define _DATA_SPLINE_KOCHANEK_BARTELS_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/Curve.h"

namespace Data
{
namespace Spline
{
	class KochanekBartelsCurve : public Data::Spline::Curve
	{
	public:

		KochanekBartelsCurve();
		virtual ~KochanekBartelsCurve();

		virtual void setTension(Real tension);
		virtual Real getTension() const;

		virtual void setBias(Real bias);
		virtual Real getBias() const;

		virtual void setContinuity(Real continuity);
		virtual Real getContinuity() const;

		using Data::Spline::Curve::numPoints;
		virtual UInt numPoints(UInt segment) const;

		virtual UInt numSegments() const;
		virtual Point *point(UInt segment, UInt i) const;

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI
	protected:

		virtual SplineFunction *makeSplineFunction() const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_KOCHANEK_BARTELS_CURVE_H_

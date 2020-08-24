#ifndef _DATA_SPLINE_CARDINAL_CURVE_H_
#define _DATA_SPLINE_CARDINAL_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/KochanekBartelsCurve.h"

namespace Data
{
namespace Spline
{
	class CardinalCurve : public Data::Spline::KochanekBartelsCurve
	{
	public:

		CardinalCurve();
		virtual ~CardinalCurve();

		virtual void add(Point *p);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual void setBias(Real bias);

		virtual void setContinuity(Real continuity);

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_CARDINAL_CURVE_H_

#ifndef _DATA_SPLINE_NURB_CURVE_H_
#define _DATA_SPLINE_NURB_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/BCurve.h"
#include "Data/Spline/RationalCurve.h"

namespace Data
{
namespace Spline
{
	class NURBCurve :
		public Data::Spline::RationalCurve,
		public Data::Spline::BCurve
	{
	public:

		NURBCurve();
		virtual ~NURBCurve();

		virtual void add(Point *p);
		virtual void add(Point *p, Real weight);

		using Data::Spline::BCurve::blend;
#if USE_NUMERICAL
		using Data::Spline::BCurve::dBlend;
		using Data::Spline::BCurve::d2Blend;
#endif //#if USE_NUMERICAL

		using Data::Spline::RationalCurve::p;
#if USE_NUMERICAL
		using Data::Spline::RationalCurve::dp;
		using Data::Spline::RationalCurve::d2p;
#endif //#if USE_NUMERICAL

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_NURB_CURVE_H_

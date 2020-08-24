#ifndef _DATA_SPLINE_DEFINITIONS_H_
#define _DATA_SPLINE_DEFINITIONS_H_

#include "Data/Definitions.h"

#if USE_SPLINES

namespace Data
{
    //classes for Data::Math::Geometry::Spline library
	namespace Spline
	{
		class SplineFunction;
		class LagrangeSplineFunction;
#if USE_NUMERICAL
		class LagrangeChebyshevSplineFunction;
#endif //#if USE_NUMERICAL
		class HermiteSplineFunction;
		class BezierSplineFunction;
		class KochanekBartelsSplineFunction;
		class BSplineFunction;
		class NURBSplineFunction;

		class ICurve;
		class Curve;
		class RationalCurve;
		class LagrangeCurve;
		class CubicLagrangeCurve;
#if USE_NUMERICAL
		class LagrangeChebyshevCurve;
#endif //#if USE_NUMERICAL
		class HermiteCurve;
		class C1HermiteCurve;
		class BezierCurve;
		class CubicBezierCurve;
		class C1CubicBezierCurve;
		class KochanekBartelsCurve;
		class CardinalCurve;
		class CatmullRomCurve;
		class BCurve;
		class CubicBCurve;
		class NURBCurve;

		class ISurface;
		class Surface;
		class RationalSurface;
		class LagrangeSurface;
		class BicubicLagrangeSurface;
		class HermiteSurface;
		class C1HermiteSurface;
		class BezierSurface;
		class BicubicBezierSurface;
		class C1BicubicBezierSurface;
		class KochanekBartelsSurface;
		class CardinalSurface;
		class CatmullRomSurface;
		class BSurface;
		class BicubicBSurface;
		class NURBSurface;
	}

	typedef std::list<Spline::Curve *> CurveList;
	typedef std::list<Spline::Surface *> SurfaceList;
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_DEFINITIONS_H_

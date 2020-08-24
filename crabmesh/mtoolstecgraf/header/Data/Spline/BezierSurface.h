#ifndef _DATA_BEZIER_SURFACE_H_
#define _DATA_BEZIER_SURFACE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/Surface.h"

namespace Data
{
namespace Spline
{
	class BezierSurface : public Data::Spline::Surface
	{
	public:

		BezierSurface();
		virtual ~BezierSurface();

		virtual UInt add(UInt row, Point *p);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeUSplineFunction() const;
		virtual SplineFunction *makeVSplineFunction() const;
	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_BEZIER_SURFACE_H_

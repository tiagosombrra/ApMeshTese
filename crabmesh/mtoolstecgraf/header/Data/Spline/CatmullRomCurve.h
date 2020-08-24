#ifndef _DATA_SPLINE_CATMULL_ROM_CURVE_H_
#define _DATA_SPLINE_CATMULL_ROM_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/CardinalCurve.h"

namespace Data
{
namespace Spline
{
	class CatmullRomCurve : public Data::Spline::CardinalCurve
	{
	public:

		CatmullRomCurve();
		virtual ~CatmullRomCurve();

		virtual void add(Point *p);

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual void setTension(Real tension);

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_CATMULL_ROM_CURVE_H_

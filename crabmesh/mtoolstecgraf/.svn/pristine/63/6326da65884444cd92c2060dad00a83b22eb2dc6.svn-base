#ifndef _DATA_SPLINE_B_CURVE_H_
#define _DATA_SPLINE_B_CURVE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/Spline/Curve.h"

namespace Data
{
namespace Spline
{
	class BCurve : public Data::Spline::Curve
	{
	public:

		BCurve();
		virtual ~BCurve();

		virtual void setDegree(UInt segment, UInt degree);
		virtual UInt getDegree(UInt segment) const;

		virtual void setDegree(UInt degree);
		virtual UInt getDegree() const;

		virtual void setKnot(UInt segment, UInt i, Real knot);
		virtual Real getKnot(UInt segment, UInt i) const;
		virtual UInt getKnotSize(UInt segment) const;

		virtual void setKnot(UInt i, Real knot);
		virtual Real getKnot(UInt i) const;
		virtual UInt getKnotSize() const;

		virtual UInt getMinKnot(UInt segment) const;
		virtual UInt getMaxKnot(UInt segment) const;

		virtual void add(Point *p);

#if USE_GUI
		using Data::Spline::Curve::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeSplineFunction() const;

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_B_CURVE_H_

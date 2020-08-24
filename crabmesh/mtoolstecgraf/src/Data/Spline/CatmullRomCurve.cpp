#include "Data/Spline/CatmullRomCurve.h"

#if USE_SPLINES

#include "Data/Spline/KochanekBartelsSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::CatmullRomCurve::CatmullRomCurve() :
	Data::Spline::CardinalCurve::CardinalCurve()
{

}

Data::Spline::CatmullRomCurve::~CatmullRomCurve()
{

}

void Data::Spline::CatmullRomCurve::add(Point *p)
{
	Data::Spline::CardinalCurve::add(p);

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setTension(0.0);
}

void Data::Spline::CatmullRomCurve::setTension(Real /*tension*/)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<KochanekBartelsSplineFunction *>(this->function)->setTension(0.0);
}

#if USE_GUI
std::string Data::Spline::CatmullRomCurve::text() const
{
	std::stringstream s;

	s << "CatmullRom, " << Data::Spline::Curve::text();

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

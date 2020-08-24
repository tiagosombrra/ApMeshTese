#include "Data/Spline/NURBCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::NURBCurve::NURBCurve() :
	Data::Spline::RationalCurve::RationalCurve(),
	Data::Spline::BCurve::BCurve()
{

}

Data::Spline::NURBCurve::~NURBCurve()
{

}

void Data::Spline::NURBCurve::add(Point *p)
{
	this->add(p, 1.0);
}

void Data::Spline::NURBCurve::add(Point *p, Real weight)
{
	Data::Spline::BCurve::add(p);

	this->weights.push_back(weight);
}

#if USE_GUI
std::string Data::Spline::NURBCurve::text() const
{
	std::stringstream s;

	s << "NUR" << Data::Spline::BCurve::text()
	  << ", weights:";

	for (UInt i = 0; i < static_cast<UInt>(this->weights.size()); i++)
	{
		s << " " << this->weights[i];
	}

	return s.str();
}
#endif //#if USE_GUI

#endif //#if USE_SPLINES

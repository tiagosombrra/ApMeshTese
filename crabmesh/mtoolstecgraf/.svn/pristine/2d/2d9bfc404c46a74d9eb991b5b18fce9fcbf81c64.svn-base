#include "Data/Spline/Curve.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::Curve::Curve() :
	Data::Spline::ICurve::ICurve()
{

}

Data::Spline::Curve::~Curve()
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (this->points[i])
		{
			delete this->points[i];
		}
	}
}

void Data::Spline::Curve::add(Point *p)
{
	this->points.push_back(p);

#if USE_GUI
	p->setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		p->highlight();
	}
	else
	{
		p->unhighlight();
	}
#endif //#if USE_GUI

	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}
}

UInt Data::Spline::Curve::numPoints() const
{
	return static_cast<UInt>(this->points.size());
}

void Data::Spline::Curve::setPoint(UInt i, Point *p)
{
	this->points[i] = p;

#if USE_GUI
	p->setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		p->highlight();
	}
	else
	{
		p->unhighlight();
	}
#endif //#if USE_GUI
}

Point *Data::Spline::Curve::getPoint(UInt i) const
{
	return this->points[i];
}

void Data::Spline::Curve::degrees(UInt &min, UInt &max) const
{
	min = this->degree(0);
	max = min;
}

#endif //#if USE_SPLINES

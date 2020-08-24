#include "Data/Colorable.h"

#if USE_GUI

using namespace Data;

Data::Colorable::Colorable(Real r, Real g, Real b) :
	Drawable()
{
	this->setColor(r, g, b);
	this->highlighted = false;
}

Data::Colorable::~Colorable()
{

}

void Data::Colorable::setR(Real r) const
{
	this->r = r;
}

Real Data::Colorable::getR() const
{
	return this->r;
}

void Data::Colorable::setG(Real g) const
{
	this->g = g;
}

Real Data::Colorable::getG() const
{
	return this->g;
}

void Data::Colorable::setB(Real b) const
{
	this->b = b;
}

Real Data::Colorable::getB() const
{
	return this->b;
}

void Data::Colorable::highlight() const
{
	this->highlighted = true;
}

void Data::Colorable::unhighlight() const
{
	this->highlighted = false;
}

bool Data::Colorable::isHighlighted() const
{
	return this->highlighted;
}

#endif //#if USE_GUI

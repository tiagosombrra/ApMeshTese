#include "Data/FrontElement.h"

#include "Data/GeometricShape.h"
#include "Data/Vertex.h"

using namespace Data;

Data::FrontElement::FrontElement(GeometricShape *shape, ULInt id) :
	Element(shape, id)
{

}

Data::FrontElement::~FrontElement()
{

}

Real Data::FrontElement::quality() const
{
	return 0.0;
}

bool Data::FrontElement::isBad() const
{
	return false;
}

bool Data::FrontElement::accordingToNormal(Vertex *v, bool insideTest) const
{
	return this->accordingToNormal(v->getPoint(), insideTest);
}

bool Data::FrontElement::have(const FrontElement &/*e*/) const
{
	return false;
}

bool Data::FrontElement::have(const FrontElement */*e*/) const
{
	return false;
}

#if USE_GUI
void Data::FrontElement::drawNormal(Real r, Real g, Real b, Real factor) const
{
	Real orr = this->getR();
	Real org = this->getG();
	Real orb = this->getB();

	this->setColor(r, g, b);

	this->drawNormal(factor);

	this->setColor(orr, org, orb);
}
#endif //#if USE_GUI

#include "Tools/BoundaryNormalInverter.h"

#include "Data/Boundary.h"

Tools::BoundaryNormalInverter::BoundaryNormalInverter() :
	Performer::BoundaryNormalInverter::BoundaryNormalInverter(),
	Tools::Tool::Tool()
{

}

Tools::BoundaryNormalInverter::~BoundaryNormalInverter()
{

}

void Tools::BoundaryNormalInverter::execute()
{
	if (!this->boundary)
	{
		return;
	}

	this->setMeasure(0, static_cast<UInt>(this->boundary->size()) + 1);

	Performer::BoundaryNormalInverter::execute();

	this->endMeasure();
}

void Tools::BoundaryNormalInverter::invert(Data::FrontElement *fel)
{
	this->addMeasure(1, 0);

	Performer::BoundaryNormalInverter::invert(fel);
}

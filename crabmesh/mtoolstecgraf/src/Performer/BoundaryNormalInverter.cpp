#include "Performer/BoundaryNormalInverter.h"

#include "Data/FrontElement.h"
#include "Data/Boundary.h"

Performer::BoundaryNormalInverter::BoundaryNormalInverter() :
	Performer::Builder::Builder()
{
	this->boundary = NULL;
}

Performer::BoundaryNormalInverter::~BoundaryNormalInverter()
{

}

void Performer::BoundaryNormalInverter::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::BoundaryNormalInverter::execute()
{
	if (!this->boundary)
	{
		return;
	}

	for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
		 iter != end; iter++)
	{
		this->invert((*iter));
	}
}

void Performer::BoundaryNormalInverter::invert(Data::FrontElement *fel)
{
	fel->invert();
}

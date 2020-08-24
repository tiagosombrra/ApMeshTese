#include "Performer/BoundaryBuilder.h"

Performer::BoundaryBuilder::BoundaryBuilder() :
	Builder()
{
	this->boundary = NULL;
}

Performer::BoundaryBuilder::~BoundaryBuilder()
{

}

//#if USE_GUI
void Performer::BoundaryBuilder::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}
//#endif //#if USE_GUI

Data::Boundary *Performer::BoundaryBuilder::getBoundary() const
{
	return this->boundary;
}

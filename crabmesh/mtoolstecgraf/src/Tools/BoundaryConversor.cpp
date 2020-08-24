#include "Tools/BoundaryConversor.h"

#include "Data/Boundary.h"
#include "MJMesh/BoundaryBuilder.h"

Tools::BoundaryConversor::BoundaryConversor(Data::Boundary *original) :
	Performer::BoundaryConversor::BoundaryConversor(original),
	Tools::Tool::Tool()
{
	this->builder = NULL;
}

Tools::BoundaryConversor::~BoundaryConversor()
{
	if (this->builder)
	{
		delete this->builder;
	}
}

void Tools::BoundaryConversor::execute()
{
	if (!this->original)
	{
		return;
	}

	this->setMeasure(0, static_cast<UInt>(this->original->verticesSize() + this->original->size()) + 1);

	this->builder = new MJMesh::BoundaryBuilder(false);

	Performer::BoundaryConversor::execute();

	delete this->builder;

	this->builder = NULL;

	this->endMeasure();
}

void Tools::BoundaryConversor::addVertex(Real x, Real y, ULInt id)
{
	this->addMeasure(1, 0);

	this->builder->add(id, x, y);
}

void Tools::BoundaryConversor::addVertex(Real x, Real y, Real z, ULInt id)
{
	this->addMeasure(1, 0);

	this->builder->add(id, x, y, z);
}

void Tools::BoundaryConversor::addFrontElement(ULInt id1, ULInt id2, ULInt id)
{
	this->addMeasure(1, 0);

	this->builder->add(id, id1, id2);
}

void Tools::BoundaryConversor::addFrontElement(ULInt id1, ULInt id2, ULInt id3, ULInt id)
{
	this->addMeasure(1, 0);

	this->builder->add(id, id1, id2, id3);
}

Data::Boundary *Tools::BoundaryConversor::consolidate() const
{
	this->builder->execute();

	return this->builder->getBoundary();
}

#include "Tools/InnerSurfacesIdentifier.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/AnglePointInObjectDecider.h"

Tools::InnerSurfacesIdentifier::InnerSurfacesIdentifier() :
	Performer::InnerSurfacesIdentifier::InnerSurfacesIdentifier(),
	Tools::Tool::Tool()
{
	this->decider = NULL;
}

Tools::InnerSurfacesIdentifier::~InnerSurfacesIdentifier()
{
	if (this->decider)
	{
		delete this->decider;
	}
}

void Tools::InnerSurfacesIdentifier::execute()
{
	if (!this->boundary)
	{
		return;
	}

	this->startMeasure(1);

	this->decider = this->makeDecider();

	Performer::InnerSurfacesIdentifier::execute();

	delete this->decider;

	this->decider = NULL;

	this->endMeasure();
}

Data::Boundary *Tools::InnerSurfacesIdentifier::makeBoundary() const
{
	return new MJMesh::Boundary();
}

MJMesh::AnglePointInObjectDecider *Tools::InnerSurfacesIdentifier::makeDecider() const
{
	return new MJMesh::AnglePointInObjectDecider(this->boundary->dimension(), NULL, NULL, false, this->surfaceDirection);
}

Data::Position Tools::InnerSurfacesIdentifier::position(const Data::Point *point, const Data::Boundary *boundary) const
{
	this->decider->setPoint(point);

	this->decider->setFels(&boundary->getElements());

	this->decider->execute();

	return this->decider->getDecision();
}

void Tools::InnerSurfacesIdentifier::propagate()
{
	this->addMeasure(0, static_cast<UInt>(this->boundary->size()));

	Performer::InnerSurfacesIdentifier::propagate();
}

void Tools::InnerSurfacesIdentifier::propagate(Data::FrontElement *fe, Surface *surface)
{
	this->addMeasure(1, 0);

	Performer::InnerSurfacesIdentifier::propagate(fe, surface);
}

bool Tools::InnerSurfacesIdentifier::findCompleteBoundary2D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested)
{
	bool ret = Performer::InnerSurfacesIdentifier::findCompleteBoundary2D(fel, surface, toBeTested);

	if (ret)
	{
		this->addMeasure(1, 0);
	}

	return ret;
}

bool Tools::InnerSurfacesIdentifier::findCompleteBoundary3D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested)
{
	bool ret = Performer::InnerSurfacesIdentifier::findCompleteBoundary3D(fel, surface, toBeTested);

	if (ret)
	{
		this->addMeasure(1, 0);
	}

	return ret;
}

void Tools::InnerSurfacesIdentifier::findCompleteBoundaries()
{
	this->addMeasure(0, static_cast<UInt>(this->boundary->size()));

	Performer::InnerSurfacesIdentifier::findCompleteBoundaries();
}

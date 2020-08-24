#include "Tools/AdjacenciesIdentifier.h"

#include "Data/Boundary.h"
#include "Data/Mesh.h"

Tools::AdjacenciesIdentifier::AdjacenciesIdentifier(const Tool *tool) :
	Performer::AdjacenciesIdentifier::AdjacenciesIdentifier(),
	Tools::ExternalTool::ExternalTool(tool)
{

}

Tools::AdjacenciesIdentifier::~AdjacenciesIdentifier()
{

}

void Tools::AdjacenciesIdentifier::execute()
{
	if ((!this->boundary) && (!this->surface))
	{
		return;
	}

	this->addMeasure(0, static_cast<UInt>(this->boundary ? this->boundary->size() : this->surface->size()));

	Performer::AdjacenciesIdentifier::execute();
}

bool Tools::AdjacenciesIdentifier::identifyAdjacencies(Data::/*Front*/Element *fel)
{
	this->addMeasure(1, 0);

	return Performer::AdjacenciesIdentifier::identifyAdjacencies(fel);
}

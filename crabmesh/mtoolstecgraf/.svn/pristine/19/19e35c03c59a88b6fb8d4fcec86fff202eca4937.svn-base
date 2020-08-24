#include "Tools/BoundaryWaterTightnessChecker.h"

#include "Data/Boundary.h"

Tools::BoundaryWaterTightnessChecker::BoundaryWaterTightnessChecker() :
	Performer::BoundaryWaterTightnessChecker::BoundaryWaterTightnessChecker(),
	Tools::Tool::Tool()
{

}

Tools::BoundaryWaterTightnessChecker::~BoundaryWaterTightnessChecker()
{

}

void Tools::BoundaryWaterTightnessChecker::execute()
{
	Performer::BoundaryWaterTightnessChecker::execute();
}

void Tools::BoundaryWaterTightnessChecker::check(AdjacencyMap &map)
{
	this->setMeasure(0, static_cast<UInt>(map.size()) + 1);

	Performer::BoundaryWaterTightnessChecker::check(map);

	this->endMeasure();
}

void Tools::BoundaryWaterTightnessChecker::check(const AdjacencyMap::iterator &iter)
{
	this->addMeasure(1, 0);

	Performer::BoundaryWaterTightnessChecker::check(iter);
}

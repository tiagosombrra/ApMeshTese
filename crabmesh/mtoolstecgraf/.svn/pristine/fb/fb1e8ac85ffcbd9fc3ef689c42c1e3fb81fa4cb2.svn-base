#include "Performer/PointInObjectDecider.h"

Performer::PointInObjectDecider::PointInObjectDecider(UInt dimension,
	const Data::Point *point, const Data::GeometricShape *shape) :
	Builder()
{
	this->setDimension(dimension);
	this->setPoint(point);
	this->setShape(shape);

	this->decision = Data::UNKNOWN;
}

Performer::PointInObjectDecider::~PointInObjectDecider()
{

}

void Performer::PointInObjectDecider::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void Performer::PointInObjectDecider::setPoint(const Data::Point *point)
{
	this->point = point;
}

void Performer::PointInObjectDecider::setShape(const Data::GeometricShape *shape)
{
	this->shape = shape;
}

enum Data::Position Performer::PointInObjectDecider::getDecision() const
{
	return this->decision;
}

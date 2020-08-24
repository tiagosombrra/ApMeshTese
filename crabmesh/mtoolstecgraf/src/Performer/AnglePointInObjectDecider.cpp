#include "Performer/AnglePointInObjectDecider.h"

#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"

using namespace Performer;

Performer::AnglePointInObjectDecider::AnglePointInObjectDecider(UInt dimension,
	Data::Point *point, Data::GeometricShape *shape) :
	PointInObjectDecider(dimension, point, shape)
{
	this->ang = 0.0;
}

Performer::AnglePointInObjectDecider::~AnglePointInObjectDecider()
{
	//this->point = NULL;
}

Real Performer::AnglePointInObjectDecider::getAngle() const
{
	return this->ang;
}

Real Performer::AnglePointInObjectDecider::angle(const Data::GeometricShape *shape) const
{
	if (this->dimension == 2)
	{
		return static_cast<const Data::Edge2D *>(shape)->orientedAngle(static_cast<const Data::Point2D *>(this->point), false);
	}

	if (this->dimension == 3)
	{
		return static_cast<const Data::Triangle3D *>(shape)->orientedSolidAngle(static_cast<const Data::Point3D *>(this->point), false);
	}

	return 0.0;
}

void Performer::AnglePointInObjectDecider::decide()
{
	Real testAngle = (this->dimension == 2) ? 2.0*M_PI : 4.0*M_PI;

	//if the (solid) angle is equal to the circle (sphere) angle (surface),
	// than the point is inside the object. otherwise, the point is outside
	// the object

	this->decision = (std::fabs(this->ang - testAngle) <= Data::getTolerance()) ? Data::IN : Data::OUT;
}

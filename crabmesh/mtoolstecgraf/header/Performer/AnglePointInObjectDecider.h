#ifndef _PERFORMER_ANGLE_POINT_IN_OBJECT_DECIDER_H_
#define _PERFORMER_ANGLE_POINT_IN_OBJECT_DECIDER_H_

#include "Performer/Definitions.h"
#include "Performer/PointInObjectDecider.h"

namespace Performer
{
	class AnglePointInObjectDecider : public PointInObjectDecider
	{
	public:

		AnglePointInObjectDecider(UInt dimension, Data::Point *point = NULL,
			Data::GeometricShape *shape = NULL);
		virtual ~AnglePointInObjectDecider();

		virtual Real getAngle() const;

		virtual Real angle(const Data::GeometricShape *shape) const;

		virtual void decide();

	protected:

		Real ang;
	};
}

#endif //#ifndef _PERFORMER_ANGLE_POINT_IN_OBJECT_DECIDER_H_

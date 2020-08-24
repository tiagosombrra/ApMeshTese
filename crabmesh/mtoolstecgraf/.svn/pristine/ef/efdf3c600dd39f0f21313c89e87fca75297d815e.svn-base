#ifndef _PERFORMER_POINT_IN_OBJECT_DECIDER_H_
#define _PERFORMER_POINT_IN_OBJECT_DECIDER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class PointInObjectDecider : public Builder
	{
	public:

		PointInObjectDecider(UInt dimension = 0, const Data::Point *point = NULL,
			const Data::GeometricShape *shape = NULL);
		virtual ~PointInObjectDecider();

		virtual void setDimension(UInt dimension);

		virtual void setPoint(const Data::Point *point);
		virtual void setShape(const Data::GeometricShape *shape);

		virtual enum Data::Position getDecision() const;

		virtual void decide() = 0;

	protected:

		UInt dimension;

		const Data::Point *point;
		const Data::GeometricShape *shape;

		enum Data::Position decision;
	};
}

#endif //#ifndef _PERFORMER_POINT_IN_OBJECT_DECIDER_H_

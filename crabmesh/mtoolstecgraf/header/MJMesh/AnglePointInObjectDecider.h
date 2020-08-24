#ifndef _MJMESH_ANGLE_POINT_IN_OBJECT_DECIDER_H_
#define _MJMESH_ANGLE_POINT_IN_OBJECT_DECIDER_H_

#include "MJMesh/Definitions.h"

#include "Performer/AnglePointInObjectDecider.h"

namespace MJMesh
{
	class AnglePointInObjectDecider : public Performer::AnglePointInObjectDecider
	{
	public:

		AnglePointInObjectDecider(UInt dimension,
			Data::Point *point = NULL,
			const Data::FrontElementList *fels = NULL,
			bool canBeUnknown = true,
			UInt surfaceDirection = UINT_MAX);
		virtual ~AnglePointInObjectDecider();

		virtual void setFels(const Data::FrontElementList *fels);

		virtual Real angle(const Data::GeometricShape *shape) const;

		virtual void decide();

		virtual void execute();

	protected:

		const Data::FrontElementList *fels;

		bool canBeUnknown;
		UInt surfaceDirection;
	};
}

#endif //#ifndef _MJMESH_ANGLE_POINT_IN_OBJECT_DECIDER_H_

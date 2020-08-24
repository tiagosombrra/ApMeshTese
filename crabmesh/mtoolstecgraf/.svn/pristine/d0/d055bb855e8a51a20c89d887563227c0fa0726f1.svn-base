#ifndef _PERFORMER_BOUNDARY_NORMAL_INVERTER_H_
#define _PERFORMER_BOUNDARY_NORMAL_INVERTER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryNormalInverter : public Performer::Builder
	{
	public:

		BoundaryNormalInverter();
		virtual ~BoundaryNormalInverter();

		virtual void setBoundary(Data::Boundary *boundary);

		virtual void execute();

	protected:

		virtual void invert(Data::FrontElement *fel);

	protected:

		Data::Boundary *boundary;
	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_NORMAL_INVERTER_H_

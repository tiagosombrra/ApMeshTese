#ifndef _PERFORMER_BOUNDARY_BUILDER_H_
#define _PERFORMER_BOUNDARY_BUILDER_H_

#include "Performer/Definitions.h"

#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryBuilder : virtual public Performer::Builder
	{
	public:

		BoundaryBuilder();
		virtual ~BoundaryBuilder();

//#if USE_GUI
		virtual void setBoundary(Data::Boundary *boundary);
//#endif //#if USE_GUI

		virtual Data::Boundary *getBoundary() const;

		virtual void execute() = 0;

	protected:

		Data::Boundary *boundary;
	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_BUILDER_H_

#ifndef _TOOLS_BOUNDARY_NORMAL_INVERTER_H_
#define _TOOLS_BOUNDARY_NORMAL_INVERTER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/BoundaryNormalInverter.h"

namespace Tools
{
	class BoundaryNormalInverter :
		public Performer::BoundaryNormalInverter,
		public Tools::Tool
	{
	public:

		BoundaryNormalInverter();
		virtual ~BoundaryNormalInverter();

		virtual void execute();

	protected:

		virtual void invert(Data::FrontElement *fel);
	};
}

#endif //#ifndef _TOOLS_BOUNDARY_NORMAL_INVERTER_H_

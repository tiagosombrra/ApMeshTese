#ifndef _TOOLS_BOUNDARY_WATER_TIGHTNESS_CHECKER_H_
#define _TOOLS_BOUNDARY_WATER_TIGHTNESS_CHECKER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/BoundaryWaterTightnessChecker.h"

namespace Tools
{
	class BoundaryWaterTightnessChecker :
		public Performer::BoundaryWaterTightnessChecker,
		public Tools::Tool
	{
	public:

		BoundaryWaterTightnessChecker();
		virtual ~BoundaryWaterTightnessChecker();

		virtual void execute();

	protected:

		using Performer::BoundaryWaterTightnessChecker::check;
		virtual void check(AdjacencyMap &map);
		virtual void check(const AdjacencyMap::iterator &iter);

	};
}

#endif //#ifndef _TOOLS_BOUNDARY_WATER_TIGHTNESS_CHECKER_H_

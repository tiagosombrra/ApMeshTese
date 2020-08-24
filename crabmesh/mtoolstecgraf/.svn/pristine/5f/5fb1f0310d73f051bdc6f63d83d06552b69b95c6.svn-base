#ifndef _TOOLS_ADJACENCIES_IDENTIFIER_H_
#define _TOOLS_ADJACENCIES_IDENTIFIER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Performer/AdjacenciesIdentifier.h"

namespace Tools
{
	class AdjacenciesIdentifier :
		public Performer::AdjacenciesIdentifier,
		public Tools::ExternalTool
	{
	public:

		AdjacenciesIdentifier(const Tool *tool = NULL);
		virtual ~AdjacenciesIdentifier();

		virtual void execute();

	protected:

		virtual bool identifyAdjacencies(Data::/*Front*/Element *fel);
	};
}

#endif //#ifndef _TOOLS_ADJACENCIES_IDENTIFIER_H_

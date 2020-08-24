#ifndef _TOOLS_PRINCIPAL_COMPONENTS_ANALYZER_H_
#define _TOOLS_PRINCIPAL_COMPONENTS_ANALYZER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Performer/PrincipalComponentsAnalyzer.h"

#if USE_NUMERICAL

namespace Tools
{
	class PrincipalComponentsAnalyzer :
		public Performer::PrincipalComponentsAnalyzer,
		public Tools::ExternalTool
	{
	public:

		PrincipalComponentsAnalyzer(const Tool *tool = NULL);
		virtual ~PrincipalComponentsAnalyzer();

		virtual Data::Mesh *getMesh() const;

		virtual void execute();

    protected:

        Data::Mesh *mesh;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _TOOLS_PRINCIPAL_COMPONENTS_ANALYZER_H_

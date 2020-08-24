#ifndef _TOOLS_MESH_INTERCEPTION_CHECKER_H_
#define _TOOLS_MESH_INTERCEPTION_CHECKER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/MeshInterceptionChecker.h"

namespace Tools
{
	class MeshInterceptionChecker :
		public Performer::MeshInterceptionChecker,
		public Tools::Tool
	{
	public:

		MeshInterceptionChecker();
		virtual ~MeshInterceptionChecker();

		virtual const Data::FrontElementList &getSmall() const;

		virtual void execute();

	protected:

		virtual void checkInterceptions();

		virtual Data::FrontElement *makeFrontElement(const Data::VertexVector &vertices, Data::Element *el) const;

		virtual void adjustTopology(Data::FrontElement *fel, Data::Element *el) const;

	protected:

		Data::FrontElementList small;

	};
}

#endif //#ifndef _TOOLS_MESH_INTERCEPTION_CHECKER_H_

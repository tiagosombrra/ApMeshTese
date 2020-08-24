#ifndef _TOOLS_CRACKS_IDENTIFIER_H_
#define _TOOLS_CRACKS_IDENTIFIER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Performer/CracksIdentifier.h"

namespace Tools
{
	class CracksIdentifier :
		public Performer::CracksIdentifier,
		public Tools::ExternalTool
	{
	public:

		CracksIdentifier(const Tool *tool = NULL);
		virtual ~CracksIdentifier();

		virtual void execute();

	protected:

		virtual void createFront();

		virtual Data::FrontElement *findCrackFel(Data::Boundary::FrontElementIterator iter, const Data::FrontElement *fel) const;

		virtual void checkForCrack(Data::Boundary::FrontElementIterator iter, Data::FrontElement *fel);

	protected:

		MJMesh::Front *front;
		Data::BoxTree *tree;
		Data::FrontElementList small;

	};
}

#endif //#ifndef _TOOLS_CRACKS_IDENTIFIER_H_

#ifndef _TOOLS_CRACK_REMOVER_H_
#define _TOOLS_CRACK_REMOVER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/CrackRemover.h"

namespace Tools
{
	class CrackRemover :
		public Performer::CrackRemover,
		public Tools::Tool
	{
	public:

		CrackRemover();
		virtual ~CrackRemover();

		virtual void execute();

	protected:

		virtual Performer::CracksIdentifier *makeCracksIdentifier() const;

		virtual void removeCrack(Performer::CracksIdentifier::CrackMap::iterator &iter);
		virtual void removeCracks();

		virtual void replace(Data::FrontElement *fel, Data::Vertex *oldV, Data::Vertex *newV) const;
		virtual void replace(Data::Element *e, Data::Vertex *oldV, Data::Vertex *newV) const;
	};
}

#endif //#ifndef _TOOLS_CRACK_REMOVER_H_

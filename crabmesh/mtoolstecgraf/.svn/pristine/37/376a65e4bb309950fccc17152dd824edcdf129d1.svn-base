#ifndef _PERFORMER_CRACKS_IDENTIFIER_H_
#define _PERFORMER_CRACKS_IDENTIFIER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Data/Boundary.h"

namespace Performer
{
	class CracksIdentifier : public Performer::Builder
	{
	public:

#if USE_C__11
		typedef ECrackHashMap CrackMap;
#else
		typedef ECrackMap CrackMap;
#endif //#if USE_C__11

	public:

		CracksIdentifier();
		virtual ~CracksIdentifier();

		virtual void setBoundary(Data::Boundary *boundary);
		virtual void setUseTolerance(bool useTolerance);

		virtual CrackMap *getCracks() const;

		virtual bool equal(const Data::Vertex *other, const Data::Vertex *vertex) const;
		virtual bool equal(const Data::FrontElement *other, const Data::FrontElement *fel) const;

		virtual void execute();

	protected:

		virtual Data::FrontElement *findCrackFel(Data::Boundary::FrontElementIterator iter,
			const Data::FrontElement *fel) const;

		virtual void checkForCrack(Data::Boundary::FrontElementIterator iter, Data::FrontElement *fel);

	protected:

		CrackMap *cracks;

		Data::Boundary *boundary;

		bool useTolerance;
	};
}

#endif //#ifndef _PERFORMER_CRACKS_IDENTIFIER_H_

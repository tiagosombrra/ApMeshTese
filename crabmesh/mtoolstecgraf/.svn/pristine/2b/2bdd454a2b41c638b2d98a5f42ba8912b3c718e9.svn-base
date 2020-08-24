#ifndef _PERFORMER_CRACK_REMOVER_H_
#define _PERFORMER_CRACK_REMOVER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Performer/CracksIdentifier.h"

namespace Performer
{
	class CrackRemover : public Performer::Builder
	{
	public:

		CrackRemover();
		virtual ~CrackRemover();

		virtual void setBoundary(Data::Boundary *boundary);
		virtual void setMesh(Data::Mesh *mesh);
		virtual void setUseTolerance(bool useTolerance);

		virtual bool isAnyCrackRemoved() const;

		virtual void execute();

	protected:

		virtual CracksIdentifier *makeCracksIdentifier() const;

		virtual void removeCrack(CracksIdentifier::CrackMap::iterator &iter);
		virtual void removeCracks();

		virtual bool isCrackFel(const Data::FrontElement *fel) const;

		virtual void findAdjacentsNotInCrack(Data::FrontElementList &adjs) const;

		virtual void replace(Data::FrontElement *fel, Data::Vertex *oldV, Data::Vertex *newV) const;
		virtual void replace(Data::Element *e, Data::Vertex *oldV, Data::Vertex *newV) const;

	protected:

		CracksIdentifier::CrackMap *cracks;

		Data::Boundary *boundary;
		Data::Mesh *mesh;

		bool anyCrackRemoved;
		bool useTolerance;

		CracksIdentifier *identifier;
	};
}

#endif //#ifndef _PERFORMER_CRACK_REMOVER_H_

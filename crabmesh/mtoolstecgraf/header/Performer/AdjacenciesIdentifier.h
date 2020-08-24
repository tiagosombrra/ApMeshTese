#ifndef _PERFORMER_ADJACENCIES_IDENTIFIER_H_
#define _PERFORMER_ADJACENCIES_IDENTIFIER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class AdjacenciesIdentifier : public Builder::Builder
	{
	public:
#if USE_C__11
		typedef VEHashMap VertexAdjacencyMap;
		typedef EVEMapHashMap AdjacencyMap;
#else
		typedef VEMap VertexAdjacencyMap;
		typedef EVEMapMap AdjacencyMap;
#endif //USE_C__11

	public:

		AdjacenciesIdentifier();
		virtual ~AdjacenciesIdentifier();

		virtual void setBoundary(const Data::Boundary *boundary);
		virtual void setSurface(const Data::Mesh *surface);

		virtual void setAllowHangingVertices(bool allowHangingVertices);

		virtual const AdjacencyMap &getAdjacencies() const;
		virtual void getAdjacenciesSwapping(AdjacencyMap &adjacencies);

		virtual void execute();

	protected:

		virtual bool identifyAdjacencies(Data::/*Front*/Element *fel);

	protected:

		UInt dimension;

		const Data::Boundary *boundary;
		const Data::Mesh *surface;

		AdjacencyMap adjacencies;

		bool allowHangingVertices;
	};
}

#endif //#ifndef _PERFORMER_ADJACENCIES_IDENTIFIER_H_

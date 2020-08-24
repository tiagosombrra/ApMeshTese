#ifndef _PERFORMER_EDGE_SWAP_BOUNDARY_ADAPTER_H_
#define _PERFORMER_EDGE_SWAP_BOUNDARY_ADAPTER_H_

#include "Performer/Definitions.h"
#include "Performer/BoundaryAdapter.h"
#include "Performer/AdjacenciesIdentifier.h"

namespace Performer
{
	class EdgeSwapBoundaryAdapter : public Performer::BoundaryAdapter
	{
	public:

		EdgeSwapBoundaryAdapter(Data::Boundary *boundary = NULL);
		EdgeSwapBoundaryAdapter(Data::Mesh *surface);
		virtual ~EdgeSwapBoundaryAdapter();

		virtual void setSizeProportion(Real sizeProportion);
		virtual void setQualityThreshold(Real qualityThreshold);
		virtual void setNormalTestEnabled(bool normalTestEnabled);
		virtual void setChangeSurfaceBoundaryEnabled(bool changeSurfaceBoundaryEnabled);
		virtual void setMaxEdgesOnBoundary(UInt maxEdgesOnBoundary);

		virtual bool isAdapted() const;
		virtual bool isOk() const;

		virtual void execute();

	protected:

		virtual bool adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual bool swap(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
			Data::Vertex *v1, Data::Vertex *v2, Data::Vertex *v3, Data::Vertex *v4) const;

		virtual Real quality(const Data::Triangle *t) const;
		virtual bool shouldSwap(const Data::Triangle *t, UInt priority[]) const;
		virtual bool canSwap(const Data::/*Front*/Element *fel, UInt i) const;

		virtual bool addToElements(const Data::/*Front*/Element *fel) const;
		virtual bool removeFromElements(const Data::/*Front*/Element *fel) const;

		virtual AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;
		virtual bool identifyAdjacencies();
		virtual void identifyQualities();
		virtual void executeAdaptation();

	protected:

		///It has to be a map, because it has to be sorted
		typedef std::multimap<Real, Data::FrontElement *> RealFEMultiMap;
		typedef std::multimap<Real, Data::/*Front*/Element *> RealEMultiMap;

	protected:

		UInt dimension;

		Real sizeProportion;
		Real qualityThreshold;
		bool normalTestEnabled;
		bool changeSurfaceBoundaryEnabled;
		UInt maxEdgesOnBoundary;

		mutable AdjacenciesIdentifier::AdjacencyMap adjacencies;

		mutable RealEMultiMap elements;

		bool ok;
		mutable bool adapted;
	};
}

#endif //#ifndef _PERFORMER_EDGE_SWAP_BOUNDARY_ADAPTER_H_

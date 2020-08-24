#ifndef _PERFORMER_EDGE_COLLAPSE_BOUNDARY_ADAPTER_H_
#define _PERFORMER_EDGE_COLLAPSE_BOUNDARY_ADAPTER_H_

#include "Performer/Definitions.h"
#include "Performer/BoundaryAdapter.h"
#include "Performer/AdjacenciesIdentifier.h"

namespace Performer
{
	class EdgeCollapseBoundaryAdapter : public Performer::BoundaryAdapter
	{
	public:

		EdgeCollapseBoundaryAdapter(Data::Boundary *boundary = NULL, Data::Mesh *mesh = NULL);
		EdgeCollapseBoundaryAdapter(Data::Mesh *surface);
		virtual ~EdgeCollapseBoundaryAdapter();

		virtual void setSizeProportion(Real sizeProportion);
		virtual void setQualityThreshold(Real qualityThreshold);
		virtual void setOnlyInPlane(bool onlyInPlane);
		virtual void setAllowHangingVertices(bool allowHangingVertices);

		virtual bool isAdapted() const;
		virtual bool isOk() const;

		virtual void execute();

	protected:

		virtual bool adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual Real hangingAngle(Data::Vertex *v, Data::/*Front*/ElementList &adjacency) const;
		virtual UInt hanging(Data::Vertex *v, Data::/*Front*/ElementList &adjacency) const;
		virtual bool inPlane(Data::Vertex *v, Data::/*Front*/ElementList &adjacency) const;

		virtual bool collapse(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj, Data::Vertex *v1, Data::Vertex *v2) const;
		virtual bool deletes(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj, Data::Vertex *v1, Data::Vertex *v2) const;

		virtual Real quality(const Data::Triangle *t) const;
		virtual UInt shouldCollapse(const Data::Triangle *t) const;
		virtual bool canCollapse(const Data::/*Front*/Element *fel, UInt i) const;

		virtual bool addToElements(Data::/*Front*/Element *fel) const;
		virtual bool removeFromElements(const Data::/*Front*/Element *fel) const;

		virtual AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;
		virtual bool identifyAdjacencies();

		virtual void identifyQualities();
		virtual void executeAdaptation();

	protected:

		///It has to be a map, because it has to be sorted
		typedef std::multimap<Real, Data::FrontElement *> RealFEMultiMap;
		typedef std::multimap<Real, Data::Element *> RealEMultiMap;

	protected:

		UInt dimension;

		bool onlyInPlane;

		Real sizeProportion;
		Real qualityThreshold;

		mutable AdjacenciesIdentifier::AdjacencyMap adjacencies;

		mutable RealEMultiMap elements;

		bool ok;
		mutable bool adapted;

		Data::Mesh *mesh;

		bool allowHangingVertices;

	};
}

#endif //#ifndef _PERFORMER_EDGE_COLLAPSE_BOUNDARY_ADAPTER_H_

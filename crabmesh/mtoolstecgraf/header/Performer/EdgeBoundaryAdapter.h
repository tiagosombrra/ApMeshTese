#ifndef _PERFORMER_EDGE_BOUNDARY_ADAPTER_H_
#define _PERFORMER_EDGE_BOUNDARY_ADAPTER_H_

#include "Performer/Definitions.h"
#include "Performer/BoundaryAdapter.h"
#include "Performer/AdjacenciesIdentifier.h"

namespace Performer
{
	class EdgeBoundaryAdapter : public Performer::BoundaryAdapter
	{
	public:

		EdgeBoundaryAdapter(Data::Boundary *boundary = NULL);
		EdgeBoundaryAdapter(Data::Mesh *surface);
		virtual ~EdgeBoundaryAdapter();

		virtual void setSizeProportion(Real sizeProportion);
		virtual void setQualityThreshold(Real qualityThreshold);
		virtual void setMesh(Data::Mesh *mesh);

		virtual bool isAdapted() const;
		virtual bool isOk() const;

		virtual void execute();

	protected:

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		//virtual bool adaptEdge2D(Data::FrontElement *element, Data::Vertex *mid, Data::FrontElementList &newElements, Data::VertexList &newVertices) const;
		virtual bool adaptTriangle3D(Data::/*Front*/Element *element,
            Data::Vertex *vs[3], Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual Data::Vertex *makeVertex(const Data::Point &pt) const;
		virtual Data::/*Front*/Element *makeElement(const Data::VertexVector &vertices) const = 0;

		virtual Real quality(const Data::Triangle *t) const;
		virtual bool shouldRefine(const Data::/*Front*/Element *fel);

		virtual void removeFromElements(const Data::/*Front*/Element *fel);

		virtual AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;
		virtual bool identifyAdjacencies();

		virtual bool addToElements(Data::/*Front*/Element *fel) const;
		virtual void identifyQualities();

		virtual void identifyRefinements();
		virtual void executeAdaptation();

	protected:

#if USE_C__11
		typedef std::unordered_map<Data::FrontElement *, Data::BoolVector> FEBoolHashMap;
		typedef std::unordered_map<Data::Element *, Data::BoolVector> EBoolHashMap;
#else
		typedef std::unordered_map<Data::FrontElement *, Data::BoolVector> FEBoolMap;
		typedef std::unordered_map<Data::Element *, Data::BoolVector> EBoolMap;
#endif //#if USE_C__11

#if USE_C__11
		typedef std::unordered_map<ULInt, std::unordered_map<ULInt, Data::Vertex *> > VertexMatrixHashMap;
#endif //#if USE_C__11

		typedef std::map<ULInt, std::map<ULInt, Data::Vertex *> > VertexMatrixMap;

	protected:

		UInt dimension;

		Real sizeProportion;
		Real qualityThreshold;

		mutable AdjacenciesIdentifier::AdjacencyMap adjacencies;

		mutable Data::/*Front*/ElementList elements;

#if USE_C__11
		EBoolHashMap refinements;
#else
		EBoolMap refinements;
#endif //#if USE_C__11

#if USE_C__11
		mutable VertexMatrixHashMap mids;
#else
		mutable VertexMatrixMap mids;
#endif //#if USE_C__11

		Data::Mesh *mesh;

		bool ok;
		bool adapted;
	};
}

#endif //#ifndef _PERFORMER_EDGE_BOUNDARY_ADAPTER_H_

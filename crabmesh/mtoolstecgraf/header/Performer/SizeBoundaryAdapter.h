#ifndef _PERFORMER_SIZE_BOUNDARY_ADAPTER_H_
#define _PERFORMER_SIZE_BOUNDARY_ADAPTER_H_

#include "Performer/Definitions.h"
#include "Performer/BoundaryAdapter.h"
#include "Performer/AdjacenciesIdentifier.h"

namespace Performer
{
	class SizeBoundaryAdapter : public Performer::BoundaryAdapter
	{
	public:

		SizeBoundaryAdapter(Data::Boundary *boundary = NULL);
		SizeBoundaryAdapter(Data::Mesh *surface);
		virtual ~SizeBoundaryAdapter();

		virtual void setMaxAdaptations(UInt maxAdaptations);
		virtual void setSizeProportion(Real sizeProportion);
		virtual void setMesh(Data::Mesh *mesh);

		virtual bool adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual bool isAdapted() const;
		virtual bool isOk() const;

		virtual void execute();

	protected:

		virtual AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;
		//virtual bool identifyAdjacencies(Data::FrontElement *fel);
		virtual bool identifyAdjacencies();

		virtual void checkAdjacenciesSizes(const AdjacenciesIdentifier::AdjacencyMap::iterator &iter);
		virtual void checkAdjacenciesSizes();

		virtual void executeAdaptation();

		virtual bool adaptEdge2D(Data::/*Front*/Element *element, Data::Vertex *mid, Data::/*Front*/ElementList &newElements) const;
		virtual bool adaptTriangle3D(Data::/*Front*/Element *element, Data::Vertex *mid, Data::/*Front*/ElementList &newElements) const;

		virtual Data::Vertex *makeVertex(const Data::Point &pt) const;
		virtual Data::/*Front*/Element *makeElement(const Data::VertexVector &vertices) const = 0;

	protected:

		UInt dimension;

		UInt maxAdaptations;

		Real sizeProportion;

		mutable AdjacenciesIdentifier::AdjacencyMap adjacencies;

		Data::ElementList toAdapt;

		Data::Mesh *mesh;

		bool ok;
		bool adapted;
	};
}

#endif //#ifndef _PERFORMER_SIZE_BOUNDARY_ADAPTER_H_

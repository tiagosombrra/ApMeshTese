#ifndef _TOOLS_EDGE_BOUNDARY_ADAPTER_H_
#define _TOOLS_EDGE_BOUNDARY_ADAPTER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/EdgeBoundaryAdapter.h"
#include "Performer/CracksIdentifier.h"

namespace Tools
{
	class EdgeBoundaryAdapter :
		public Performer::EdgeBoundaryAdapter,
		public Tools::Tool
	{
	public:

		EdgeBoundaryAdapter(Data::Boundary *boundary = NULL);
		EdgeBoundaryAdapter(Data::Mesh *surface);
		virtual ~EdgeBoundaryAdapter();

		virtual void setNoCrack(bool noCrack);

		//virtual bool adapt(Data::FrontElement *element, Data::FrontElementList &newElements, Data::VertexList &newVertices) const;

		virtual void execute();

	protected:

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual bool adaptTriangle3D(Data::/*Front*/Element *element,
            Data::Vertex *vs[3], Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		//virtual void adjustTopologyEdge2D(Data::FrontElement *element, Data::FrontElementList &newElements) const;
		virtual void adjustTopologyTriangle3D(Data::/*Front*/Element *element,
            Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements) const;

		virtual Data::Vertex *makeVertex(const Data::Point &pt) const;
		virtual Data::/*Front*/Element *makeElement(const Data::VertexVector &vertices) const;

		virtual bool addToElements(Data::/*Front*/Element *fel) const;
		virtual void identifyQualities();

		virtual Performer::AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;

		virtual Performer::CracksIdentifier *makeCracksIdentifier() const;

		virtual void identifyCracks();

		virtual bool shouldRefine(const Data::/*Front*/Element *fel);

		virtual void identifyRefinements();

		virtual void executeAdaptation();

	protected:

		/*struct CrackFrontElement
		{
			Data::FrontElement *fel1;
			Data::FrontElement *fel2;
			//Data::UIntVector map;
			bool done;
		};

#if USE_C__11
		typedef std::map<Data::FrontElement *, CrackFrontElement> CrackFrontElementHashMap;
#else
		typedef std::map<Data::FrontElement *, CrackFrontElement> CrackFrontElementMap;
#endif //#if USE_C__11*/

	protected:

/*#if USE_C__11
		mutable CrackFrontElementHashMap cracks;
#else
		mutable CrackFrontElementMap cracks;
#endif //#if USE_C__11*/

		mutable Performer::CracksIdentifier::CrackMap *cracks;

		bool noCrack;

	};
}

#endif //#ifndef _TOOLS_EDGE_BOUNDARY_ADAPTER_H_

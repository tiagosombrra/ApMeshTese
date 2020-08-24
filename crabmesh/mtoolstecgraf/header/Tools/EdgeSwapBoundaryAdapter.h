#ifndef _TOOLS_EDGE_SWAP_BOUNDARY_ADAPTER_H_
#define _TOOLS_EDGE_SWAP_BOUNDARY_ADAPTER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/EdgeSwapBoundaryAdapter.h"
#include "Performer/CracksIdentifier.h"

namespace Tools
{
	class EdgeSwapBoundaryAdapter :
		public Performer::EdgeSwapBoundaryAdapter,
		public Tools::Tool
	{
	public:

		EdgeSwapBoundaryAdapter(Data::Boundary *boundary = NULL);
		EdgeSwapBoundaryAdapter(Data::Mesh *surface);
		virtual ~EdgeSwapBoundaryAdapter();

		virtual void setNoCrack(bool noCrack);

		virtual void execute();

	protected:

		virtual void adjustTopology(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
			Data::Vertex *v1, Data::Vertex *v2, Data::Vertex *v3, Data::Vertex *v4) const;

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual bool swap(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
			Data::Vertex *v1, Data::Vertex *v2, Data::Vertex *v3, Data::Vertex *v4) const;

		virtual Performer::AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;

		virtual bool addToElements(const Data::/*Front*/Element *fel) const;
		virtual bool removeFromElements(const Data::/*Front*/Element *fel) const;
		virtual void identifyQualities();

		virtual void executeAdaptation();

		virtual Performer::CracksIdentifier *makeCracksIdentifier() const;

		virtual void identifyCracks();

	/*protected:

		struct CrackFrontElement
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

#endif //#ifndef _TOOLS_EDGE_SWAP_BOUNDARY_ADAPTER_H_

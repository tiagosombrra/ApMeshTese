#ifndef _TOOLS_EDGE_COLLAPSE_BOUNDARY_ADAPTER_H_
#define _TOOLS_EDGE_COLLAPSE_BOUNDARY_ADAPTER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/EdgeCollapseBoundaryAdapter.h"
#include "Performer/CracksIdentifier.h"

namespace Tools
{
	class EdgeCollapseBoundaryAdapter :
		public Performer::EdgeCollapseBoundaryAdapter,
		public Tools::Tool
	{
	public:

		EdgeCollapseBoundaryAdapter(Data::Boundary *boundary = NULL, Data::Mesh *mesh = NULL);
		EdgeCollapseBoundaryAdapter(Data::Mesh *surface);
		virtual ~EdgeCollapseBoundaryAdapter();

		virtual void setNoCrack(bool noCrack);

		virtual void execute();

	protected:

		virtual void adjustTopology(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
			Data::Vertex *v1, Data::Vertex *v2) const;

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual bool collapse(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
			Data::Vertex *v1, Data::Vertex *v2) const;

		virtual bool canCollapse(const Data::/*Front*/Element *fel, UInt i) const;

		virtual bool addToElements(Data::/*Front*/Element *fel) const;

		virtual void identifyQualities();

		virtual void executeAdaptation();

		virtual Performer::AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;

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

#endif //#ifndef _TOOLS_EDGE_COLLAPSE_BOUNDARY_ADAPTER_H_

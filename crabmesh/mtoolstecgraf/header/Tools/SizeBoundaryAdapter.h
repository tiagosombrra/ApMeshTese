#ifndef _TOOLS_SIZE_BOUNDARY_ADAPTER_H_
#define _TOOLS_SIZE_BOUNDARY_ADAPTER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/SizeBoundaryAdapter.h"
#include "Performer/CracksIdentifier.h"

namespace Tools
{
	class SizeBoundaryAdapter :
		public Performer::SizeBoundaryAdapter,
		public Tools::Tool
	{
	public:

		SizeBoundaryAdapter(MJMesh::Boundary *boundary = NULL);
		SizeBoundaryAdapter(MJMesh::Mesh *surface);
		virtual ~SizeBoundaryAdapter();

		virtual void setNoCrack(bool noCrack);

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual void execute();

	protected:

		virtual void adjustTopologyEdge2D(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements) const;
		virtual void adjustTopologyTriangle3D(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements) const;

		virtual Data::Vertex *makeVertex(const Data::Point &pt) const;
		virtual Data::/*Front*/Element *makeElement(const Data::VertexVector &vertices) const;

		virtual Performer::CracksIdentifier *makeCracksIdentifier() const;

		virtual void identifyCracks();

		virtual Performer::AdjacenciesIdentifier *makeAdjacenciesIdentifier() const;
		//virtual bool identifyAdjacencies(Data::FrontElement *fel);
		//virtual bool identifyAdjacencies();

		virtual void checkAdjacenciesSizes(const Performer::AdjacenciesIdentifier::AdjacencyMap::iterator &iter);
		virtual void checkAdjacenciesSizes();

		virtual void executeAdaptation();

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

#endif //#ifndef _TOOLS_SIZE_BOUNDARY_ADAPTER_H_

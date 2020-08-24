#ifndef _TOOLS_UNIFORM_BOUNDARY_ADAPTER
#define _TOOLS_UNIFORM_BOUNDARY_ADAPTER

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/UniformBoundaryAdapter.h"
#include "Performer/CracksIdentifier.h"

namespace Tools
{
	class UniformBoundaryAdapter :
		public Performer::UniformBoundaryAdapter,
		public Tools::Tool
	{
	public:

		UniformBoundaryAdapter(MJMesh::Boundary *boundary = NULL);
		UniformBoundaryAdapter(MJMesh::Mesh *surface);
		virtual ~UniformBoundaryAdapter();

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

	protected:

		mutable Performer::CracksIdentifier::CrackMap *cracks;

		bool noCrack;

	};
}

#endif //#ifndef _TOOLS_UNIFORM_BOUNDARY_ADAPTER

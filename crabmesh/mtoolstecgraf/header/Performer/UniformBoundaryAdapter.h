#ifndef _PERFORMER_UNIFORM_BOUNDARY_ADAPTER_H_
#define _PERFORMER_UNIFORM_BOUNDARY_ADAPTER_H_

#include "Performer/Definitions.h"
#include "Performer/BoundaryAdapter.h"

namespace Performer
{
	class UniformBoundaryAdapter : public Performer::BoundaryAdapter
	{
	public:

		UniformBoundaryAdapter(Data::Boundary *boundary = NULL);
		UniformBoundaryAdapter(Data::Mesh *surface);
		virtual ~UniformBoundaryAdapter();

		virtual void setMesh(Data::Mesh *mesh);

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const;

		virtual void execute();

	protected:

		virtual bool adaptEdge2D(Data::/*Front*/Element *element,
            Data::Vertex *mid, Data::/*Front*/ElementList &newElements) const;
		virtual bool adaptTriangle3D(Data::/*Front*/Element *element,
            Data::Vertex *vs[3], Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements) const;

		virtual Data::Vertex *makeVertex(const Data::Point &pt) const;
		virtual Data::/*Front*/Element *makeElement(const Data::VertexVector &vertices) const = 0;

	protected:

		UInt dimension;

#if USE_C__11
		typedef std::unordered_map<ULInt, std::unordered_map<ULInt, Data::Vertex *> > VertexMatrixHashMap;
#endif //#if USE_C__11

		typedef std::map<ULInt, std::map<ULInt, Data::Vertex *> > VertexMatrixMap;

#if USE_C__11
		mutable VertexMatrixHashMap mids;
#else
		mutable VertexMatrixMap mids;
#endif //#if USE_C__11

		Data::Mesh *mesh;
	};
}

#endif //#ifndef _PERFORMER_UNIFORM_BOUNDARY_ADAPTER_H_

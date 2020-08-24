#ifndef _MJMESH_ELEMENT_SURFACE_H_
#define _MJMESH_ELEMENT_SURFACE_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/Element.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class ElementSurface : public MJMesh::Element
#if USE_POOL
		, public Data::Poolable<MJMesh::ElementSurface>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::ElementSurface)
#endif //#if USE_POOL

	public:

		ElementSurface(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, ULInt id = 0);
#if USE_QUADS
		ElementSurface(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4, ULInt id = 0);
#endif //#if USE_QUADS
		virtual ~ElementSurface();

		virtual void setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3);
#if USE_QUADS
		virtual void setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4);
#endif //#if USE_QUADS

		virtual UInt numSides() const;
		virtual UInt numVerticesInSide(UInt side) const;
		virtual Data::Vertex *getVertexInSide(UInt vertex, UInt side) const;

		virtual void invert();

		virtual Real gamma() const;

		virtual Real quality() const;

		virtual bool isBad() const;

		virtual bool intercept(const Data::Element &e) const;
		virtual bool intercept(const Data::Element *e) const;

		virtual bool intercept(const Data::Edge &e) const;
		virtual bool intercept(const Data::Edge *e) const;

		virtual bool intercept(const Data::Point &p) const;
		virtual bool intercept(const Data::Point *p) const;

		using MJMesh::Element::have;
		virtual bool have(const Data::Edge &e) const;
		virtual bool have(const Data::Edge *e) const;

		virtual bool have(const Data::FrontElement &e) const;
		virtual bool have(const Data::FrontElement *e) const;

		virtual bool isAdjacent(const Data::Element &e) const;
		virtual bool isAdjacent(const Data::Element *e) const;

#if USE_ARRAY
		using MJMesh::Element::operator=;
#if USE_EXPERIMENTAL
		virtual ElementSurface &operator=(const MJMesh::ElementSurface &e);
#endif //#if USE_EXPERIMENTAL
#endif //#if USE_ARRAY

#if USE_EXPERIMENTAL
	protected:

#if USE_QUADS
		Data::Vertex *vertices[4];
#else
		Data::Vertex *vertices[3];
#endif //#if USE_QUADS
#endif //#if USE_EXPERIMENTA
	};
}

#endif //#ifndef _MJMESH_ELEMENT_SURFACE_H_

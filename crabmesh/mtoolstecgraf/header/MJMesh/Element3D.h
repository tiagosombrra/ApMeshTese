#ifndef _MJMESH_ELEMENT3D_H_
#define _MJMESH_ELEMENT3D_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/Element.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class Element3D : public MJMesh::Element
#if USE_POOL
		, public Data::Poolable<MJMesh::Element3D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::Element3D)
#endif //#if USE_POOL

	public:

		Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
			MJMesh::Vertex *v4, ULInt id = 0);
#if USE_BRICKS
		Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
			MJMesh::Vertex *v4, MJMesh::Vertex *v5, ULInt id = 0);
		Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
			MJMesh::Vertex *v4, MJMesh::Vertex *v5, MJMesh::Vertex *v6, ULInt id = 0);
		Element3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
			MJMesh::Vertex *v4, MJMesh::Vertex *v5, MJMesh::Vertex *v6,
			MJMesh::Vertex *v7, MJMesh::Vertex *v8, ULInt id = 0);
#endif //#if USE_BRICKS
		virtual ~Element3D();

		virtual void setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4);
#if USE_BRICKS
		virtual void setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
			MJMesh::Vertex *v5);
		virtual void setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
			MJMesh::Vertex *v5, MJMesh::Vertex *v6);
		virtual void setVertices(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, MJMesh::Vertex *v4,
			MJMesh::Vertex *v5, MJMesh::Vertex *v6, MJMesh::Vertex *v7, MJMesh::Vertex *v8);
#endif //#if USE_BRICKS

		virtual UInt numSides() const;
		//involves dynamic_cast<>
		virtual UInt numVerticesInSide(UInt side) const;
		//involves dynamic_cast<>
		virtual Data::Vertex *getVertexInSide(UInt vertex, UInt side) const;

		virtual Real gamma() const;

		virtual Real quality() const;

		virtual bool isBad() const;

		virtual bool intercept(const Data::Element &e) const;
		virtual bool intercept(const Data::Element *e) const;

		virtual bool intercept(const Data::Edge &e) const;
		virtual bool intercept(const Data::Edge *e) const;

		virtual bool intercept(const Data::Point &p) const;
		virtual bool intercept(const Data::Point *p) const;

		using Data::Element::have;
		virtual bool have(const Data::Edge &e) const;
		virtual bool have(const Data::Edge *e) const;

		//virtual bool have(const Data::Triangle &t) const;
		//virtual bool have(const Data::Triangle *t) const;
        virtual bool have(const Data::Polygon &p) const;
		virtual bool have(const Data::Polygon *p) const;

		virtual bool have(const Data::FrontElement &e) const;
		virtual bool have(const Data::FrontElement *e) const;

		virtual bool have(const MJMesh::FrontElement3D &e) const;
		virtual bool have(const MJMesh::FrontElement3D *e) const;

		virtual bool isAdjacent(const Data::Element &e) const;
		virtual bool isAdjacent(const Data::Element *e) const;

#if USE_ARRAY
		using MJMesh::Element::operator=;
#if USE_EXPERIMENTAL
		virtual Element3D &operator=(const MJMesh::Element3D &e);
#endif //#if USE_EXPERIMENTAL
#endif //#if USE_ARRAY

#if USE_EXPERIMENTAL
	protected:

#if USE_BRICKS
		Data::Vertex *vertices[8];
#else
		Data::Vertex *vertices[4];
#endif //#if USE_BRICKS
#endif //#if USE_EXPERIMENTA
	};
}

#endif //#ifndef _MJMESH_ELEMENT3D_H_

#ifndef _MJMESH_EDGE2D_H_
#define _MJMESH_EDGE2D_H_

#include "MJMesh/Definitions.h"
#include "Data/Edge2D.h"
#include "Data/Box.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class Edge2D : public Data::Edge2D
#if USE_POOL
		, public Data::Poolable<MJMesh::Edge2D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::Edge2D)
#endif //#if USE_POOL

	public:

		Edge2D(MJMesh::Vertex *v1, MJMesh::Vertex *v2);
		virtual ~Edge2D();

#if USE_ATTRIBS
		virtual Real size() const;

#if USE_EXPERIMENTAL_3
		virtual const Data::Vector2D *getVector() const;
		virtual Data::Vector2D vector2D() const;
#else
		virtual const Data::Vector *getVector() const;
		virtual Data::Vector vector() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual const Data::Vector2D *getNormal() const;
		virtual Data::Vector2D normal() const;

#if USE_EXPERIMENTAL_3
		virtual const Data::Point2D *getMid() const;
		virtual Data::Point2D mid2D() const;
#else
		virtual const Data::Point *getMid() const;
		virtual Data::Point mid() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual const Data::Box *getBox() const;
		virtual Data::Box box() const;

		virtual void clearAttribs() const;
		virtual void recalcAttribs() const;
		virtual void invertNormal() const;
#endif //#if USE_ATTRIBS

#if USE_ARRAY
		using Data::Edge2D::operator=;
#if USE_ATTRIBS
		virtual MJMesh::Edge2D &operator=(const MJMesh::Edge2D &e);
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY

#if USE_ATTRIBS
	protected:

		mutable Real s;
#if USE_EXPERIMENTAL_3
		mutable Data::Vector2D *v;
#else
		mutable Data::Vector *v;
#endif //#if USE_EXPERIMENTAL_3
		mutable Data::Vector2D *n;
#if USE_EXPERIMENTAL_3
		mutable Data::Point2D *m;
#else
		mutable Data::Point *m;
#endif //#if USE_EXPERIMENTAL_3
		mutable Data::Box *bbox;
#endif //#if USE_ATTRIBS
	};
}

#endif //#ifndef _MJMESH_EDGE2D_H_

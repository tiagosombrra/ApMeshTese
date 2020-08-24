#ifndef _MJMESH_EDGE3D_H_
#define _MJMESH_EDGE3D_H_

#include "MJMesh/Definitions.h"
#include "Data/Edge3D.h"
#include "Data/Box.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class Edge3D : public Data::Edge3D
#if USE_POOL
		, public Data::Poolable<MJMesh::Edge3D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::Edge3D)
#endif //#if USE_POOL

	public:
		Edge3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2);
		virtual ~Edge3D();

		virtual UInt dimension() const;

#if USE_ATTRIBS
		virtual Real size() const;

#if USE_EXPERIMENTAL_3
		virtual const Data::Vector3D *getVector() const;
		virtual Data::Vector3D vector3D() const;
#else
		virtual const Data::Vector *getVector() const;
		virtual Data::Vector vector() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
		virtual const Data::Point3D *getMid() const;
		virtual Data::Point3D mid3D() const;
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
		using Data::Edge::operator=;
#if USE_ATTRIBS
		virtual MJMesh::Edge3D &operator=(const MJMesh::Edge3D &e);
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY

#if USE_ATTRIBS
	protected:

		mutable Real s;
#if USE_EXPERIMENTAL_3
		mutable Data::Vector3D *v;
#else
		mutable Data::Vector *v;
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
		mutable Data::Point3D *m;
#else
		mutable Data::Point *m;
#endif //#if USE_EXPERIMENTAL_3
		mutable Data::Box *bbox;
#endif //#if USE_ATTRIBS
	};
}

#endif //#ifndef _MJMESH_EDGE3D_H_

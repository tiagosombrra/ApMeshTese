#ifndef _MJMESH_TRIANGLE3D_H_
#define _MJMESH_TRIANGLE3D_H_

#include "MJMesh/Definitions.h"
#include "Data/Triangle3D.h"
#include "MJMesh/Vertex.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class Triangle3D : public Data::Triangle3D
#if USE_POOL
		, public Data::Poolable<MJMesh::Triangle3D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::Triangle3D)
#endif //#if USE_POOL

	public:

		Triangle3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3);
		virtual ~Triangle3D();

#if USE_ATTRIBS
		virtual Real size() const;

		virtual const Data::Vector3D *getNormal() const;
		virtual Data::Vector3D normal() const;

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
		using Data::Triangle3D::operator=;
#if USE_ATTRIBS
		virtual MJMesh::Triangle3D &operator=(const MJMesh::Triangle3D &t);
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY

#if USE_ATTRIBS
	protected:

		mutable Real s;
		mutable Data::Vector3D *n;
#if USE_EXPERIMENTAL_3
		mutable Data::Point3D *m;
#else
		mutable Data::Point *m;
#endif //#if USE_EXPERIMENTAL_3
		mutable Data::Box *bbox;
#endif //#if USE_ATTRIBS
	};
}

#endif //#ifndef _MJMESH_TRIANGLE3D_H_

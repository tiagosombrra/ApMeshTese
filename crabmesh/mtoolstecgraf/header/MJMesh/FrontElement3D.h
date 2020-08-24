#ifndef _MJMESH_FRONT_ELEMENT3D_H_
#define _MJMESH_FRONT_ELEMENT3D_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/FrontElement.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class FrontElement3D : public MJMesh::FrontElement
#if USE_POOL
		, public Data::Poolable<MJMesh::FrontElement3D>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::FrontElement3D)
#endif //#if USE_POOL

	public:

		//FrontElement3D(Data::Triangle3D *t);
		FrontElement3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3, ULInt id = 0);
#if USE_QUADS
		FrontElement3D(MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3,
			MJMesh::Vertex *v4, ULInt id = 0);
#endif //#if USE_QUADS
		virtual ~FrontElement3D();

		virtual MJMesh::Vertex *getV3() const;

#if USE_QUADS
		virtual MJMesh::Vertex *getV4() const;
#endif //#if USE_QUADS

		virtual UInt numSides() const;
		virtual UInt numVerticesInSide(UInt side) const;
		virtual Data::Vertex *getVertexInSide(UInt vertex, UInt side) const;

		virtual bool intercept(const Data::Element &e) const;
		virtual bool intercept(const Data::Element *e) const;
		virtual bool intercept(const MJMesh::Element3D &e) const;
		virtual bool intercept(const MJMesh::Element3D *e) const;

		virtual bool intercept(const Data::FrontElement *e) const;
		virtual bool intercept(const MJMesh::FrontElement3D *e) const;

		virtual bool intercept(const Data::Triangle &t) const;
		virtual bool intercept(const Data::Triangle *t) const;
		virtual bool intercept(const MJMesh::Triangle3D &t) const;
		virtual bool intercept(const MJMesh::Triangle3D *t) const;

		virtual bool intercept(const Data::Edge &e) const;
		virtual bool intercept(const Data::Edge *e) const;
		virtual bool intercept(const MJMesh::Edge3D &e) const;
		virtual bool intercept(const MJMesh::Edge3D *e) const;

		virtual bool intercept(const Data::Point &p) const;
		virtual bool intercept(const Data::Point *p) const;

#if USE_EXACT
		virtual bool eintercept(const Data::FrontElement *e) const;
		virtual bool eintercept(const MJMesh::FrontElement3D *e) const;

		virtual bool eintercept(const Data::Triangle &t) const;
		virtual bool eintercept(const Data::Triangle *t) const;
		virtual bool eintercept(const MJMesh::Triangle3D &t) const;
		virtual bool eintercept(const MJMesh::Triangle3D *t) const;
#endif //#if USE_EXACT

		using MJMesh::FrontElement::accordingToNormal;
		virtual bool accordingToNormal(Data::Point *p, bool insideTest = true) const;

		using Data::Element::have;
		virtual bool have(const Data::Triangle &t) const;
		virtual bool have(const Data::Triangle *t) const;
		virtual bool have(const MJMesh::Triangle3D &t) const;
		virtual bool have(const MJMesh::Triangle3D *t) const;

		virtual bool have(const Data::Edge &e) const;
		virtual bool have(const Data::Edge *e) const;
		//virtual bool have(const MJMesh::Edge3D &e) const;
		//virtual bool have(const MJMesh::Edge3D *e) const;

		virtual bool isAdjacent(const Data::Element &e) const;
		virtual bool isAdjacent(const Data::Element *e) const;
		virtual bool isAdjacent(const MJMesh::Element3D &e) const;
		virtual bool isAdjacent(const MJMesh::Element3D *e) const;

		using MJMesh::FrontElement::angle;
		virtual Real angle(const Data::Point *p) const;

		virtual Real distance(const Data::Point *p) const;
		virtual Real straightDistance(const Data::Point *p) const;

#if USE_ATTRIBS
		virtual void invert();
#endif //#if USE_ATTRIBS

		virtual Element *advance(Data::Vertex *v, const Data::Front *f,
			Data::FrontElementList &newFront,
			Data::FrontElementList &oldFront) const;
		virtual MJMesh::Element3D *advance(MJMesh::Vertex *v, const MJMesh::Front *f,
			Data::FrontElementList &newFront,
			Data::FrontElementList &oldFront) const;

		virtual bool match(const Data::VertexVector &vertices) const;
		virtual bool geometricallyMatch(const Data::VertexVector &vertices) const;

#if USE_ARRAY
		using MJMesh::FrontElement::operator=;
#if USE_EXPERIMENTAL
		virtual FrontElement3D &operator=(const MJMesh::FrontElement3D &e);
#endif //#if USE_EXPERIMENTAL
#endif //#if USE_ARRAY

#if USE_GUI
		using Data::FrontElement::drawNormal;
		virtual void drawNormal(Real factor) const;
#endif //#if USE_GUI

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

#endif //#ifndef _MJMESH_FRONT_ELEMENT3D_H_

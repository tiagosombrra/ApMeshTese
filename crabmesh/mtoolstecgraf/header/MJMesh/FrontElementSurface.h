#ifndef _MJMESH_FRONT_ELEMENT_SURFACE_H_
#define _MJMESH_FRONT_ELEMENT_SURFACE_H_

#include "MJMesh/Definitions.h"
#include "Data/Vector3D.h"
#include "MJMesh/FrontElement.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class FrontElementSurface : public MJMesh::FrontElement
#if USE_POOL
		, public Data::Poolable<MJMesh::FrontElementSurface>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::FrontElementSurface)
#endif //#if USE_POOL

	public:

		//FrontElementSurface(MJMesh::Edge3D *e);
		FrontElementSurface(MJMesh::Vertex *v1, MJMesh::Vertex *v2, ULInt id = 0);
		virtual ~FrontElementSurface();

		virtual UInt numSides() const;
		virtual UInt numVerticesInSide(UInt side) const;
		virtual Data::Vertex *getVertexInSide(UInt vertex, UInt side) const;

		virtual void setNormal(const Data::Vector3D &normal);
		virtual const Data::Vector3D &getNormal() const;

		virtual bool intercept(const Data::Element &e) const;
		virtual bool intercept(const Data::Element *e) const;
		virtual bool intercept(const MJMesh::ElementSurface &e) const;
		virtual bool intercept(const MJMesh::ElementSurface *e) const;

		virtual bool intercept(const Data::FrontElement *e) const;
		virtual bool intercept(const MJMesh::FrontElementSurface *e) const;

		virtual bool intercept(const Data::Edge &e) const;
		virtual bool intercept(const Data::Edge *e) const;
		virtual bool intercept(const MJMesh::Edge3D &e) const;
		virtual bool intercept(const MJMesh::Edge3D *e) const;

		//a point intercepts a front element if their distance is too small
		//UPDATE: a point intercepts a front element if their distance is too
		// small with respect to the size of the front element
		using MJMesh::FrontElement::intercept;
		virtual bool intercept(const Data::Point &p) const;
		virtual bool intercept(const Data::Point *p) const;

#if USE_EXACT
		virtual bool eintercept(const Data::FrontElement *e) const;
		virtual bool eintercept(const MJMesh::FrontElementSurface *e) const;

		virtual bool eintercept(const Data::Edge &e) const;
		virtual bool eintercept(const Data::Edge *e) const;
		virtual bool eintercept(const MJMesh::Edge3D &e) const;
		virtual bool eintercept(const MJMesh::Edge3D *e) const;
#endif //#if USE_EXACT

		using MJMesh::FrontElement::accordingToNormal;
		virtual bool accordingToNormal(Data::Point *p, bool insideTest = true) const;

		using Data::Element::have;
		virtual bool have(const Data::Edge &e) const;
		virtual bool have(const Data::Edge *e) const;
		virtual bool have(const MJMesh::Edge3D &e) const;
		virtual bool have(const MJMesh::Edge3D *e) const;

		virtual bool isAdjacent(const Data::Element &e) const;
		virtual bool isAdjacent(const Data::Element *e) const;
		virtual bool isAdjacent(const MJMesh::ElementSurface &e) const;
		virtual bool isAdjacent(const MJMesh::ElementSurface *e) const;

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
		virtual MJMesh::ElementSurface *advance(MJMesh::Vertex *v, const MJMesh::Front *f,
			Data::FrontElementList &newFront,
			Data::FrontElementList &oldFront) const;

		virtual bool match(const Data::VertexVector &vertices) const;
		virtual bool geometricallyMatch(const Data::VertexVector &vertices) const;

#if USE_ARRAY
		using MJMesh::FrontElement::operator=;
#if USE_EXPERIMENTAL
		virtual FrontElementSurface &operator=(const MJMesh::FrontElementSurface &e);
#endif //#if USE_EXPERIMENTAL
#endif //#if USE_ARRAY

#if USE_GUI
		using Data::FrontElement::drawNormal;
		virtual void drawNormal(Real factor) const;
#endif //#if USE_GUI

	protected:

#if USE_EXPERIMENTAL
		Data::Vertex *vertices[2];
#endif //#if USE_EXPERIMENTA

		Data::Vector3D normal;
	};
}

#endif //#ifndef _MJMESH_FRONT_ELEMENT_SURFACE_H_

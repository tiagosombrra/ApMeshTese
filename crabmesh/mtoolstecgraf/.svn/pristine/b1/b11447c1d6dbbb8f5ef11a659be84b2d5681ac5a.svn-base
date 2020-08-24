#ifndef _MJMESH_BOUNDARY_H_
#define _MJMESH_BOUNDARY_H_

#include "MJMesh/Definitions.h"
#include "Data/Boundary.h"

namespace MJMesh
{
	bool less(Data::FrontElement *e1, Data::FrontElement *e2);

	class Boundary : public Data::Boundary
	{
	public:

		Boundary(ULInt id = 0);
		Boundary(MJMesh::Boundary *boundary);
		virtual ~Boundary();

		virtual void setVertices(const Data::VertexList &vertices);
		virtual void setElements(const Data::FrontElementList &elements);

		virtual bool isSurfaceBoundary() const;

		using Data::Boundary::add;
		using Data::Boundary::have;

		virtual void add(Data::Vertex *v);
		virtual void remove(Data::Vertex *v);

		virtual void add(Data::FrontElement *e);
		virtual void remove(Data::FrontElement *e);

		virtual Data::FrontElementList adjacency(const Data::Vertex *v) const;

		using Data::Boundary::have;
		virtual bool have(const Data::Vertex *v, bool matchingCoords) const;
		virtual bool have(const Data::FrontElement *e, bool matchingCoords) const;

		virtual void clear();

		virtual Data::Boundary::VertexIterator erase(const Data::Boundary::VertexIterator &iter);
		virtual Data::Boundary::FrontElementIterator erase(const Data::Boundary::FrontElementIterator &iter);

		virtual void swap(MJMesh::Boundary *boundary);

		virtual void sort();

		//virtual void debug() const;

#if USE_GUI
		using Data::Boundary::draw;
		virtual void draw(bool lighting) const;
		virtual void draw() const;

		virtual void drawDiscretization() const;
#endif //#if USE_GUI

	protected:

		virtual void clearVertices();
		virtual void clearElements();

	};
}

#endif //#ifndef _MJMESH_BOUNDARY_H_

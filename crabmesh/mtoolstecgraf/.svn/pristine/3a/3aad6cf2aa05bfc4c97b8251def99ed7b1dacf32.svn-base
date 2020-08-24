#ifndef _DATA_BOUNDARY_H_
#define _DATA_BOUNDARY_H_

#include "Data/Definitions.h"

#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/AbstractMesh.h"

namespace Data
{
	class Boundary :
		public Data::AbstractMesh
	{
	public:

		typedef VertexList::iterator VertexIterator;
		typedef VertexList::const_iterator ConstVertexIterator;

		typedef FrontElementList::iterator FrontElementIterator;
		typedef FrontElementList::const_iterator ConstFrontElementIterator;

	public:

		Boundary(bool sorted = true, ULInt id = 0);
		virtual ~Boundary();

		virtual void setVertices(const VertexList &vertices);
		virtual const VertexList &getVertices() const;

		virtual void setElements(const FrontElementList &elements);
		virtual const FrontElementList &getElements() const;

		virtual void setSorted(bool sorted);
		virtual bool isSorted() const;

        virtual void sortBoundary();

		virtual UInt dimension() const;

		virtual void add(Vertex *v);
		virtual void add(FrontElement *e);

		virtual void remove(Vertex *v);
		virtual void remove(FrontElement *e);

		virtual Vertex *getVertex(ULInt id) const;
		virtual FrontElement *getElement(ULInt id) const;

		virtual FrontElementList adjacency(const Vertex *v) const;

		using Data::AbstractMesh::have;

		virtual bool have(const Vertex *v) const;
		virtual bool have(const Vertex *v, bool matchingCoords) const;

		virtual bool have(const FrontElement *e) const;
		virtual bool have(const FrontElement *e, bool matchingCoords) const;

		virtual bool have(const Element *e, const Mesh *mesh, Data::FrontElementList *fels = NULL) const;
		virtual bool have(const Element *e, Data::FrontElementList &fels) const;

		virtual bool haveAny(const VertexList &vertices, bool matchingCoords = false) const;
		virtual bool haveAny(const FrontElementList &elements, bool matchingCoords = false) const;

		virtual ULInt verticesSize() const;
		virtual ULInt size() const;

		virtual VertexIterator vBegin();
		virtual ConstVertexIterator vBegin() const;
		virtual VertexIterator vEnd();
		virtual ConstVertexIterator vEnd() const;

		virtual FrontElementIterator eBegin();
		virtual ConstFrontElementIterator eBegin() const;
		virtual FrontElementIterator eEnd();
		virtual ConstFrontElementIterator eEnd() const;

		virtual VertexIterator erase(const VertexIterator &iter);
		virtual FrontElementIterator erase(const FrontElementIterator &iter);

		virtual void clear();

		virtual Box *box() const;

		virtual void sort() = 0;

#if USE_GUI
		using Colorable::draw;
		virtual void draw() const;
		virtual void drawPick() const;
		virtual void drawNormals(Real factor) const;
		virtual void drawDiscretization() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		VertexList vertices;
		FrontElementList elements;

		bool sorted;
	};
}

#endif //#ifndef _DATA_BOUNDARY_H_

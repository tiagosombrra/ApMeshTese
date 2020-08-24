#ifndef _MJMESH_FRONT_H_
#define _MJMESH_FRONT_H_

#include "MJMesh/Definitions.h"
#include "Data/Front.h"

namespace MJMesh
{
	class Front : public Data::Front
	{
	public:

		Front(bool sorted = true, Data::GeometricSearchTree *vstree = NULL,
			Data::GeometricSearchTree *festree = NULL, ULInt id = 0);
		Front(const MJMesh::Boundary *boundary, Data::GeometricSearchTree *vstree = NULL,
			Data::GeometricSearchTree *festree = NULL, ULInt id = 0);
		virtual ~Front();

		virtual void set(const Data::Front *front);

		virtual void setSorted(bool sorted);

		virtual void sortVertexs();

		virtual Data::FrontElement *getElement(ULInt id) const;

#if USE_SET
		virtual Data::FrontElementList getBoundary() const;
#else
		virtual const Data::FrontElementList &getBoundary() const;
#endif //#if USE_SET

		virtual void setVertices(const Data::VertexList &vertices);
		virtual void setElements(const Data::FrontElementList &elements);

		virtual void setRejected(const Data::FrontElementList &rejected);
#if USE_SET
		virtual Data::FrontElementList getRejected() const;
#else
		virtual const Data::FrontElementList &getRejected() const;
#endif //#if USE_SET

		virtual Data::FrontElementList getAllElements() const;

		virtual void setVSTree(Data::GeometricSearchTree *vstree);
		virtual void setFESTree(Data::GeometricSearchTree *festree);

		virtual bool isUsingSearchTrees() const;
		virtual bool isUsingRejected() const;

		virtual void clear();
		virtual void clearSearchTrees();
		virtual void deleteSearchTrees(bool ignoreContents = false);

		using Data::Front::add;
		using Data::Front::remove;

		virtual void add(Data::Vertex *v);
		//virtual void add(MJMesh::Vertex *v);

		virtual bool remove(const Data::Vertex *v);
		//virtual bool remove(const MJMesh::Vertex *v);

		virtual void add(Data::FrontElement *e);
		//virtual void add(MJMesh::FrontElement *e);

		virtual bool remove(const Data::FrontElement *e);
		//virtual bool remove(const MJMesh::FrontElement *e);

		virtual void prepareForTopologyPhase(bool usingSearchTrees);

		virtual bool isEmpty() const;
		virtual ULInt size() const;
		virtual ULInt size(UShort state) const;

		virtual Data::FrontElement *next() const;

		//returns true if the given front element was rejected for the first
		// time. returns false if it was rejected for the second time
		virtual bool reject(Data::FrontElement *e, UInt code = 0);
		//virtual bool reject(MJMesh::FrontElement *e, UInt code = 0);

		using Data::Front::shouldRemove;
		virtual bool shouldRemove(const Data::Vertex *v) const;
		//virtual bool shouldRemove(const MJMesh::Vertex *v) const;

		//virtual bool intercept(const Data::FrontElement *e) const;
		//virtual bool intercept(const MJMesh::FrontElement *e) const;
		//virtual bool intercept(const Data::FrontElement *e, const Data::Point *p) const;
		//tests if the given point intercepts any of the elements adjacent to
		// the front vertices or elements
		virtual bool interceptElements(const Data::Point *p) const;
		virtual bool interceptElements(const Data::Point *p, const Data::ElementSet &elements) const;
		virtual bool interceptElements(const Data::Point *p, const Data::FrontElementList &frontElements) const;

		using Data::Front::verticesInRange;
		virtual Data::VertexList verticesInRange(const Data::Box &box) const;
		virtual Data::FrontElementList frontElementsInRange(const Data::Box &box) const;

		using Data::Front::have;
		virtual bool have(const Data::Vertex *v) const;
		virtual bool have(const Data::FrontElement *fel) const;
		virtual bool have(const Data::FrontElement *fel, UShort state) const;

		virtual UShort getState(const Data::FrontElement *fel) const;

		using Data::Front::haveAny;
		virtual bool haveAny(const Data::FrontElementList &elements) const;

		using Data::Front::find;
		virtual Data::FrontElement *find(const Data::VertexVector &vertices, const Data::VertexList &others) const;

		virtual Data::Front::FrontElementIterator eBegin(UShort state);
		virtual Data::Front::ConstFrontElementIterator eBegin(UShort state) const;
		virtual Data::Front::FrontElementIterator eEnd(UShort state);
		virtual Data::Front::ConstFrontElementIterator eEnd(UShort state) const;

		using Data::Front::erase;
		virtual Data::Front::VertexIterator erase(Data::Front::VertexIterator &iter);
		virtual Data::Front::FrontElementIterator erase(Data::Front::FrontElementIterator &iter, UShort state);

#if USE_GUI
		using Data::Front::draw;
		virtual void draw() const;
/*#if USE_SET
		virtual void draw(const Data::FrontElementMultiSet &elements, Real r, Real g, Real b, GLboolean lighting) const;
#else
		virtual void draw(const Data::FrontElementList &elements, Real r, Real g, Real b, GLboolean lighting) const;
#endif //#if USE_SET*/
		virtual void drawPick() const;
		virtual void drawNormals(Real factor) const;
		virtual void drawDiscretization() const;
#endif //#if USE_GUI

#if USE_SET
	protected:

#if USE_SET
		//virtual void setVertices(const Data::VertexSet &vertices);

		virtual void setBoundary(const Data::FrontElementMultiSet &boundary);
		virtual const Data::FrontElementMultiSet &getBoundarySet() const;

		virtual void setElements(const Data::FrontElementMultiSet &elements);

		virtual void setRejected(const Data::FrontElementMultiSet &rejected);
		virtual const Data::FrontElementMultiSet &getRejectedSet() const;

		virtual UShort numStates() const;

		//virtual Data::FrontElementMultiSet *makeSet() const;
#endif //#if USE_SET

	/*public:

		virtual bool operator()(const Data::FrontElement *e1, const Data::FrontElement *e2) const;*/

		//virtual bool better(const Data::FrontElement *e1, const Data::FrontElement *e2) const;
#endif //#if USE_SET

	protected:

		virtual bool remove(const Data::FrontElement *e, bool rejecting);

#if USE_SET
		virtual void addSpecificData(Data::FrontElementMultiSet *list, UShort state) const;
#else
		virtual void addSpecificData(Data::FrontElementList &list, UShort state) const;
#endif //#if USE_SET

#if USE_SET
		virtual void removeSpecificData(Data::FrontElementMultiSet *list, bool ignoreContents) const;
#else
		virtual void removeSpecificData(Data::FrontElementList &list, bool ignoreContents) const;
#endif //#if USE_SET

	protected:

		virtual void addVerticesSpecificData(Data::VertexList &vertices) const;
		virtual void removeVerticesSpecificData(Data::VertexList &vertices, bool ignoreContents) const;
/*#if USE_SET
		virtual void removeVerticesSpecificData(Data::VertexSet &vertices, bool ignoreContents) const;
#else
		virtual void removeVerticesSpecificData(Data::VertexList &vertices, bool ignoreContents) const;
#endif //#if USE_SET*/

	protected:
		virtual void addSpecificData();
		virtual void removeSpecificData(bool ignoreContents = false);

#if USE_SET
		virtual void removeSearchTreeSpecificData(Data::FrontElementMultiSet *list) const;
#else
		virtual void removeSearchTreeSpecificData(Data::FrontElementList &list) const;
#endif //#if USE_SET

		virtual void removeSearchTreeSpecificData() const;

	protected:

#if USE_SET
		Data::FrontElementMultiSet *boundary;
		Data::FrontElementMultiSet *rejected;
#else
		Data::FrontElementList boundary;
		Data::FrontElementList rejected;
#endif //#if USE_SET

		enum Step
		{
			USE_FRONT,
			USE_REJECTED
		};

		mutable enum Step step;

		Data::GeometricSearchTree *vstree;
		Data::GeometricSearchTree *festree;
	};
}

#endif //#ifndef _MJMESH_FRONT_H_

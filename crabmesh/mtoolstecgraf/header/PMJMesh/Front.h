#ifndef _PMJMESH_FRONT_H_
#define _PMJMESH_FRONT_H_

#include "PMJMesh/Definitions.h"
#include "MJMesh/Front.h"

namespace PMJMesh
{
	class Front : public MJMesh::Front
	{
	public:

		Front(bool sorted = true, Data::GeometricSearchTree *vstree = NULL,
			Data::GeometricSearchTree *festree = NULL, ULInt id = 0);
		Front(const MJMesh::Boundary *boundary, Data::GeometricSearchTree *vstree = NULL,
			Data::GeometricSearchTree *festree = NULL, ULInt id = 0);
		virtual ~Front();

#if USE_SET
		virtual void initialize();
#endif //#if USE_SET

		virtual void set(const Data::Front *front);

		virtual Data::GeometricSearchTree *getVSTree() const;
		virtual Data::GeometricSearchTree *getFESTree() const;

		virtual void setBoundary(const Data::FrontElementList &boundary);

		virtual void setCrossing(const Data::FrontElementList &crossing);
		virtual Data::FrontElementList getCrossing() const;

		virtual void addElement(Data::FrontElement *e);

		virtual void addBoundary(Data::FrontElement *e);

		virtual void addRejected(Data::FrontElement *e);

		virtual void addCrossing(Data::FrontElement *e);

		virtual void clear();

/*#if USE_UNIQUE_SEARCH_TREE
		virtual void clearSearchTrees();
#endif //#if USE_UNIQUE_SEARCH_TREE*/

		//virtual UInt size() const;

		using MJMesh::Front::size;
		virtual ULInt size(UShort state) const;

		using MJMesh::Front::remove;

#if USE_UNIQUE_SEARCH_TREE
		virtual void prepareForTopologyPhase(bool usingSearchTrees);
#endif //#if USE_UNIQUE_SEARCH_TREE

		virtual bool reject(Data::FrontElement *e, UInt code);

		virtual Data::FrontElement *getElement(ULInt id) const;

		using MJMesh::Front::intercept;
		virtual bool intercept(const Data::FrontElement *e) const;
		virtual bool intercept(const Data::FrontElement *e, const Data::Point *p) const;

		//using MJMesh::Front::find;
		//virtual Data::FrontElement *find(const Data::VertexVector &vertices, const Data::VertexList &others) const;

		virtual void moveBoundaryToElements();
		virtual void moveRejectedToElements();
		virtual void moveRejectedToCrossing();
		virtual void moveCrossingToRejected();

		virtual Front *setIntersection(PMJMesh::Front *front);
		virtual void setMerge(PMJMesh::Front *front);

		///Obs.: No external access to crossing is granted through methods
		///  getAllElements(), allEBegin() and allEEnd(). To access these
		///  front elements, one must call method moveCrossingToRejected()
		///   before. However, one can access them through getCrossing(),
		///   eBegin(3) and eEnd(3).

		virtual Data::Front::FrontElementIterator eBegin(UShort state);
		virtual Data::Front::ConstFrontElementIterator eBegin(UShort state) const;
		virtual Data::Front::FrontElementIterator eEnd(UShort state);
		virtual Data::Front::ConstFrontElementIterator eEnd(UShort state) const;

		using MJMesh::Front::erase;
		virtual Data::Front::FrontElementIterator erase(Data::Front::FrontElementIterator &iter, UShort state);

		virtual Data::Front::VertexIterator replace(Data::Front::VertexIterator &iter, Data::Vertex *v);
		virtual Data::Front::FrontElementIterator replace(Data::Front::FrontElementIterator &iter, UShort state, Data::FrontElement *e);

#if USE_GUI
		using MJMesh::Front::draw;
		virtual void draw() const;
		virtual void drawPick() const;
		virtual void drawNormals(Real factor) const;
#endif //#if USE_GUI

	protected:

		using MJMesh::Front::setBoundary;

		virtual void setCrossing(const Data::FrontElementMultiSet &crossing);
		virtual const Data::FrontElementMultiSet &getCrossingSet() const;

		//virtual UShort numStates() const;

		virtual bool remove(const Data::FrontElement *e, bool rejecting);

		using MJMesh::Front::addSpecificData;
		virtual void addSpecificData();
		using MJMesh::Front::removeSpecificData;
		virtual void removeSpecificData(bool ignoreContents = false);
		using MJMesh::Front::removeSearchTreeSpecificData;
		virtual void removeSearchTreeSpecificData() const;

	protected:

		Data::FrontElementMultiSet *crossing;
	};
}

#endif //#ifndef _PMJMESH_FRONT_H_

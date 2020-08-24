#ifndef _DATA_FRONT_H_
#define _DATA_FRONT_H_

#include "Data/Definitions.h"
#include "Data/Vertex.h"
#include "Data/AbstractMesh.h"

namespace Data
{
#if USE_SET
	class AbstractFront :
		public Data::AbstractMesh
	{
	public:
		//AbstractFront(const AbstractFront &front);
		AbstractFront(bool sorted, ULInt id);
		//virtual ~AbstractFront();

		//inserts always in the end (fifo, list-like)
		virtual bool operator()(const FrontElement *e1, const FrontElement *e2) const;

#if USE_GUI
		using Data::AbstractMesh::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual UShort numStates() const;

	protected:

		bool sorted;
	};
#endif //#if USE_SET

	class Front :
#if USE_SET
		public Data::AbstractFront
#else
		public Data::AbstractMesh
#endif //#if USE_SET
	{
	public:

		typedef VertexList::iterator VertexIterator;
		typedef VertexList::const_iterator ConstVertexIterator;

#if USE_SET
		//typedef VertexSet::iterator VertexIterator;
		//typedef VertexSet::const_iterator ConstVertexIterator;

		typedef FrontElementMultiSet::iterator FrontElementIterator;
		typedef FrontElementMultiSet::const_iterator ConstFrontElementIterator;
#else
		//typedef VertexList::iterator VertexIterator;
		//typedef VertexList::const_iterator ConstVertexIterator;

		typedef FrontElementList::iterator FrontElementIterator;
		typedef FrontElementList::const_iterator ConstFrontElementIterator;
#endif //#if USE_SET

#if USE_SET
		class AllFrontElementIterator
		{
		friend
			class Data::Front;

		public:
			AllFrontElementIterator(Front *front);
			AllFrontElementIterator(Front *front, UShort state, FrontElementIterator iter);

			AllFrontElementIterator &operator++();
			AllFrontElementIterator operator++(int);

			AllFrontElementIterator &operator--();
			AllFrontElementIterator operator--(int);

			AllFrontElementIterator &operator=(const AllFrontElementIterator &iter);

			bool operator==(const AllFrontElementIterator &iter);
			bool operator!=(const AllFrontElementIterator &iter);

			FrontElement *operator*() const;

			UShort getState() const;

		protected:

			void ensureValidIncrement();
			void ensureValidDecrement();

		protected:

			Front *front;
			UShort state;
			FrontElementIterator iter;
		};

		class ConstAllFrontElementIterator
		{
		friend
			class Data::AbstractFront;

		public:
			ConstAllFrontElementIterator(const Front *front);
			ConstAllFrontElementIterator(const Front *front, UShort state, ConstFrontElementIterator iter);

			ConstAllFrontElementIterator &operator++();
			ConstAllFrontElementIterator operator++(int);

			ConstAllFrontElementIterator &operator--();
			ConstAllFrontElementIterator operator--(int);

			ConstAllFrontElementIterator &operator=(const ConstAllFrontElementIterator &iter);

			bool operator==(const ConstAllFrontElementIterator &iter);
			bool operator!=(const ConstAllFrontElementIterator &iter);

			FrontElement *operator*() const;

		protected:

			void ensureValidIncrement();
			void ensureValidDecrement();

		protected:

			const Front *front;
			UShort state;
			ConstFrontElementIterator iter;
		};

#endif //#if USE_SET

	public:

		Front(bool sorted = true, ULInt id = 0);
		Front(const Boundary *boundary, ULInt id = 0);
		virtual ~Front();

		virtual void set(const Data::Front *front);

		virtual void setVertices(const VertexList &vertices);
		virtual const VertexList &getVertices() const;
/*#if USE_SET
		virtual VertexList getVertices() const;
#else
		virtual const VertexList &getVertices() const;
#endif //#if USE_SET*/

		virtual Vertex *getVertex(ULInt id) const;

		virtual void setElements(const FrontElementList &elements);
#if USE_SET
		virtual FrontElementList getElements() const;
#else
		virtual const FrontElementList &getElements() const;
#endif //#if USE_SET

		virtual FrontElementList getAllElements() const;

		virtual FrontElement *getElement(ULInt id) const;

		virtual void setSorted(bool sorted);
		virtual bool isSorted() const;

		virtual UInt dimension() const;

		virtual void clear();

		virtual void add(Vertex *v);
		virtual void add(FrontElement *e);

		virtual bool remove(const Vertex *v);
		virtual bool remove(const FrontElement *e);

		virtual bool isEmpty() const;
		virtual ULInt size() const;
		virtual ULInt size(UShort state) const;
		virtual ULInt verticesSize() const;

		virtual FrontElement *next() const;

		//default behaviour should return true, meaning that the advancing
		// front procedure can continue. otherwise, return false, meaning that
		// the advancing front procedure should stop as soon as possible. the
		// given code can be used if there are many rejection types involving
		// the same front
		virtual bool reject(FrontElement *e, UInt code = 0);

		virtual bool shouldRemove(const Vertex *v) const;
		virtual bool shouldRemove(const FrontElement *e) const;

		virtual Element *advance(FrontElement *e, Vertex *v, FrontElementList &newFront);
		virtual Element *advance(FrontElement *e, Vertex *v, FrontElementList &newFront, FrontElementList &oldFront);

		//tests if the given element intercepts any of the vertices belonging
		// to the front
		virtual bool intercept(const Element *e) const;
		virtual bool intercept(const Element *e, const VertexList &list) const;
		//tests if the given element intercepts any of the front elements
		// belonging to the front
		virtual bool intercept(const FrontElement *e) const;
		virtual bool intercept(const FrontElement *e, const FrontElementList &list) const;
		//tests if the given point, generated for the given front element,
		// intercepts any of the front elements belonging to the front (except
		// the given element)
		virtual bool intercept(const FrontElement *e, const Point *p) const;
		virtual bool intercept(const FrontElement *e, const Point *p, const FrontElementList &list) const;

		virtual VertexList verticesInRange(const PointList &p, Real radius, bool useBox = true) const;
		virtual VertexList verticesInRange(const PointList &p, Real radius, VertexList &inbox) const;
		virtual VertexList verticesInRange(const Box &box) const;
		virtual FrontElementList frontElementsInRange(const Box &box) const;

		using Data::AbstractFront::have;
		virtual bool have(const Vertex *v) const;
		virtual bool have(const FrontElement *e) const;

		virtual bool haveAny(const FrontElementList &elements) const;

		virtual FrontElement *find(const VertexVector &vertices, const Vertex *other) const;
		virtual FrontElement *find(const VertexVector &vertices, const VertexList &others) const;

		virtual VertexIterator vBegin();
		virtual ConstVertexIterator vBegin() const;
		virtual VertexIterator vEnd();
		virtual ConstVertexIterator vEnd() const;

		virtual FrontElementIterator eBegin(UShort state);
		virtual ConstFrontElementIterator eBegin(UShort state) const;
		virtual FrontElementIterator eEnd(UShort state);
		virtual ConstFrontElementIterator eEnd(UShort state) const;

		virtual AllFrontElementIterator allEBegin();
		virtual ConstAllFrontElementIterator allEBegin() const;
		virtual AllFrontElementIterator allEEnd();
		virtual ConstAllFrontElementIterator allEEnd() const;

		virtual VertexIterator erase(VertexIterator &iter);
		virtual FrontElementIterator erase(FrontElementIterator &iter, UShort state);
		virtual AllFrontElementIterator erase(AllFrontElementIterator &iter);

#if USE_GUI
		using Colorable::draw;
		virtual void draw() const;
		virtual void drawPick() const;
		virtual void drawNormals(Real factor) const;
		virtual void drawDiscretization() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

#if USE_SET
		//virtual void setVertices(const VertexSet &vertices);
		//virtual const VertexSet &getVerticesSet() const;

		virtual void setElements(const FrontElementMultiSet &elements);
		virtual const FrontElementMultiSet &getElementsSet() const;

		virtual UShort numStates() const;

		virtual FrontElementMultiSet *makeSet() const;
#endif //#if USE_SET

        virtual bool accordingToNormal(const Data::FrontElement *fel, const VertexList &others) const;

		//tests if any front element in the given list has the given vertex v
#if USE_SET
		virtual bool have(const FrontElementMultiSet *list, const Vertex *v) const;
#else
		virtual bool have(const FrontElementList &list, const Vertex *v) const;
#endif //#if USE_SET

#if USE_SET
		virtual bool have(const FrontElementMultiSet *list, const FrontElement *e) const;
#else
		virtual bool have(const FrontElementList &list, const FrontElementList *e) const;
#endif //#if USE_SET

#if USE_SET
		virtual bool haveAny(const FrontElementMultiSet *list, const FrontElementList &elements) const;
#else
		virtual bool haveAny(const FrontElementList &list, const FrontElementList &elements) const;
#endif //#if USE_SET

		//searches for the element that is equal to the given vertices in the
		// given list. if no element is found, return NULL
#if USE_SET
		virtual FrontElement *find(const FrontElementMultiSet *list, const VertexVector &vertices, const VertexList &others) const;
#else
		virtual FrontElement *find(const FrontElementList &list, const VertexVector &vertices, const VertexList &others) const;
#endif //#if USE_SET

#if USE_SET
		virtual FrontElement *find(const FrontElementMultiSet *list, ULInt id) const;
#else
		virtual FrontElement *find(const FrontElementList &list, ULInt id) const;
#endif //#if USE_SET

/*		//tests if the given element intercepts any element in the list
		static bool intercept(const FrontElementList &list, const FrontElement *e);*/

/*		//test if the given newly inserted point p, inserted for element e,
		// intercpts any element in the given list
		static bool intercept(const FrontElementList &list, const FrontElement *e, const Point *p);*/

/*#if USE_SET
	public:

		virtual bool operator()(const FrontElement *e1, const FrontElement *e2) const;
#endif //#if USE_SET*/

	protected:

		VertexList vertices;

/*#if USE_SET ///TODO optimize it (go back to list)
		VertexSet vertices;
#else
		VertexList vertices;
#endif //#if USE_SET*/

#if USE_SET
		FrontElementMultiSet *elements;
#else
		FrontElementList elements;
#endif //#if USE_SET

#if (!USE_SET)
		bool sorted;
#endif //#if (!USE_SET)
	};
}

#endif //#ifndef _DATA_FRONT_H_

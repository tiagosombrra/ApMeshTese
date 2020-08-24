#ifndef _DATA_ALTERNATING_DIGITAL_TREE_CELL_H_
#define _DATA_ALTERNATING_DIGITAL_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/GeometricSearchTreeNode.h"
#include "Data/Box.h"

namespace Data
{
	class AlternatingDigitalTreeCell : public Data::GeometricSearchTreeNode
	{
	public:

		//AlternatingDigitalTreeCell(AlternatingDigitalTree *tree);
		AlternatingDigitalTreeCell(AlternatingDigitalTreeCell *parent = NULL);
		virtual ~AlternatingDigitalTreeCell();

#if USE_EXPERIMENTAL_5
		virtual UInt numAdjacents() const;

		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;

		virtual GraphNodeVector getAdjacents() const;

		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);
#endif //#if USE_EXPERIMENTAL_5

		//virtual const Box &getBox() const;

		//virtual bool isActive() const;

		virtual AlternatingDigitalTreeCell *leftChild() const;
		virtual AlternatingDigitalTreeCell *rightChild() const;

		virtual void build(const IdentifiableList &shapes) = 0;

		using Data::GeometricSearchTreeNode::add;
		using Data::GeometricSearchTreeNode::remove;

		virtual Identifiable *getShape() const;

		virtual GeometricSearchTreeNode::ShapeSet allShapes() const;
		virtual void allShapes(GeometricSearchTreeNode::ShapeSet &shapes) const;
		virtual void allShapes(IdentifiableList &shapes) const;

		virtual GeometricSearchTreeNode::ShapeSet inRange(const Box *box) const;

#if USE_ARRAY
		using GeometricSearchTreeNode::operator=;
		virtual AlternatingDigitalTreeCell &operator=(const AlternatingDigitalTreeCell &c);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		virtual AlternatingDigitalTreeCell *makeChild() = 0;
		virtual void set(const Identifiable *shape, const Point *pt, const Box *box) = 0;
		virtual void set(const AlternatingDigitalTreeCell *other) = 0;

		virtual void setShape(const Identifiable *shape);
		virtual Identifiable *getAndRemoveShape();

		virtual void subdivide();

		virtual void makeLeftChild();
		virtual void makeRightChild();

		virtual bool inGlobal(const Data::Point *pt) const;
		virtual bool in(const Data::Box *box) const;
		virtual bool inGlobal(const Data::Box *box) const;

		virtual bool add(const Identifiable *shape, const Point *pt, const Box *box = NULL);
		virtual bool remove(const Identifiable *shape, const Point *pt, const Box *box = NULL);
		virtual bool removeCheap(const Identifiable *shape, const Point *pt);
		virtual bool removeFull(const Identifiable *shape, const Box *box);
		virtual bool removeFull(const Identifiable *shape, const Point *pt);

		virtual void build(UInt numShapes, Identifiable **shapes, Point **pts, Box **boxes = NULL);

		virtual void inRange(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const = 0;

		virtual void inRangeBox(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const;
		virtual void inRangeMid(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const;

		virtual void adjustGlobal();
		virtual void adjustGlobalWithBoxOrMid();

		virtual UInt median(UInt comp, UInt numPts, Point **pts) const;

		virtual void swap(Int i, Int j,
			Identifiable **shapes, Point **pts, Box **boxes) const;

		virtual Int split(UInt comp, UInt numShapes, Identifiable **shapes, Point **pts, Box **boxes = NULL) const;

	protected:

#if USE_EXPERIMENTAL_5
        GraphNode *adjacents[3];
#endif //#if USE_EXPERIMENTAL_5

#if USE_EXPERIMENTAL_3
		Point *mid;
#else
		Point mid;
#endif //#if USE_EXPERIMENTAL_3
		Box box;
		Box global;

		bool active;

	};
}

#endif //#ifndef _DATA_ALTERNATING_DIGITAL_TREE_CELL_H_

#ifndef _DATA_BOX_TREE_H_
#define _DATA_BOX_TREE_H_

#include "Data/Definitions.h"
#include "Data/RecursiveDecompositionTree.h"

namespace Data
{
	class BoxTree :
		public Data::RecursiveDecompositionTree
	{
	public:

		BoxTree(ULInt id = 0);
		BoxTree(BoxTreeCell *root, ULInt id = 0);
		BoxTree(Box *box, BoxTreeCell *root = NULL, ULInt id = 0);
		BoxTree(Point *min, Point *max, BoxTreeCell *root = NULL, ULInt id = 0);
		virtual ~BoxTree();

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool optIn(const Point &p) const;
		virtual bool optIn(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool optOn(const Point &p) const;
		virtual bool optOn(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual bool optOut(const Point &p) const;
		virtual bool optOut(const Point *p) const;

		virtual BoxTreeCell *find(const Point &p) const;
		virtual BoxTreeCell *find(const Point *p) const;

		virtual Real size() const;
		virtual Real size(UInt level) const;

		//for side neighbors
		virtual void neighbors(const BoxTreeCell *c, GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(const BoxTreeCell *c) const;
		virtual void neighbors(const ULInt id, GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(const ULInt id) const;
		virtual void neighbors(const BoxTreeCell *c, UInt side, GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(const BoxTreeCell *c, UInt side) const;
		virtual void neighbors(const ULInt id, UInt side, GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(const ULInt id, UInt side) const;
		//for corner neighbors
		virtual void neighbors(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(const BoxTreeCell *c, const UIntVector &sides) const;
		virtual void neighbors(const ULInt id, const UIntVector &sides, GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(const ULInt id, const UIntVector &sides) const;
		//return an adjacent cell with level less or equal than c->level, i.e.,
		//  this method does not recurse until leaves
		virtual BoxTreeCell *neighbor(const BoxTreeCell *c, UInt side) const;
		virtual BoxTreeCell *neighbor3D2(const BoxTreeCell *c, const UIntVector &sides) const;

		virtual void children(const BoxTreeCell *c, const UIntVector &children, GraphNodeList &cells) const;
		virtual GraphNodeList children(const BoxTreeCell *c, const UIntVector &children) const;
		virtual void children(const BoxTreeCell *c, UInt side, GraphNodeList &cells) const;
		virtual GraphNodeList children(const BoxTreeCell *c, UInt side) const;

#if USE_GUI
		using RecursiveDecompositionTree::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		//return an adjacent cell with level less or equal than c->level, i.e.,
		//  this method does not recurse until leaves
		virtual BoxTreeCell *neighbor(const BoxTreeCell *c, UInt side, UIntList &stack) const;
		virtual BoxTreeCell *neighbor3D2(const BoxTreeCell *c, const UIntVector &sides, UIntList &stack) const;

		virtual bool buildTableNeighbor3D2(const UIntVector &sides, UIntVector &callRec, UIntVector &callSib, UIntVector &callNeigh, UIntVector &children) const;
		virtual bool buildTableNeighbor3D3(const UIntVector &sides, UInt &callRec, UIntVector &callNeigh, UIntVector &callNeigh2, UInt &child) const;

		virtual void neighbors2D(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const;
		virtual GraphNodeList neighbors2D(const BoxTreeCell *c, const UIntVector &sides) const;
		virtual void neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const;
		virtual GraphNodeList neighbors3D2(const BoxTreeCell *c, const UIntVector &sides) const;
		virtual void neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, UIntList &stack, GraphNodeList &cells) const;
		virtual GraphNodeList neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, UIntList &stack) const;
		virtual void neighbors3D3(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const;
		virtual GraphNodeList neighbors3D3(const BoxTreeCell *c, const UIntVector &sides) const;

		virtual void neighbors(const BoxTreeCell *c, UInt side, UIntList &stack, GraphNodeList &cells) const;
		virtual GraphNodeList neighbors(const BoxTreeCell *c, UInt side, UIntList &stack) const;

		virtual BoxTreeCell *child(const BoxTreeCell *c, UIntList &stack) const;
		virtual BoxTreeCell *child(const BoxTreeCell *c, UInt child) const;

		virtual void children(const BoxTreeCell *c, const UIntVector &children, UIntList &stack, GraphNodeList &cells) const;
		virtual GraphNodeList children(const BoxTreeCell *c, const UIntVector &children, UIntList &stack) const;
	};
}

#endif //#ifndef _DATA_BOX_TREE_H_

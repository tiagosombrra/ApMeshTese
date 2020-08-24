#ifndef _DATA_BSP_TREE_H_
#define _DATA_BSP_TREE_H_

#include "Data/Definitions.h"
#include "Data/BoxTree.h"

namespace Data
{
	class BSPTree : public Data::BoxTree
	{
	public:

		BSPTree(ULInt id = 0);
		BSPTree(BSPTreeCell *root, ULInt id = 0);
		BSPTree(Box *b, BSPTreeCell *root = NULL, ULInt id = 0);
		BSPTree(Point *min, Point *max, BSPTreeCell *root = NULL, ULInt id = 0);
		virtual ~BSPTree();

		virtual GraphNode *node(ULInt id) const;

		using Data::BoxTree::size;
		virtual Real size(UInt level) const; //undefined

		virtual Data::BSPTreeCell *commonParent(ULInt id1, ULInt id2) const;
		virtual Data::BSPTreeCell *commonParent(ULInt id1, ULInt id2, Data::BSPTreeCell *base) const;

		virtual void neighbors(const BoxTreeCell *c, UInt side, GraphNodeList &cells) const;

	protected:

		using Data::BoxTree::neighbors;
		using Data::BoxTree::neighbors2D;
		using Data::BoxTree::neighbors3D2;
		using Data::BoxTree::neighbors3D3;
		using Data::BoxTree::children;

		virtual void children(const BoxTreeCell *c, UInt side, GraphNodeList &cells, const Data::Point *min, const Point *max, bool test = true) const;

		virtual void neighbors(const BoxTreeCell *c, UInt side, GraphNodeList &cells, const Point *min, const Point *max) const;

		///TODO
		virtual void neighbors2D(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const;
		virtual void neighbors2D(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells, const Data::Point2D &pt) const;
		virtual void neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const;
		virtual void neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells, const Data::Point *min, const Data::Point *max, bool test = true) const;
		virtual void neighbors3D3(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const;
		virtual void neighbors3D3(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells, const Data::Point3D &pt) const;
	};
}

#endif //#ifndef _DATA_BSP_TREE_H_

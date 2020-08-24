#ifndef _DATA_BIN_TREE_H_
#define _DATA_BIN_TREE_H_

#include "Data/Definitions.h"
#include "Data/BoxTree.h"

namespace Data
{
	class BinTree : public Data::BoxTree
	{
	public:

		BinTree(const Point &min, const Point &max, ULInt id = 0);
		BinTree(Real min, Real max, ULInt id = 0);
		virtual ~BinTree();

		virtual GraphNode *node(ULInt id) const;

		using BoxTree::in;
		virtual bool in(Real x) const;

		using BoxTree::optIn;
		virtual bool optIn(Real x) const;

		using BoxTree::on;
		virtual bool on(Real x) const;

		using BoxTree::optOn;
		virtual bool optOn(Real x) const;

		using BoxTree::out;
		virtual bool out(Real x) const;

		using BoxTree::optOut;
		virtual bool optOut(Real x) const;

		/*virtual GraphNodeVector neighbors(const BoxTreeCell *c) const;
		virtual GraphNodeVector neighbors(const BoxTreeCell *c, UInt side) const = 0;*/
	};
}

#endif //#ifndef _DATA_BIN_TREE_H_

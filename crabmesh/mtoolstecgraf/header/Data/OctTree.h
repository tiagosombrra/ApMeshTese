#ifndef _DATA_OCT_TREE_H_
#define _DATA_OCT_TREE_H_

#include "Data/Definitions.h"
#include "Data/Point3D.h"
#include "Data/BoxTree.h"
#include "Data/OctTreeCell.h"

namespace Data
{
	class OctTree : public Data::BoxTree
	{
	public:

		OctTree(const Point3D &min, const Point3D &max, ULInt id = 0);
		OctTree(Box *box, OctTreeCell *root, ULInt id = 0);
		virtual ~OctTree();

		virtual GraphNode *node(ULInt id) const;

		using BoxTree::in;
		virtual bool in(Real x, Real y, Real z) const;

		using BoxTree::optIn;
		virtual bool optIn(Real x, Real y, Real z) const;

		using BoxTree::on;
		virtual bool on(Real x, Real y, Real z) const;

		using BoxTree::optOn;
		virtual bool optOn(Real x, Real y, Real z) const;

		using BoxTree::out;
		virtual bool out(Real x, Real y, Real z) const;

		using BoxTree::optOut;
		virtual bool optOut(Real x, Real y, Real z) const;

		/*virtual GraphNodeVector neighbors(const BoxTreeCell *c) const;
		virtual GraphNodeVector neighbors(const BoxTreeCell *c, UInt side) const = 0;*/
	};
}

#endif //#ifndef _DATA_OCT_TREE_H_

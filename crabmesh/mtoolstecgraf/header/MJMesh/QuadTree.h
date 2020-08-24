#ifndef _MJMESH_QUAD_TREE_H_
#define _MJMESH_QUAD_TREE_H_

#include "MJMesh/Definitions.h"
#include "Data/QuadTree.h"

namespace MJMesh
{
	class QuadTree : public Data::QuadTree
	{
	public:

		QuadTree(const Data::Point2D &min, const Data::Point2D &max, ULInt id = 0);
		QuadTree(Data::Box *box, MJMesh::QuadTreeCell *root, ULInt id = 0);
		virtual ~QuadTree();

		/*virtual Data::GraphNodeVector neighbors(const Data::BoxTreeCell *c, UInt side) const;
		virtual Data::GraphNodeVector neighbors(const Data::BoxTreeCell *c) const;*/

		using Data::QuadTree::find;
		virtual MJMesh::QuadTreeCell *find(const MJMesh::FrontElement2D *e) const;
		virtual Data::BoxTreeCell *find(const MJMesh::FrontElement *e) const;
	};
}

#endif //#ifndef _MJMESH_QUAD_TREE_H_

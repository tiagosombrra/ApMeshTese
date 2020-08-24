#ifndef _MJMESH_OCT_TREE_H_
#define _MJMESH_OCT_TREE_H_

#include "MJMesh/Definitions.h"
#include "Data/OctTree.h"

namespace MJMesh
{
	class OctTree : public Data::OctTree
	{
	public:

		OctTree(const Data::Point3D &min, const Data::Point3D &max, ULInt id = 0);
		OctTree(Data::Box *box, MJMesh::OctTreeCell *root, ULInt id = 0);
		virtual ~OctTree();

		/*virtual Data::GraphNodeVector neighbors(const Data::BoxTreeCell *c, UInt side) const;
		virtual Data::GraphNodeVector neighbors(const Data::BoxTreeCell *c) const;*/

		using Data::OctTree::find;
		virtual MJMesh::OctTreeCell *find(const MJMesh::FrontElement3D *e) const;
		virtual Data::BoxTreeCell *find(const MJMesh::FrontElement *e) const;
	};
}

#endif //#ifndef _MJMESH_OCT_TREE_H_

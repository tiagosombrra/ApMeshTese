#ifndef _PMJMESH_OCT_TREE_H_
#define _PMJMESH_OCT_TREE_H_

#include "PMJMesh/Definitions.h"
#include "MJMesh/OctTree.h"

namespace PMJMesh
{
	class OctTree : public MJMesh::OctTree
	{
	public:

		OctTree(const Data::Point3D &min, const Data::Point3D &max, ULInt id = 0);
		OctTree(Data::Box *box, PMJMesh::OctTreeCell *root, ULInt id = 0);
		virtual ~OctTree();

		virtual Data::PositionMap positions(bool onlyLeaves = false) const;
		virtual void positions(Data::PositionMap &positions, bool onlyLeaves = false) const;

		virtual void adjustPosition();

	protected:

	};
}

#endif //#ifndef _PMJMESH_OCT_TREE_H_

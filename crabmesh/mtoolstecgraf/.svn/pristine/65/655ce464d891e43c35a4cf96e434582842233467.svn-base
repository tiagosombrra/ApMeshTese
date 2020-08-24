#ifndef _PMJMESH_QUAD_TREE_H_
#define _PMJMESH_QUAD_TREE_H_

#include "PMJMesh/Definitions.h"
#include "MJMesh/QuadTree.h"

namespace PMJMesh
{
	class QuadTree : public MJMesh::QuadTree
	{
	public:

		QuadTree(const Data::Point2D &min, const Data::Point2D &max, ULInt id = 0);
		QuadTree(Data::Box *box, PMJMesh::QuadTreeCell *root, ULInt id = 0);
		virtual ~QuadTree();

		virtual Data::PositionMap positions(bool onlyLeaves = false) const;
		virtual void positions(Data::PositionMap &positions, bool onlyLeaves = false) const;

		virtual void adjustPosition();

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

	};
}

#endif //#ifndef _PMJMESH_QUAD_TREE_H_

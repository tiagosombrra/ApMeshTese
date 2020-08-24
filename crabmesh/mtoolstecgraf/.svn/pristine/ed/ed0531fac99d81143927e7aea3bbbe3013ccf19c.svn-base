#ifndef _PMJMESH_OCT_TREE_CELL_H_
#define _PMJMESH_OCT_TREE_CELL_H_

#include "PMJMesh/Definitions.h"
#include "MJMesh/OctTreeCell.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace PMJMesh
{
	class OctTreeCell : public MJMesh::OctTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<PMJMesh::OctTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(PMJMesh::OctTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		OctTreeCell(PMJMesh::OctTreeCell *parent = NULL, ULInt id = 0);
		virtual ~OctTreeCell();

		virtual void setPosition(enum Data::Position position);
		virtual enum Data::Position getPosition() const;

		virtual void adjustPositionFromChildren();
		virtual void adjustPositionFromChildren(bool down);

		virtual void subdivide();

		virtual void positions(Data::PositionMap &positions, bool onlyLeaves = false) const;

#if USE_ARRAY
		using MJMesh::OctTreeCell::operator=;
		virtual PMJMesh::OctTreeCell &operator=(const PMJMesh::OctTreeCell &c);
#endif //#if USE_ARRAY

#if USE_GUI
		using MJMesh::OctTreeCell::draw;
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		enum Data::Position position;
	};
}

#endif //#ifndef _PMJMESH_OCT_TREE_CELL_H_

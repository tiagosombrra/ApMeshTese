#ifndef _PMJMESH_QUAD_TREE_CELL_H_
#define _PMJMESH_QUAD_TREE_CELL_H_

#include "PMJMesh/Definitions.h"
#include "MJMesh/QuadTreeCell.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace PMJMesh
{
	class QuadTreeCell : public MJMesh::QuadTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<PMJMesh::QuadTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(PMJMesh::QuadTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		QuadTreeCell(PMJMesh::QuadTreeCell *parent = NULL, ULInt id = 0);
		virtual ~QuadTreeCell();

		virtual void setPosition(enum Data::Position position);
		virtual enum Data::Position getPosition() const;

		virtual void adjustPositionFromChildren();
		virtual void adjustPositionFromChildren(bool down);

		virtual void subdivide();

		virtual void positions(Data::PositionMap &positions, bool onlyLeaves = false) const;

#if USE_ARRAY
		using MJMesh::QuadTreeCell::operator=;
		virtual PMJMesh::QuadTreeCell &operator=(const PMJMesh::QuadTreeCell &c);
#endif //#if USE_ARRAY

#if USE_GUI
		using MJMesh::QuadTreeCell::draw;
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		enum Data::Position position;
	};
}

#endif //#ifndef _PMJMESH_QUAD_TREE_CELL_H_

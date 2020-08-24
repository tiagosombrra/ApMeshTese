#ifndef _MJMESH_QUAD_TREE_CELL_H_
#define _MJMESH_QUAD_TREE_CELL_H_

#include "MJMesh/Definitions.h"
#include "Data/QuadTreeCell.h"
#include "MJMesh/FrontElement2D.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace MJMesh
{
	class QuadTreeCell : public Data::QuadTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::QuadTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::QuadTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		QuadTreeCell(MJMesh::QuadTreeCell *parent = NULL, ULInt id = 0);
		virtual ~QuadTreeCell();

#if USE_ATTRIBS
		virtual Real size() const;
#endif //#if USE_ATTRIBS

		virtual void subdivide();

		using Data::QuadTreeCell::find;
		virtual MJMesh::QuadTreeCell *find(MJMesh::FrontElement2D *e);
		virtual Data::BoxTreeCell *find(MJMesh::FrontElement *e);

#if USE_ARRAY
		using Data::QuadTreeCell::operator=;
#if USE_ATTRIBS
		virtual MJMesh::QuadTreeCell &operator=(const MJMesh::QuadTreeCell &c);
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY

#if USE_ATTRIBS
	protected:

		mutable Real s;
#endif //#if USE_ATTRIBS
	};
}

#endif //#ifndef _MJMESH_QUAD_TREE_CELL_H_

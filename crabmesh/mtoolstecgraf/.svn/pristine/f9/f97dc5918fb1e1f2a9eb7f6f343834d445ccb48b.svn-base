#ifndef _MJMESH_OCT_TREE_CELL_H_
#define _MJMESH_OCT_TREE_CELL_H_

#include "MJMesh/Definitions.h"
#include "Data/OctTreeCell.h"
#include "MJMesh/FrontElement3D.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace MJMesh
{
	class OctTreeCell : public Data::OctTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::OctTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::OctTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		OctTreeCell(MJMesh::OctTreeCell *parent = NULL, ULInt id = 0);
		virtual ~OctTreeCell();

#if USE_ATTRIBS
		virtual Real size() const;
#endif //#if USE_ATTRIBS

		virtual void subdivide();

		using Data::OctTreeCell::find;
		virtual MJMesh::OctTreeCell *find(MJMesh::FrontElement3D *e);
		virtual Data::BoxTreeCell *find(MJMesh::FrontElement *e);

#if USE_ARRAY
		using Data::OctTreeCell::operator=;
#if USE_ATTRIBS
		virtual MJMesh::OctTreeCell &operator=(const MJMesh::OctTreeCell &c);
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY

#if USE_ATTRIBS
	protected:

		mutable Real s;
#endif //#if USE_ATTRIBS
	};
}

#endif //#ifndef _MJMESH_OCT_TREE_CELL_H_

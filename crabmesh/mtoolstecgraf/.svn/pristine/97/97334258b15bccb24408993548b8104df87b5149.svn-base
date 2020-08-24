#ifndef _MJMESH_VERTEX_BOX_SEARCH_TREE_CELL_H_
#define _MJMESH_VERTEX_BOX_SEARCH_TREE_CELL_H_

#include "MJMesh/Definitions.h"

#include "Data/VertexBoxSearchTreeCell.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace MJMesh
{
#if USE_EXPERIMENTAL_5

	class VertexBoxSearchTreeCell2D : public Data::VertexBoxSearchTreeCell2D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::VertexBoxSearchTreeCell2D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::VertexBoxSearchTreeCell2D)
#endif //#if USE_TREE_CELL_POOL

	public:

		VertexBoxSearchTreeCell2D(Data::BoxTreeCell *cell = NULL, MJMesh::VertexBoxSearchTreeCell2D *parent = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTreeCell2D();

		using Data::VertexBoxSearchTreeCell2D::add;
		using Data::VertexBoxSearchTreeCell2D::remove;

		virtual bool add(const Data::Identifiable *s);
		virtual bool remove(const Data::Identifiable *s);

#if USE_ARRAY
		using Data::VertexBoxSearchTreeCell2D::operator=;
#endif //#if USE_ARRAY

	protected:

		using Data::VertexBoxSearchTreeCell2D::inRange;

		virtual bool inRange(const Data::Box *box, const Data::Identifiable *s) const;
	};

	class VertexBoxSearchTreeCell3D : public Data::VertexBoxSearchTreeCell3D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::VertexBoxSearchTreeCell3D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::VertexBoxSearchTreeCell3D)
#endif //#if USE_TREE_CELL_POOL

	public:

		VertexBoxSearchTreeCell3D(Data::BoxTreeCell *cell = NULL, MJMesh::VertexBoxSearchTreeCell3D *parent = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTreeCell3D();

		using Data::VertexBoxSearchTreeCell3D::add;
		using Data::VertexBoxSearchTreeCell3D::remove;

		virtual bool add(const Data::Identifiable *s);
		virtual bool remove(const Data::Identifiable *s);

#if USE_ARRAY
		using Data::VertexBoxSearchTreeCell3D::operator=;
#endif //#if USE_ARRAY

	protected:

		using Data::VertexBoxSearchTreeCell3D::inRange;

		virtual bool inRange(const Data::Box *box, const Data::Identifiable *s) const;
	};

#else

	class VertexBoxSearchTreeCell : public Data::VertexBoxSearchTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::VertexBoxSearchTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::VertexBoxSearchTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		VertexBoxSearchTreeCell(Data::BoxTreeCell *cell = NULL, MJMesh::VertexBoxSearchTreeCell *parent = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTreeCell();

		using Data::VertexBoxSearchTreeCell::add;
		using Data::VertexBoxSearchTreeCell::remove;

		virtual bool add(const Data::Identifiable *s);
		virtual bool remove(const Data::Identifiable *s);

#if USE_ARRAY
		using Data::VertexBoxSearchTreeCell::operator=;
#endif //#if USE_ARRAY

	protected:

		using Data::VertexBoxSearchTreeCell::inRange;

		virtual bool inRange(const Data::Box *box, const Data::Identifiable *s) const;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _MJMESH_VERTEX_BOX_SEARCH_TREE_CELL_H_

#ifndef _DATA_VERTEX_BOX_SEARCH_TREE_CELL_H_
#define _DATA_VERTEX_BOX_SEARCH_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxTreeCell.h"
#include "Data/BoxSearchTreeCell.h"
#include "Data/Vertex.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace Data
{
#if USE_EXPERIMENTAL_5

	class VertexBoxSearchTreeCell2D : public Data::BoxSearchTreeCell2D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::VertexBoxSearchTreeCell2D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::VertexBoxSearchTreeCell2D)
#endif //#if USE_TREE_CELL_POOL

	public:

		VertexBoxSearchTreeCell2D(BoxTreeCell *cell = NULL, VertexBoxSearchTreeCell2D *parent = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTreeCell2D();

#if USE_ARRAY
		using BoxSearchTreeCell2D::operator=;
#endif //#if USE_ARRAY

	protected:

		using BoxSearchTreeCell2D::inRange;

		virtual bool inRange(const Box *box, const Identifiable *s) const;
	};

	class VertexBoxSearchTreeCell3D : public Data::BoxSearchTreeCell3D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::VertexBoxSearchTreeCell3D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::VertexBoxSearchTreeCell3D)
#endif //#if USE_TREE_CELL_POOL

	public:

		VertexBoxSearchTreeCell3D(BoxTreeCell *cell = NULL, VertexBoxSearchTreeCell3D *parent = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTreeCell3D();

#if USE_ARRAY
		using BoxSearchTreeCell3D::operator=;
#endif //#if USE_ARRAY

	protected:

		using BoxSearchTreeCell3D::inRange;

		virtual bool inRange(const Box *box, const Identifiable *s) const;
	};

#else

	class VertexBoxSearchTreeCell : public Data::BoxSearchTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::VertexBoxSearchTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::VertexBoxSearchTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		VertexBoxSearchTreeCell(BoxTreeCell *cell = NULL, VertexBoxSearchTreeCell *parent = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTreeCell();

#if USE_ARRAY
		using BoxSearchTreeCell::operator=;
#endif //#if USE_ARRAY

	protected:

		using BoxSearchTreeCell::inRange;

		virtual bool inRange(const Box *box, const Identifiable *s) const;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _DATA_VERTEX_BOX_SEARCH_TREE_CELL_H_

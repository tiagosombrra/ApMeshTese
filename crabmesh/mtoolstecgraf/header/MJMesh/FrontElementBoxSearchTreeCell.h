#ifndef _MJMESH_FRONT_ELEMENT_BOX_SEARCH_TREE_CELL_H_
#define _MJMESH_FRONT_ELEMENT_BOX_SEARCH_TREE_CELL_H_

#include "MJMesh/Definitions.h"

#include "Data/FrontElementBoxSearchTreeCell.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace MJMesh
{
#if USE_EXPERIMENTAL_5

	class FrontElementBoxSearchTreeCell2D : public Data::FrontElementBoxSearchTreeCell2D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::FrontElementBoxSearchTreeCell2D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::FrontElementBoxSearchTreeCell2D)
#endif //#if USE_TREE_CELL_POOL

	public:

		FrontElementBoxSearchTreeCell2D(Data::BoxTreeCell *cell = NULL, MJMesh::FrontElementBoxSearchTreeCell2D *parent = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTreeCell2D();

		using Data::FrontElementBoxSearchTreeCell2D::add;
		using Data::FrontElementBoxSearchTreeCell2D::remove;

		virtual bool add(const Data::Identifiable *s);
		virtual bool remove(const Data::Identifiable *s);

#if USE_ARRAY
		using Data::FrontElementBoxSearchTreeCell2D::operator=;
#endif //#if USE_ARRAY

	protected:

		using Data::FrontElementBoxSearchTreeCell2D::inRange;

		virtual bool inRange(const Data::Box *box, const Data::Identifiable *s) const;
	};

	class FrontElementBoxSearchTreeCell3D : public Data::FrontElementBoxSearchTreeCell3D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::FrontElementBoxSearchTreeCell3D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::FrontElementBoxSearchTreeCell3D)
#endif //#if USE_TREE_CELL_POOL

	public:

		FrontElementBoxSearchTreeCell3D(Data::BoxTreeCell *cell = NULL, MJMesh::FrontElementBoxSearchTreeCell3D *parent = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTreeCell3D();

		using Data::FrontElementBoxSearchTreeCell3D::add;
		using Data::FrontElementBoxSearchTreeCell3D::remove;

		virtual bool add(const Data::Identifiable *s);
		virtual bool remove(const Data::Identifiable *s);

#if USE_ARRAY
		using Data::FrontElementBoxSearchTreeCell3D::operator=;
#endif //#if USE_ARRAY

	protected:

		using Data::FrontElementBoxSearchTreeCell3D::inRange;

		virtual bool inRange(const Data::Box *box, const Data::Identifiable *s) const;
	};

#else

	class FrontElementBoxSearchTreeCell : public Data::FrontElementBoxSearchTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<MJMesh::FrontElementBoxSearchTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(MJMesh::FrontElementBoxSearchTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		FrontElementBoxSearchTreeCell(Data::BoxTreeCell *cell = NULL, MJMesh::FrontElementBoxSearchTreeCell *parent = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTreeCell();

		using Data::FrontElementBoxSearchTreeCell::add;
		using Data::FrontElementBoxSearchTreeCell::remove;

		virtual bool add(const Data::Identifiable *s);
		virtual bool remove(const Data::Identifiable *s);

#if USE_ARRAY
		using Data::FrontElementBoxSearchTreeCell::operator=;
#endif //#if USE_ARRAY

	protected:

		using Data::FrontElementBoxSearchTreeCell::inRange;

		virtual bool inRange(const Data::Box *box, const Data::Identifiable *s) const;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _MJMESH_FRONT_ELEMENT_BOX_SEARCH_TREE_CELL_H_

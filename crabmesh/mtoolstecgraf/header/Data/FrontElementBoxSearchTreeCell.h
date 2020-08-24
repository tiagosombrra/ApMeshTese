#ifndef _DATA_FRONT_ELEMENT_BOX_SEARCH_TREE_CELL_H_
#define _DATA_FRONT_ELEMENT_BOX_SEARCH_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxSearchTreeCell.h"
#include "Data/FrontElement.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace Data
{
#if USE_EXPERIMENTAL_5

	class FrontElementBoxSearchTreeCell2D : public Data::BoxSearchTreeCell2D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::FrontElementBoxSearchTreeCell2D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::FrontElementBoxSearchTreeCell2D)
#endif //#if USE_TREE_CELL_POOL

	public:

		FrontElementBoxSearchTreeCell2D(BoxTreeCell *cell = NULL, FrontElementBoxSearchTreeCell2D *parent = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTreeCell2D();

#if USE_ARRAY
		using BoxSearchTreeCell::operator=;
#endif //#if USE_ARRAY

	protected:

		using BoxSearchTreeCell::inRange;

		virtual bool inRange(const Box *box, const Identifiable *s) const;
	};

	class FrontElementBoxSearchTreeCell3D : public Data::BoxSearchTreeCell3D
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::FrontElementBoxSearchTreeCell3D>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::FrontElementBoxSearchTreeCell3D)
#endif //#if USE_TREE_CELL_POOL

	public:

		FrontElementBoxSearchTreeCell3D(BoxTreeCell *cell = NULL, FrontElementBoxSearchTreeCell3D *parent = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTreeCell3D();

#if USE_ARRAY
		using BoxSearchTreeCell::operator=;
#endif //#if USE_ARRAY

	protected:

		using BoxSearchTreeCell::inRange;

		virtual bool inRange(const Box *box, const Identifiable *s) const;
	};

#else

	class FrontElementBoxSearchTreeCell : public Data::BoxSearchTreeCell
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::FrontElementBoxSearchTreeCell>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::FrontElementBoxSearchTreeCell)
#endif //#if USE_TREE_CELL_POOL

	public:

		FrontElementBoxSearchTreeCell(BoxTreeCell *cell = NULL, FrontElementBoxSearchTreeCell *parent = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTreeCell();

#if USE_ARRAY
		using BoxSearchTreeCell::operator=;
#endif //#if USE_ARRAY

	protected:

		using BoxSearchTreeCell::inRange;

		virtual bool inRange(const Box *box, const Identifiable *s) const;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _DATA_FRONT_ELEMENT_BOX_SEARCH_TREE_CELL_H_

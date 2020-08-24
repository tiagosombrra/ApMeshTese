#ifndef _DATA_BSP_TREE_CELL_H_
#define _DATA_BSP_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxTreeCell.h"

namespace Data
{
	class BSPTreeCell : public Data::BoxTreeCell
	{
	public:

#if USE_EXPERIMENTAL
		BSPTreeCell(ULInt id = 0);
		BSPTreeCell(BSPTree *tree, ULInt id = 0);
		BSPTreeCell(BSPTreeCell *parent, ULInt id = 0);
#else
		BSPTreeCell(BSPTreeCell *parent = NULL, ULInt id = 0);
#endif //#if USE_EXPERIMENTAL
		virtual ~BSPTreeCell();

#if USE_EXPERIMENTAL_5
		virtual UInt numAdjacents() const;

		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;

		virtual GraphNodeVector getAdjacents() const;

		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);
#endif //#if USE_EXPERIMENTAL_5

#if USE_ARRAY
		using RecursiveDecompositionTreeCell::operator=;
		virtual BSPTreeCell &operator=(const BSPTreeCell &c);
#endif //#if USE_ARRAY

		virtual void setSplit(UInt direction, Real coord);

		virtual UInt getSplitDirection() const;
		virtual Real getSplitCoord() const;

		///returns true if a cell with given id would be descendant of this cell,
		///  false otherwise (including if id == this->id)
		virtual bool isDescendant(ULInt id) const;

#if USE_EXPERIMENTAL_3
		virtual Point2D min2D() const;
		virtual Point3D min3D() const;
		virtual Point2D max2D() const;
		virtual Point3D max3D() const;
#else
		virtual Point min() const;
		virtual Point max() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual UInt position() const;

		virtual Real size() const;

		virtual void subdivide();

	protected:

		UShort direction;
		Real coord;

#if USE_EXPERIMENTAL_5
        GraphNode *adjacents[3];
#endif //#if USE_EXPERIMENTAL_5
	};
}

#endif //#ifndef _DATA_BSP_TREE_CELL_H_

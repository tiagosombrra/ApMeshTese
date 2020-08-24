#ifndef _DATA_BIN_TREE_CELL_H_
#define _DATA_BIN_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxTreeCell.h"
#include "Data/BinTree.h"

namespace Data
{
	class BinTreeCell : public Data::BoxTreeCell
	{
	public:

		BinTreeCell(BinTreeCell *parent = NULL, ULInt id = 0);
		virtual ~BinTreeCell();

#if USE_EXPERIMENTAL_5
		virtual UInt numAdjacents() const;

		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;

		virtual GraphNodeVector getAdjacents() const;

		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);
#endif //#if USE_EXPERIMENTAL_5

		virtual UInt dimension() const;

		virtual Real size() const;
		virtual UInt numSides() const;

		using BoxTreeCell::in;
		virtual bool in(Real x) const;

		using BoxTreeCell::optIn;
		virtual bool optIn(Real x) const;

		using BoxTreeCell::on;
		virtual bool on(Real x) const;

		using BoxTreeCell::optOn;
		virtual bool optOn(Real x) const;

		using BoxTreeCell::out;
		virtual bool out(Real x) const;

		using BoxTreeCell::optOut;
		virtual bool optOut(Real x) const;

#if USE_ARRAY
		using BoxTreeCell::operator=;
#if USE_EXPERIMENTAL_5
        BinTreeCell &operator=(const BinTreeCell &c);
#endif //#if USE_EXPERIMENTAL_5
#endif //#if USE_ARRAY

		virtual bool isChild(UInt i) const;
		virtual UInt position() const;

#if USE_EXPERIMENTAL_3
		virtual Point2D min2D() const;
		virtual Point2D max2D() const;
		virtual Point2D mid2D() const;
#else
		virtual Point min() const;
		virtual Point max() const;
		virtual Point mid() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual GeometricShape *makeShape() const;

		virtual void subdivide();

#if USE_EXPERIMENTAL_3
    protected:

        virtual Point3D min3D() const;
		virtual Point3D max3D() const;
		virtual Point3D mid3D() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_5
    protected:

        GraphNode *adjacents[3];
#endif //#if USE_EXPERIMENTAL_5
	};
}

#endif //#ifndef _DATA_BIN_TREE_CELL_H_

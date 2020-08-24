#ifndef _DATA_BOX_TREE_CELL_H_
#define _DATA_BOX_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/RecursiveDecompositionTreeCell.h"

namespace Data
{
	class BoxTreeCell :
		public Data::RecursiveDecompositionTreeCell
	{
	public:

#if USE_EXPERIMENTAL
		BoxTreeCell(ULInt id = 0);
		BoxTreeCell(BoxTree *tree, ULInt id = 0);
		BoxTreeCell(BoxTreeCell *parent, ULInt id = 0);
#else
		BoxTreeCell(BoxTreeCell *parent = NULL, ULInt id = 0);
#endif //#if USE_EXPERIMENTAL
		virtual ~BoxTreeCell();

		//virtual bool isSubdivided() const;

#if USE_ARRAY
		using RecursiveDecompositionTreeCell::operator=;
#endif //#if USE_ARRAY

		virtual UInt dimension() const;

		virtual bool isChild(UInt i) const;
		virtual UInt position() const;

		///returns true if a cell with given id would be descendant of this cell,
		///  false otherwise (including if id == this->id)
		virtual bool isDescendant(ULInt id) const;

#if USE_EXPERIMENTAL_3
		virtual Point2D min2D() const = 0;
		virtual Point3D min3D() const = 0;
		virtual Point2D max2D() const = 0;
		virtual Point3D max3D() const = 0;
		virtual Point2D mid2D() const;
		virtual Point3D mid3D() const;
#else
		virtual Point min() const = 0;
		virtual Point max() const = 0;
		virtual Point mid() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool optIn(const Point &p) const;
		virtual bool optIn(const Point *p) const;

		virtual bool in(const Box &box) const;
		virtual bool in(const Box *box) const;

		virtual bool optIn(const Box &box) const;
		virtual bool optIn(const Box *box) const;

		///tests if this cell is in the given box by looking only at the given side
		virtual bool in(const Box &box, UInt side) const;
		virtual bool in(const Box *box, UInt side) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool optOn(const Point &p) const;
		virtual bool optOn(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual bool optOut(const Point &p) const;
		virtual bool optOut(const Point *p) const;

		virtual bool intercept(const Box &box) const;
		virtual bool intercept(const Box *box) const;

		virtual bool optIntercept(const Box &box) const;
		virtual bool optIntercept(const Box *box) const;

		virtual bool outOf(const Box &box) const;
		virtual bool outOf(const Box *box) const;

		virtual GeometricShape *makeShape() const;
		virtual Box *box() const;

		virtual RecursiveDecompositionTreeCell *find(const Point &p) const;
		virtual RecursiveDecompositionTreeCell *find(const Point *p) const;

		virtual Real size() const;

		virtual UInt numSides() const;

		virtual GraphNodeList neighbors() const;
		virtual void neighbors(GraphNodeList &neighs) const;
		virtual GraphNodeList neighbors(UInt side) const;
		virtual void neighbors(UInt side, GraphNodeList &neighs) const;

		virtual void subdivide() = 0;

#if USE_GUI
		using RecursiveDecompositionTreeCell::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI
	};
}

#endif //#ifndef _DATA_BOX_TREE_CELL_H_

#ifndef _DATA_QUAD_TREE_CELL_H_
#define _DATA_QUAD_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxTreeCell.h"
#include "Data/Point2D.h"

namespace Data
{
	class QuadTreeCell : public Data::BoxTreeCell
	{
	public:

		enum ChildPosition
		{
			BOTTOM_LEFT = 0,
			LEFT_BOTTOM  = BOTTOM_LEFT,
			BL           = BOTTOM_LEFT,
			LB           = BOTTOM_LEFT,

			BOTTOM_RIGHT,
			RIGHT_BOTTOM = BOTTOM_RIGHT,
			BR           = BOTTOM_RIGHT,
			RB           = BOTTOM_RIGHT,

			TOP_RIGHT,
			RIGHT_TOP    = TOP_RIGHT,
			TR           = TOP_RIGHT,
			RT           = TOP_RIGHT,

			TOP_LEFT,
			LEFT_TOP     = TOP_LEFT,
			TL           = TOP_LEFT,
			LT           = TOP_LEFT
		};

		/*enum Side
		{
			BOTTOM = 0,
			B = BOTTOM,

			RIGHT,
			R = RIGHT,

			TOP,
			T = TOP,

			LEFT,
			L = LEFT
		};*/

	public:

		QuadTreeCell(QuadTreeCell *parent = NULL, ULInt id = 0);
		virtual ~QuadTreeCell();

#if USE_EXPERIMENTAL_5
		virtual UInt numAdjacents() const;

		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;

		virtual GraphNodeVector getAdjacents() const;
#endif //#if USE_EXPERIMENTAL_5

		virtual UInt dimension() const;

		using BoxTreeCell::child;
		virtual TreeNode *child(enum ChildPosition pos);

		virtual Real size() const;
		virtual UInt numSides() const;

		using BoxTreeCell::in;
		virtual bool in(Real x, Real y) const;

		using BoxTreeCell::optIn;
		virtual bool optIn(Real x, Real y) const;

		using BoxTreeCell::on;
		virtual bool on(Real x, Real y) const;

		using BoxTreeCell::optOn;
		virtual bool optOn(Real x, Real y) const;

		using BoxTreeCell::out;
		virtual bool out(Real x, Real y) const;

		using BoxTreeCell::optOut;
		virtual bool optOut(Real x, Real y) const;

		virtual bool isChild(UInt i) const;
		virtual UInt position() const;

#if USE_EXPERIMENTAL_3
		virtual Point2D min2D() const;
		virtual Point2D max2D() const;
#else
		virtual Point min() const;
		virtual Point max() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual void subdivide();

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);

		virtual void deleteChildren();
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))

#if USE_ARRAY
		using BoxTreeCell::operator=;
#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
		virtual QuadTreeCell &operator=(const QuadTreeCell &c);
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
#endif //#if USE_ARRAY

#if USE_EXPERIMENTAL_3
    protected:

        virtual Point3D min3D() const;
		virtual Point3D max3D() const;
#endif //#if USE_EXPERIMENTAL_3

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	protected:

		GraphNode *adjacents[5];
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	};
}

#endif //#ifndef _DATA_QUAD_TREE_CELL_H_

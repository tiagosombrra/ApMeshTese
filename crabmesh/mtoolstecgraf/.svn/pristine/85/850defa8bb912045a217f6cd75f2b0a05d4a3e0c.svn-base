#ifndef _DATA_OCT_TREE_CELL_H_
#define _DATA_OCT_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxTreeCell.h"
#include "Data/Point3D.h"

namespace Data
{
	class OctTreeCell : public Data::BoxTreeCell
	{
	public:

		//using the same opengl convention, meaning that the greater the z
		// coord, the nearer, the smaller the z coord, the farther.

		enum ChildPosition
		{
			BOTTOM_LEFT_FAR = 0,
			BOTTOM_FAR_LEFT   = BOTTOM_LEFT_FAR,
			LEFT_BOTTOM_FAR   = BOTTOM_LEFT_FAR,
			LEFT_FAR_BOTTOM   = BOTTOM_LEFT_FAR,
			FAR_BOTTOM_LEFT   = BOTTOM_LEFT_FAR,
			FAR_LEFT_BOTTOM   = BOTTOM_LEFT_FAR,
			BLF               = BOTTOM_LEFT_FAR,
			BFL               = BOTTOM_LEFT_FAR,
			LBF               = BOTTOM_LEFT_FAR,
			LFB               = BOTTOM_LEFT_FAR,
			FBL               = BOTTOM_LEFT_FAR,
			FLB               = BOTTOM_LEFT_FAR,

			BOTTOM_RIGHT_FAR,
			BOTTOM_FAR_RIGHT  = BOTTOM_RIGHT_FAR,
			RIGHT_BOTTOM_FAR  = BOTTOM_RIGHT_FAR,
			RIGHT_FAR_BOTTOM  = BOTTOM_RIGHT_FAR,
			FAR_BOTTOM_RIGHT  = BOTTOM_RIGHT_FAR,
			FAR_RIGHT_BOTTOM  = BOTTOM_RIGHT_FAR,
			BRF               = BOTTOM_RIGHT_FAR,
			BFR               = BOTTOM_RIGHT_FAR,
			RBF               = BOTTOM_RIGHT_FAR,
			RFB               = BOTTOM_RIGHT_FAR,
			FBR               = BOTTOM_RIGHT_FAR,
			FRB               = BOTTOM_RIGHT_FAR,

			TOP_RIGHT_FAR,
			TOP_FAR_RIGHT     = TOP_RIGHT_FAR,
			RIGHT_TOP_FAR     = TOP_RIGHT_FAR,
			RIGHT_FAR_TOP     = TOP_RIGHT_FAR,
			FAR_TOP_RIGHT     = TOP_RIGHT_FAR,
			FAR_RIGHT_TOP     = TOP_RIGHT_FAR,
			TRF               = TOP_RIGHT_FAR,
			TFR               = TOP_RIGHT_FAR,
			RTF               = TOP_RIGHT_FAR,
			RFT               = TOP_RIGHT_FAR,
			FTR               = TOP_RIGHT_FAR,
			FRT               = TOP_RIGHT_FAR,

			TOP_LEFT_FAR,
			TOP_FAR_LEFT      = TOP_LEFT_FAR,
			LEFT_TOP_FAR      = TOP_LEFT_FAR,
			LEFT_FAR_TOP      = TOP_LEFT_FAR,
			FAR_TOP_LEFT      = TOP_LEFT_FAR,
			FAR_LEFT_TOP      = TOP_LEFT_FAR,
			TLF               = TOP_LEFT_FAR,
			TFL               = TOP_LEFT_FAR,
			LTF               = TOP_LEFT_FAR,
			LFT               = TOP_LEFT_FAR,
			FTL               = TOP_LEFT_FAR,
			FLT               = TOP_LEFT_FAR,

			BOTTOM_LEFT_NEAR,
			BOTTOM_NEAR_LEFT  = BOTTOM_LEFT_NEAR,
			LEFT_BOTTOM_NEAR  = BOTTOM_LEFT_NEAR,
			LEFT_NEAR_BOTTOM  = BOTTOM_LEFT_NEAR,
			NEAR_BOTTOM_LEFT  = BOTTOM_LEFT_NEAR,
			NEAR_LEFT_BOTTOM  = BOTTOM_LEFT_NEAR,
			BLN               = BOTTOM_LEFT_NEAR,
			BNL               = BOTTOM_LEFT_NEAR,
			LBN               = BOTTOM_LEFT_NEAR,
			LNB               = BOTTOM_LEFT_NEAR,
			NBL               = BOTTOM_LEFT_NEAR,
			NLB               = BOTTOM_LEFT_NEAR,

			BOTTOM_RIGHT_NEAR,
			BOTTOM_NEAR_RIGHT = BOTTOM_RIGHT_NEAR,
			RIGHT_BOTTOM_NEAR = BOTTOM_RIGHT_NEAR,
			RIGHT_NEAR_BOTTOM = BOTTOM_RIGHT_NEAR,
			NEAR_BOTTOM_RIGHT = BOTTOM_RIGHT_NEAR,
			NEAR_RIGHT_BOTTOM = BOTTOM_RIGHT_NEAR,
			BRN               = BOTTOM_RIGHT_NEAR,
			BNR               = BOTTOM_RIGHT_NEAR,
			RBN               = BOTTOM_RIGHT_NEAR,
			RNB               = BOTTOM_RIGHT_NEAR,
			NBR               = BOTTOM_RIGHT_NEAR,
			NRB               = BOTTOM_RIGHT_NEAR,

			TOP_RIGHT_NEAR,
			TOP_NEAR_RIGHT    = TOP_RIGHT_NEAR,
			RIGHT_TOP_NEAR    = TOP_RIGHT_NEAR,
			RIGHT_NEAR_TOP    = TOP_RIGHT_NEAR,
			NEAR_TOP_RIGHT    = TOP_RIGHT_NEAR,
			NEAR_RIGHT_TOP    = TOP_RIGHT_NEAR,
			TRN               = TOP_RIGHT_NEAR,
			TNR               = TOP_RIGHT_NEAR,
			RTN               = TOP_RIGHT_NEAR,
			RNT               = TOP_RIGHT_NEAR,
			NTR               = TOP_RIGHT_NEAR,
			NRT               = TOP_RIGHT_NEAR,

			TOP_LEFT_NEAR,
			TOP_NEAR_LEFT     = TOP_LEFT_NEAR,
			LEFT_TOP_NEAR     = TOP_LEFT_NEAR,
			LEFT_NEAR_TOP     = TOP_LEFT_NEAR,
			NEAR_TOP_LEFT     = TOP_LEFT_NEAR,
			NEAR_LEFT_TOP     = TOP_LEFT_NEAR,
			TLN               = TOP_LEFT_NEAR,
			TNL               = TOP_LEFT_NEAR,
			LTN               = TOP_LEFT_NEAR,
			LNT               = TOP_LEFT_NEAR,
			NTL               = TOP_LEFT_NEAR,
			NLT               = TOP_LEFT_NEAR
		};

		/*enum Side
		{
			BOTTOM = 0,
			B = BOTTOM,

			RIGHT,
			R = RIGHT,

			FAR,
			F = FAR,

			TOP,
			T = TOP,

			LEFT,
			L = LEFT,

			NEAR,
			N = NEAR
		};*/

	public:

		OctTreeCell(OctTreeCell *parent = NULL, ULInt id = 0);
		virtual ~OctTreeCell();

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
		virtual bool in(Real x, Real y, Real z) const;

		using BoxTreeCell::optIn;
		virtual bool optIn(Real x, Real y, Real z) const;

		using BoxTreeCell::on;
		virtual bool on(Real x, Real y, Real z) const;

		using BoxTreeCell::optOn;
		virtual bool optOn(Real x, Real y, Real z) const;

		using BoxTreeCell::out;
		virtual bool out(Real x, Real y, Real z) const;

		using BoxTreeCell::optOut;
		virtual bool optOut(Real x, Real y, Real z) const;

		virtual bool isChild(UInt i) const;
		virtual UInt position() const;

#if USE_EXPERIMENTAL_3
		virtual Point3D min3D() const;
		virtual Point3D max3D() const;
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
		virtual OctTreeCell &operator=(const OctTreeCell &c);
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
#endif //#if USE_ARRAY

#if USE_EXPERIMENTAL_3
    protected:

        virtual Point2D min2D() const;
		virtual Point2D max2D() const;
#endif //#if USE_EXPERIMENTAL_3

#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	protected:

		GraphNode *adjacents[9];
#endif //#if ((USE_EXPERIMENTAL_5) || (USE_EXPERIMENTAL))
	};
}

#endif //#ifndef _DATA_OCT_TREE_CELL_H_

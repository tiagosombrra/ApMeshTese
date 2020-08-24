#ifndef _PMJMESH_BOX_TREE_SHIFTER_H_
#define _PMJMESH_BOX_TREE_SHIFTER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/TreeRefiner.h"

namespace PMJMesh
{
	class BoxTreeShifter : public Performer::TreeRefiner
	{
	public:

		BoxTreeShifter(UInt dimension, Data::BoxTree *tree,
			enum ShiftType type = LATERAL, Real factor = 0.5, UInt shift = 0, bool skippingCenter = true);
		virtual ~BoxTreeShifter();

		virtual void setType(enum ShiftType type);
		virtual enum ShiftType getType() const;

		virtual void setRestore(bool restore);

		virtual void setFactor(Real factor);

		virtual void setSkippingCenter(bool skippingCenter);
		virtual bool isSkippingCenter() const;

		virtual void setLateral(bool lateral);
		virtual bool isLateral() const;
		virtual void setDiagonal(bool diagonal);
		virtual bool isDiagonal() const;
		virtual void setDiagonalVertex(bool diagonalVertex);
		virtual bool isDiagonalVertex() const;

		virtual UInt getShift() const;
		virtual UInt state() const;
		virtual UInt oppositeState() const;

		virtual UInt numStates() const;
		virtual UInt numShiftedStates() const;
		virtual UInt maxStates() const;
		virtual UInt minState(UInt shiftType) const;
		virtual UInt maxState(UInt shiftType) const;

		virtual Data::UIntVector sides(UInt state) const;

		virtual void refine(Data::TreeNode *n);

		virtual void execute();

	protected:

		virtual void displace(const Data::BoxTreeCell *c, const Data::Point *min, const Data::Point *max,
			const Data::GraphNodeList &moreNeighs, Data::Point *newMin, Data::Point *newMax) const;

		virtual void setInitialShift(UInt shift);

		virtual UInt sides(UInt shift, UInt shifts[]) const;

		virtual UInt minShift(UInt shiftType) const;
		virtual UInt maxShift(UInt shiftType) const;
		virtual UInt maxShift() const;

		virtual void fillArrays(UInt i, UInt shift);

		virtual void executeForLeaves();
		virtual void executeLateral();
		virtual void executeDiagonal();
		virtual void executeDiagonalVertex();

		virtual Data::Point *makePoint(const Data::Point *p) const;

		virtual void clearOriginalPoints();

		virtual void makeArrays(UInt numDirs);
		virtual void clearArrays();

		virtual void subdivide(Data::TreeNode *n);

	protected:

		UInt dimension;

#if USE_C__11
		Data::PointHashMap originalMinPoints;
		Data::PointHashMap originalMaxPoints;
#else
		Data::PointMap originalMinPoints;
		Data::PointMap originalMaxPoints;
#endif //#if USE_C__11

		Real factor;
		bool skippingCenter;

		bool lateral;
		bool diagonal;
		bool diagonalVertex; //3 dimensions only

		UInt shift;
		bool odd;

		//for shifts
		UInt numDirs;
		UInt *coord;
		bool *positive;
		UInt *side;
		//UInt *opposite;

		bool restore;

		bool adjustSmallerCells;
	};
}

#endif //#ifndef _PMJMESH_BOX_TREE_SHIFTER_H_

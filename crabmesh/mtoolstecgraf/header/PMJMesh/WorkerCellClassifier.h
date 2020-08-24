#ifndef _PMJMESH_WORKER_CELL_CLASSIFIER_H_
#define _PMJMESH_WORKER_CELL_CLASSIFIER_H_

#include "PMJMesh/Definitions.h"

#if (!USE_WORKER_CELL_CLASSIFIER_2)

#include "PMJMesh/AngleTreeCellClassifier.h"
#include "PMJMesh/LoadCalculator.h"

namespace PMJMesh
{
	class WorkerCellClassifier : public PMJMesh::AngleTreeCellClassifier
	{
	public:

		WorkerCellClassifier(UInt dimension, const Data::BoxTree *tree,
			const Data::Box *box, const Data::FrontElementList &oldFront,
			const Data::FrontElementList &newFront,
			const Data::FrontElementList &commonFront);
		virtual ~WorkerCellClassifier();

		virtual void setBox(const Data::Box *box);

		using PMJMesh::AngleTreeCellClassifier::classify;
		virtual void classify();

		using PMJMesh::AngleTreeCellClassifier::adjustPosition;
		virtual void adjustPosition();

		virtual void execute();

	protected:

		virtual bool outOfBox(const Data::BoxTreeCell *c) const;

		virtual void findCells(const Data::BoxTreeCell *c, const Data::FrontElementList &fels, bool geometricTests, Data::GraphNodeSet &cells) const;

		virtual void findOnCells();
		virtual void findOnCells(Data::BoxTreeCell *c, bool geometricTests,
			Data::FrontElementList &commonFront, Data::FrontElementList &newFront,
			Data::FrontElementList &oldFront, bool testOnlyOldFront = false);
		virtual void findInCells();
		virtual void findOutCells();

		virtual void neighbors(const Data::BoxTreeCell *c, Data::GraphNodeList &neighs) const;
		virtual Data::GraphNodeList neighbors(const Data::BoxTreeCell *c) const;
		virtual void setChildrenPosition(const Data::BoxTreeCell *c, enum Data::Position pos);
		virtual void setNeighborsPosition(Data::BoxTreeCell *c, enum Data::Position pos);

	protected:

		Data::FrontElementList oldFront;
		Data::FrontElementList newFront;
		Data::FrontElementList commonFront;

		Data::FrontElementList elements;
		Data::GraphNodeList inCells;
		Data::GraphNodeList outCells;

		const Data::Box *box;

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	public:

		static Real timeFindOnCellsInit, timeFindOnCellsInTest, timeFindOnCellsOutOfBoxTest,
			timeFindOnCellsOldPosOn,
			timeFindOnCellsNotOnCommonAndNotNewTest, timeFindOnCellsNotOnCommonAndNotNew,
			timeFindOnCellsCommonNewOld, timeFindOnCellsOnCommonOrNewTest, timeFindOnCellsOnCommonOrNew,
			timeFindOnCellsOnOldTest, timeFindOnCellsOnOld, timeFindOnCellsElse,
			timeFindInCellsCells, timeFindInCellsNeighbors, timeFindInCellsParents,
			timeFindOutCells;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	};
}

#endif //#if (!USE_WORKER_CELL_CLASSIFIER_2)

#endif //#ifndef _PMJMESH_WORKER_CELL_CLASSIFIER_H_

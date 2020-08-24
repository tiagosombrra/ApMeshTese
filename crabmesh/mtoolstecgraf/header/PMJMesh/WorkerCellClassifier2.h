#ifndef _PMJMESH_WORKER_CELL_CLASSIFIER_2_
#define _PMJMESH_WORKER_CELL_CLASSIFIER_2_

#include "PMJMesh/Definitions.h"

#if (USE_NEW_FRONT_UPDATER)

#define DEBUG_ORDERED_CONTAINERS false

#include "PMJMesh/AngleTreeCellClassifier.h"
#include "PMJMesh/LoadCalculator.h"

#if DEBUG_ORDERED_CONTAINERS
#include "Data/BoxTreeCell.h"
#endif //#if DEBUG_ORDERED_CONTAINERS

#define USE_IN_CELLS_AS_LIST false

namespace PMJMesh
{
	class WorkerCellClassifier2 : public PMJMesh::AngleTreeCellClassifier
	{
	public:

		WorkerCellClassifier2(UInt dimension, const Data::BoxTree *tree,
			const Data::Box *box, const PMJMesh::Front *oldFront,
			const PMJMesh::Front *newFront, const PMJMesh::Front *commonFront,
			bool newFrontInverted);
		virtual ~WorkerCellClassifier2();

		virtual void setBox(const Data::Box *box);

		virtual void execute();

	protected:

		//same as in WorkerCellClassifier
		virtual bool outOfBox(const Data::BoxTreeCell *c) const;

		//same as in WorkerCellClassifier
		virtual void neighbors(const Data::BoxTreeCell *c, UInt side, Data::GraphNodeList &neighs) const;
		virtual void neighbors(const Data::BoxTreeCell *c, Data::GraphNodeList &neighs) const;

		//almost same as in WorkerCellClassifier
#if USE_IN_CELLS_AS_LIST
		virtual void setNeighborsPosition(Data::BoxTreeCell *c, enum Data::Position pos);
#else
		virtual void setNeighborsPosition(Data::BoxTreeCell *c, enum Data::Position pos, bool removingFromIn);
#endif //#if USE_IN_CELLS_AS_LIST

		virtual void classifyBasedOnOldONCells();
		virtual void classifyBasedOnOldONCellsWithGeometricTests(Data::BoxTreeCell *c,
			Data::FrontElementList &commonFront, Data::FrontElementList &newFront);
		virtual void classifyBasedOnOldONCells(Data::BoxTreeCell *c,
			Data::FrontElementList &commonFront, Data::FrontElementList &newFront);

		virtual void classifyBasedOnNormal();
		virtual void classifyBasedOnNormalFromInCells();
		virtual void classifyBasedOnPoint();

		virtual void adjustPositionFromChildren(Data::BoxTreeCell *c);

	protected:

		const PMJMesh::Front *oldFront;
		const PMJMesh::Front *newFront;
		const PMJMesh::Front *commonFront;

		bool newFrontInverted;

		const Data::Box *box;

		Data::GraphNodeList upmostCells;

		Data::GraphNodeList outCells;


#if DEBUG_ORDERED_CONTAINERS
#if USE_C__11
		typedef std::unordered_set<Data::GraphNode *, Data::Identifiable::Hash, Data::Identifiable::Equal> GraphNodeHash;
		typedef std::unordered_map<Data::BoxTreeCell *, Data::FrontElementList, Data::Identifiable::Hash, Data::Identifiable::Equal> CellFEListHashMap;
#else
		typedef std::set<Data::GraphNode *, Data::Identifiable::Less> GraphNodeSet;
		typedef std::map<Data::BoxTreeCell *, Data::FrontElementList, Data::Identifiable::Less> CellFEListMap;
#endif //#if USE_C__11
#endif //#if DEBUG_ORDERED_CONTAINERS

#if USE_IN_CELLS_AS_LIST
		Data::GraphNodeList inCells;
#else
#if DEBUG_ORDERED_CONTAINERS
#if USE_C__11
		GraphNodeHash inCells;
#else
		GraphNodeSet inCells;
#endif //#if USE_C__11
#else
#if USE_C__11
		Data::GraphNodeHash inCells;
#else
		Data::GraphNodeSet inCells;
#endif //#if USE_C__11
#endif //#if DEBUG_ORDERED_CONTAINERS
#endif //#if USE_IN_CELLS_AS_LIST

#if USE_C__11
		CellFEListHashMap felsMap;
#else
		CellFEListMap felsMap;
#endif //#if USE_C__11
	};
}

#endif //#if (USE_NEW_FRONT_UPDATER)

#endif //#ifndef _PMJMESH_WORKER_CELL_CLASSIFIER_2_

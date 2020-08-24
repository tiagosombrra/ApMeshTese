#ifndef _PMJMESH_ANGLE_TREE_CELL_CLASSIFIER_H_
#define _PMJMESH_ANGLE_TREE_CELL_CLASSIFIER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/AngleTreeCellClassifier.h"

namespace PMJMesh
{
	class AngleTreeCellClassifier : public Performer::AngleTreeCellClassifier
	{
	friend
		class PMJMesh::TreeGenerator;
	friend
		class PMJMesh::MinTreeRefiner;
	friend
		class PMJMesh::NeighborTreeRefiner;

	public:

		AngleTreeCellClassifier(UInt dimension, const Parallel::ThreadManager *tm, const Data::Boundary *boundary,
			const Data::BoxTree *tree, Data::Position doNotClassify = Data::OUT,
			bool classifyChildren = false);
		/*AngleTreeCellClassifier(UInt dimension, const Parallel::ThreadManager *tm, const Data::Front *front,
			const Data::BoxTree *tree, Data::Position doNotClassify = Data::OUT,
			bool classifyChildren = false);*/
		virtual ~AngleTreeCellClassifier();

		using Performer::AngleTreeCellClassifier::setPosition;
		virtual void setPosition(const Data::PositionMap &pos);

		virtual void setDoNotClassify(Data::Position doNotClassify);
		virtual void setClassifyChildren(bool classifyChildren);
		virtual void setThreadManager(const Parallel::ThreadManager *tm);

		//this method removes redundancies in the classification of the cells,
		// i.e., when all the children of a cell have the same classification,
		// the classification (in the PositionMap data structure) is replaced
		// by the classification of the cell. this simple procedure diminishes
		// the amount of memory required, which is good for message passing.
		// this method is used only by the subclasses MasterCellClassifier e
		// WorkerCellClassifier.
		virtual void adjustPosition();

	protected:

		virtual void setPosition(const Data::BoxTreeCell *cell, enum Data::Position pos);

		//these are used in conjunction with void adjustPosition() method
		//virtual bool adjustPosition(ULInt id, UInt numChildren, Data::Position pos, ULInt max);
		//virtual void removeChildrenPosition(ULInt id, ULInt max, UInt numChildren);
		//virtual void removeDescendantsPositions(ULInt id, ULInt max, UInt numChildren);

		//this is used in conjunction with void adjustPosition() method, and
		// tells if the cell should be ignored in tree disrefinement
		// according to position
		virtual bool ignore(ULInt id, Data::Position pos) const;

		virtual void classifyChildren(const Data::BoxTreeCell *cell,
			//const Data::FrontElementList &elements);
			const Data::FrontElementList *elements);

		virtual bool findClassificationDirections(const Data::FrontElementList &felsInCell, Int directions[]);

		virtual void findOnNeighbors(Data::GraphNodeList &neighsInSide) const;
		virtual enum Data::Position classifyByOnNeighbor(UInt side,
			const Data::FrontElementList &felsInNeighbor) const;
		virtual enum Data::Position classifyByOnNeighbor(
			const Data::FrontElementList *elements, UInt side, Data::GraphNodeList &neighsInSide) const;
		virtual enum Data::Position classifyByOnNeighbor(const Data::BoxTreeCell *c,
			const Data::FrontElementList *elements) const;

		virtual void setChildrenPosition(const Data::BoxTreeCell *cell, enum Data::Position pos);

		virtual Performer::AnglePointInObjectDecider *makeApiod(Data::Point *p,
			const Data::FrontElementList *elements, bool canBeUnknown) const;

		//virtual Data::FrontElementList findElements() const;
		virtual const Data::FrontElementList *findElements() const;

		using Performer::AngleTreeCellClassifier::classify;

		virtual enum Data::Position classify(const Data::BoxTreeCell *cell,
			//const Data::FrontElementList &elements);
			const Data::FrontElementList *elements);

	protected:

		Data::Position doNotClassify;

		const Parallel::ThreadManager *tm;

		bool classifyChildrenEnabled;
	};
}

#endif //#ifndef _PMJMESH_ANGLE_TREE_CELL_CLASSIFIER_H_

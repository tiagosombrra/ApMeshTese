#ifndef _PMJMESH_WORKER_MIN_TREE_REFNIER_H_
#define _PMJMESH_WORKER_MIN_TREE_REFNIER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/MinTreeRefiner.h"

namespace PMJMesh
{
	class WorkerMinTreeRefiner : public PMJMesh::MinTreeRefiner
	{
	public:

		WorkerMinTreeRefiner(UInt dimension, Data::BoxTree *tree,
			UInt minRefinementLevel,
			const Parallel::ThreadManager *tm,
			const Data::Box *box);
		virtual ~WorkerMinTreeRefiner();

		virtual void setBox(const Data::Box *box);

		virtual Data::GraphNodeList *getRefinedCells() const;

		virtual UInt findMinLevel() const;

		virtual void execute();

	protected:

		//using Performer::MinTreeRefiner::refine;
		//virtual void refine(Data::BoxTreeCell *c);

		using Performer::MinTreeRefiner::subdivide;
		virtual void subdivide(Data::BoxTreeCell *c);

	protected:

		const Data::Box *box;

		Data::GraphNodeList *refinedCells;
	};
}

#endif //#ifndef _PMJMESH_WORKER_MIN_TREE_REFNIER_H_

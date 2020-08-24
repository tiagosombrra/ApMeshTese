#ifndef _PMJMESH_WORKER_MIN_TREE_DISREFINER_H_
#define _PMJMESH_WORKER_MIN_TREE_DISREFINER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/TreeRefiner.h"

namespace PMJMesh
{
	class WorkerMinTreeDisrefiner : public Performer::TreeRefiner
	{
	public:

		WorkerMinTreeDisrefiner(UInt dimension,
			Data::BoxTree *tree, Data::GraphNodeList *refinedCells);
		virtual ~WorkerMinTreeDisrefiner();

		virtual void setDimension(UInt dimension);

		virtual void setRefinedCells(Data::GraphNodeList *refinedCells);

		virtual void execute();

		virtual void refine(Data::TreeNode *n);

	protected:

		virtual void subdivide(Data::TreeNode *n);

	protected:

		UInt dimension;

		Data::GraphNodeList *refinedCells;
	};
}

#endif //#ifndef _PMJMESH_WORKER_MIN_TREE_DISREFINER_H_

#ifndef _PMJMESH_TREE_UPDATER_H_
#define _PMJMESH_TREE_UPDATER_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Builder.h"
#include "Performer/TreeRefiner.h"

namespace PMJMesh
{
	class TreeUpdater : public Parallel::Builder
	{
	public:

		TreeUpdater(UInt dimension, Data::BoxTree *tree, Parallel::Communicator *comm);
		virtual ~TreeUpdater();

		virtual void setTree(Data::BoxTree *tree);
		virtual void setDimension(UInt dimension);

		virtual void execute();

	protected:

		virtual void gather();
		virtual void update(Data::BoxTreeCell *c, Data::BoxTreeCell **others, Int numOthers, Int rank, Int root);
		virtual void update();

		virtual void updateTree();

		virtual void refine(Data::BoxTreeCell *c, bool force);

		virtual void subdivide(Data::TreeNode *n);

	protected:

		UInt dimension;

		Data::BoxTree *tree;

		Data::BoxTree **trees;

		Data::GraphNodeList leaves;

	};
}

#endif //#ifndef _PMJMESH_TREE_UPDATER_H_

#include "MJMesh/NeighborTreeRefiner.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

const Performer::Status MJMesh::NeighborTreeRefiner::notInit =
	Performer::Status(MJMesh::NeighborTreeRefiner::NOT_INIT, "not initiated");
const Performer::Status MJMesh::NeighborTreeRefiner::ended =
	Performer::Status(MJMesh::NeighborTreeRefiner::ENDED, "ended");

MJMesh::NeighborTreeRefiner::NeighborTreeRefiner(/*UInt dimension, */Data::BoxTree *tree) :
	Performer::NeighborTreeRefiner(tree, 1)
{
	this->status = MJMesh::NeighborTreeRefiner::notInit;
}

MJMesh::NeighborTreeRefiner::~NeighborTreeRefiner()
{

}

void MJMesh::NeighborTreeRefiner::execute()
{
	if (!this->tree)
	{
		return;
	}

#if USE_GUI
	static_cast<Data::BoxTree *>(this->tree)->freeze();
#endif //#if USE_GUI

	//do
	//{
		//this->refined = false;

		/*Data::GraphNodeList leaves = this->tree->getLeaves();

		while (!leaves.empty())
		{
			this->refine(static_cast<Data::BoxTreeCell *>(leaves.back()));

			leaves.pop_back();
		}*/

	//}
	//while (this->refined);

	if (static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->dimension() == 2)
	{
#if USE_EXPERIMENTAL_TREE
		Data::GraphNodeList leaves;

		this->tree->getLeaves(leaves);
#else
		Data::GraphNodeSet leaves = this->tree->getLeavesSet();
#endif //#if USE_EXPERIMENTAL_TREE

#if USE_EXPERIMENTAL_TREE
		for (Data::GraphNodeList::iterator iter = leaves.begin(), end = leaves.end();
#else
		for (Data::GraphNodeSet::iterator iter = leaves.begin(), end = leaves.end();
#endif //#if USE_EXPERIMENTAL_TREE
			 iter != end; iter++)
		{
			this->refine(const_cast<Data::BoxTreeCell *>(static_cast<const Data::BoxTreeCell *>((*iter))));
		}
	}
	else
	{
		this->executeWithSet();
	}

#if USE_GUI
    for (Data::Tree::LeafIterator iter = this->tree->lBegin();
         iter != this->tree->lEnd(); iter++)
    {
        static_cast<Data::BoxTreeCell *>((*iter))->box();
    }

	static_cast<Data::BoxTree *>(this->tree)->unfreeze();
#endif //#if USE_GUI

	this->status = MJMesh::NeighborTreeRefiner::ended;
}

void MJMesh::NeighborTreeRefiner::fillSet()
{
	/*Data::GraphNodeList leaves = this->tree->getLeaves();

	while (!leaves.empty())
	{
		this->fillSet(static_cast<Data::BoxTreeCell *>(leaves.back()));

		leaves.pop_back();
	}*/

#if USE_EXPERIMENTAL_TREE
	for (Data::Tree::LeafIterator iter = this->tree->lBegin(), end = this->tree->lEnd();
#else
	for (Data::GraphNodeSet::const_iterator iter = this->tree->getLeavesSet().begin(), end = this->tree->getLeavesSet().end();
#endif //#if USE_EXPERIMENTAL_TREE
		 iter != end; iter++)
	{
		this->fillSet(const_cast<Data::BoxTreeCell *>(static_cast<const Data::BoxTreeCell *>((*iter))));
	}
}

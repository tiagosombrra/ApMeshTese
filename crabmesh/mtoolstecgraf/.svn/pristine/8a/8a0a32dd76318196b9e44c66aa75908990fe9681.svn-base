#include "PMJMesh/WorkerMinTreeDisrefiner.h"

#include "Data/BoxTree.h"
#include "Data/TreeNode.h"
#include "Data/BoxTreeCell.h"

PMJMesh::WorkerMinTreeDisrefiner::WorkerMinTreeDisrefiner(UInt dimension,
	Data::BoxTree *tree, Data::GraphNodeList *refinedCells) :
		Performer::TreeRefiner(tree)
{
	this->setDimension(dimension);

	this->setRefinedCells(refinedCells);
}

PMJMesh::WorkerMinTreeDisrefiner::~WorkerMinTreeDisrefiner()
{
	if (this->refinedCells)
	{
		delete this->refinedCells;
	}
}

void PMJMesh::WorkerMinTreeDisrefiner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::WorkerMinTreeDisrefiner::setRefinedCells(Data::GraphNodeList *refinedCells)
{
	this->refinedCells = refinedCells;
}

void PMJMesh::WorkerMinTreeDisrefiner::execute()
{
	if (!this->refinedCells)
	{
		return;
	}

	while (!this->refinedCells->empty())
	{
		this->refine(static_cast<Data::TreeNode *>(this->refinedCells->front()));

		this->refinedCells->pop_front();
	}

	delete this->refinedCells;

	this->refinedCells = NULL;
}

void PMJMesh::WorkerMinTreeDisrefiner::refine(Data::TreeNode *n)
{
	if (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) != Data::ON)
	{
		n->deleteChildren();

#if ((USE_GUI) || (!USE_EXPERIMENTAL_TREE))
		n->setTree(this->tree);
#endif //#if ((USE_GUI) || (!USE_EXPERIMENTAL_TREE))
	}
}

void PMJMesh::WorkerMinTreeDisrefiner::subdivide(Data::TreeNode */*n*/)
{

}

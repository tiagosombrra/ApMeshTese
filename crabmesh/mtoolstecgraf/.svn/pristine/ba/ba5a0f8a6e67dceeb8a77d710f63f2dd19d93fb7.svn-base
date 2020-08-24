#include "PMJMesh/WorkerMinTreeRefiner.h"

#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Parallel/ThreadManager.h"

using namespace PMJMesh;

PMJMesh::WorkerMinTreeRefiner::WorkerMinTreeRefiner(UInt dimension, Data::BoxTree *tree,
	UInt minRefinementLevel,
	const Parallel::ThreadManager *tm, const Data::Box *box) :
		PMJMesh::MinTreeRefiner(dimension, tree, NULL, minRefinementLevel, false, tm)
{
	this->refinedCells = NULL;
	//this->setMinLevel(minRefinementLevel);
	this->setBox(box);
}

PMJMesh::WorkerMinTreeRefiner::~WorkerMinTreeRefiner()
{

}

void PMJMesh::WorkerMinTreeRefiner::setBox(const Data::Box *box)
{
	this->box = box;
}

Data::GraphNodeList *PMJMesh::WorkerMinTreeRefiner::getRefinedCells() const
{
	return this->refinedCells;
}

UInt PMJMesh::WorkerMinTreeRefiner::findMinLevel() const
{
	return this->getMinLevel();
}

void PMJMesh::WorkerMinTreeRefiner::execute()
{
	if ((!this->tree) || (this->getMinLevel() == 0))
	{
		return;
	}

	this->refinedCells = new Data::GraphNodeList();

	//Performer::MinTreeRefiner::execute();

	Data::GraphNodeList cells, insides;

	cells.push_back(this->tree->getRoot());

	while (!cells.empty())
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(cells.front());

		cells.pop_front();

		if (this->box->optIn(c->box()))
		{
			insides.push_back(c);

			continue;
		}

		if (c->outOf(this->box))
		{
			continue;
		}

		if ((!c->isSubdivided()) && (c->level() < this->minLevel))
		{
#if USE_OPENMP
			if (this->tm->numThreads() == 1)
#endif //#if USE_OPENMP
			{
				this->subdivide(c);
			}
#if USE_OPENMP
			else
			{
				#pragma omp critical (worker_min_refiner)
				{
					if (!c->isSubdivided())
					{
						this->subdivide(c);
					}
				}
			}
#endif //#if USE_OPENMP
		}

		for (UInt i = 0; i < c->numChildren(); i++)
		{
			cells.push_back(c->child(i));
		}
	}

	for (Data::GraphNodeList::iterator iter = insides.begin();
		 iter != insides.end(); iter++)
	{
		this->refine(static_cast<Data::BoxTreeCell *>((*iter)));
	}
}

/*void PMJMesh::WorkerMinTreeRefiner::refine(Data::BoxTreeCell *c)
{
	//if (this->box->out(c->box()))
	//if (this->box->optOut(c->box()))
	if (c->outOf(this->box))
	{
		return;
	}

	//Performer::MinTreeRefiner::refine(c);
	PMJMesh::MinTreeRefiner::refine(c);
}*/

void PMJMesh::WorkerMinTreeRefiner::subdivide(Data::BoxTreeCell *c)
{
	if (c->getTree())
	{
#if ((USE_GUI) || (!USE_EXPERIMENTAL_TREE))
		c->setTree(NULL);
#endif //#if ((USE_GUI) || (!USE_EXPERIMENTAL_TREE))

		this->refinedCells->push_back(c);
	}

	Performer::MinTreeRefiner::subdivide(c);
}

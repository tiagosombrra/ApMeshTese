#include "PMJMesh/BSPTreePartitioner.h"

#include "Data/State.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/LoadCalculator.h"

PMJMesh::BSPTreePartitioner::BSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator) :
		PMJMesh::TreePartitioner::TreePartitioner(dimension, tree, load,
			skippingInner, numProcesses, loadCalculator)
{
	//this->setLoadCalculator(loadCalculator);
}

PMJMesh::BSPTreePartitioner::~BSPTreePartitioner()
{

}

/*void PMJMesh::BSPTreePartitioner::setLoadCalculator(LoadCalculator *loadCalculator)
{
	this->loadCalculator = loadCalculator;
}*/

void PMJMesh::BSPTreePartitioner::execute()
{
	this->treeNewCells.clear();

	this->bspLoad.clear();

	this->executePartitioning();

	if (!this->partitionTree)
	{
		return;
	}

#if USE_GUI
    Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::PARTITION, r, g, b);
#endif //#if USE_GUI

	//Data::GraphNodeList leaves = this->partitionTree->getLeaves();

	//while (!leaves.empty())
	for (Data::Tree::LeafIterator iter = this->partitionTree->lBegin(), end = this->partitionTree->lEnd();
         iter != end; iter++)
	{
		//Data::BoxTreeCell *pc = static_cast<Data::BoxTreeCell *>(leaves.back());
		Data::BoxTreeCell *pc = static_cast<Data::BoxTreeCell *>((*iter));

		//leaves.pop_back();

		Real load = this->bspLoad[pc->getId()];

		//debug
		//std::cout << "partition " << c->getId() << " has load " << load << std::endl;
		//endebug

#if USE_GUI
        pc->setColor(r, g, b);
#endif //#if USE_GUI

		if (load > 0.0)
		{
			PMJMesh::Partition *p = new PMJMesh::Partition(pc->box(), load);

			p->setId(pc->getId());

			this->partitions.push_back(p);

			if (load > this->maxCalculatedLoad)
			{
				this->maxCalculatedLoad = load;
			}
		}
		/*else
		{
			pc->setId(0);
		}*/
	}
}

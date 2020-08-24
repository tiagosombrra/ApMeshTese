#include "PMJMesh/TreePartitioner.h"

#include "Data/State.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/PartitionQuadTreeCell.h"
#include "PMJMesh/PartitionOctTreeCell.h"
#include "PMJMesh/Partition.h"

using namespace PMJMesh;

PMJMesh::TreePartitioner::TreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator) :
	Parallel::Partitioner()
{
	this->setDimension(dimension);
	this->setTree(tree);
	this->setLoad(load);
	this->setNumProcesses(numProcesses);
	this->setLoadCalculator(loadCalculator);
	this->skippingInner = skippingInner;
	this->partitionTree = NULL;
	this->maxCalculatedLoad = 0.0;
}

PMJMesh::TreePartitioner::~TreePartitioner()
{
	/*this->tree = NULL;
	this->partitionTree = NULL;
	this->load = NULL;*/
}

void PMJMesh::TreePartitioner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

#if USE_C__11
void PMJMesh::TreePartitioner::setLoad(const Data::RealHashMap *load)
#else
void PMJMesh::TreePartitioner::setLoad(const Data::RealMap *load)
#endif //#if USE_C__11
{
	this->load = load;
}

void PMJMesh::TreePartitioner::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

void PMJMesh::TreePartitioner::setNumProcesses(UInt numProcesses)
{
	this->numProcesses = numProcesses;
}

void PMJMesh::TreePartitioner::setLoadCalculator(LoadCalculator *loadCalculator)
{
	this->loadCalculator = loadCalculator;
}

void PMJMesh::TreePartitioner::setSkippingInner(bool skippingInner)
{
	this->skippingInner = skippingInner;
}

Data::BoxTree *PMJMesh::TreePartitioner::getPartitionTree() const
{
	return this->partitionTree;
}

const Data::ULIntVector &PMJMesh::TreePartitioner::getTreeNewCells() const
{
	return this->treeNewCells;
}

Real PMJMesh::TreePartitioner::getMaxCalculatedLoad() const
{
	return this->maxCalculatedLoad;
}

bool PMJMesh::TreePartitioner::isSkipped(Data::BoxTreeCell *partitionCell) const
{
	return ((this->skippingInner) && (this->skipped.find(partitionCell) != this->skipped.end()));
}

void PMJMesh::TreePartitioner::skip(Data::BoxTreeCell *partitionCell)
{
	if (this->skippingInner)
	{
		this->skipped.insert(partitionCell);
	}
}

void PMJMesh::TreePartitioner::execute()
{
	this->treeNewCells.clear();
	this->skipped.clear();

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

		const Data::BoxTreeCell *c = (this->dimension == 2) ?
			static_cast<PMJMesh::PartitionQuadTreeCell *>(pc)->getOriginal() :
			static_cast<PMJMesh::PartitionOctTreeCell *>(pc)->getOriginal();

		//leaves.pop_back();

		Real load = (*this->load->find(c->getId())).second;

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

	/*//creating secondary partitions
	///TODO load of secondary partitions
	///TODO define partitions interception

	SecondaryPartitionBuilder *spb = new SecondaryPartitionBuilder(
		this->dimension, this->partitionTree, false, false, false);

	spb->execute();

	Data::ShapeList boxes = spb->getSecondaryPartitions();

	delete spb;

	while (!boxes.empty())
	{
		Data::Box *b = (Data::Box *)boxes.back();
		boxes.pop_back();

		Real load = 0.0;//find load of box

		PMJMesh::Partition *p = new PMJMesh::Partition(*b, load);

		p->setId(++this->idPartition);

		this->secondaryPartitions.push_back(p);

		delete b;
	}*/
}

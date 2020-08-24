#include "PMJMesh/MaxLoadTreePartitioner.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/PartitionQuadTree.h"
#include "PMJMesh/PartitionQuadTreeCell.h"
#include "PMJMesh/PartitionOctTree.h"
#include "PMJMesh/PartitionOctTreeCell.h"
//#include "PMJMesh/PartitionTreeGenerator.h"
#include "PMJMesh/PartitionNeighborTreeRefiner.h"

using namespace PMJMesh;

PMJMesh::MaxLoadTreePartitioner::MaxLoadTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
	bool usePartitioningThreshold, Real maxAllowedLoad/*, bool refinement21*/) :
		PMJMesh::TreePartitioner(dimension, tree, load, skippingInner, numProcesses, loadCalculator)
{
	this->setUsePartitioningThreshold(usePartitioningThreshold);
	this->setMaxAllowedLoad(maxAllowedLoad);
	//this->setRefinement21(refinement21);
}

PMJMesh::MaxLoadTreePartitioner::~MaxLoadTreePartitioner()
{

}

void PMJMesh::MaxLoadTreePartitioner::setUsePartitioningThreshold(bool usePartitioningThreshold)
{
	this->usePartitioningThreshold = usePartitioningThreshold;
}

void PMJMesh::MaxLoadTreePartitioner::setMaxAllowedLoad(Real maxAllowedLoad)
{
	this->maxAllowedLoad = maxAllowedLoad;
}

/*void PMJMesh::MaxLoadTreePartitioner::setRefinement21(bool refinement21)
{
	this->refinement21 = refinement21;
}*/

void PMJMesh::MaxLoadTreePartitioner::executePartitioning()
{
	if (!this->usePartitioningThreshold)
	{
		this->maxAllowedLoad = this->findMaxLoad();

		//std::cout << "usethreshold = " << this->usePartitioningThreshold<< ", numprocesses = " << this->numProcesses << ", maxload = " << this->maxLoad << std::endl;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	std::cout << "total load = " << (*this->load->find(this->tree->getRoot()->getId())).second << std::endl;
	std::cout << "max allowed load = " << this->maxAllowedLoad << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	/*PartitionTreeGenerator *treeGenerator = new PartitionTreeGenerator(
		this->dimension, this->tree, this->load, this->maxAllowedLoad,
		this->loadCalculator, &this->treeNewCells);

	treeGenerator->execute();

	this->partitionTree = static_cast<Data::BoxTree *>(treeGenerator->getTree());

	delete treeGenerator;*/

	this->buildTree();

	/*if (this->refinement21)
	{
		//UInt size = static_cast<UInt>(this->treeNewCells.size());

		PartitionNeighborTreeRefiner *refiner = new PartitionNeighborTreeRefiner(
			this->dimension, this->partitionTree, this->tree, &this->treeNewCells);

		refiner->execute();

		delete refiner;
	}*/
}

bool PMJMesh::MaxLoadTreePartitioner::shouldSkip(const Data::BoxTreeCell *partitionCell) const
{
	if (!this->skippingInner)
	{
		return false;
	}

	Data::BoxTreeCell *original = (this->dimension == 2) ?
		static_cast<const PMJMesh::PartitionQuadTreeCell *>(partitionCell)->getOriginal() :
		static_cast<const PMJMesh::PartitionOctTreeCell *>(partitionCell)->getOriginal();

	Data::Position pos = PMJMesh::getPosition(this->dimension, original);

	if (pos == Data::IN)
	{
		return true;
	}

	if (original->isLeaf())
	{
		return true;
	}

	for (UInt i = 0; i < original->numChildren(); i++)
	{
		Data::Position cpos = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(original->child(i)));

		if (cpos == Data::IN)
		{
			return true;
		}
	}

	return false;
}

bool PMJMesh::MaxLoadTreePartitioner::shouldSubdivide(Real load) const
{
	return (load > this->maxAllowedLoad);
}

bool PMJMesh::MaxLoadTreePartitioner::shouldSubdivide(const Data::BoxTreeCell *original) const
{
	/*Real load = (*this->load->find(original->getId())).second;

	if (this->shouldSubdivide(load))
	{
		return true;
	}

	return false;*/

	return this->shouldSubdivide((*this->load->find(original->getId())).second);

	/*//if a child has the same load as the original, meaning that the partition
	// will be created for the cell with maximum depth, when loads are equal

	UInt noLoadChildren = 0;
	UInt numChildren = original->numChildren();

	for (UInt i = 0; i < numChildren; i++)
	{
		Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(original->child(i));

		Real cload = (*this->load->find(child->getId())).second;

		if (std::fabs(cload) < Data::getTolerance())
		{
			noLoadChildren++;
		}
	}

	return (noLoadChildren == numChildren - 1);*/
}

void PMJMesh::MaxLoadTreePartitioner::subdivide(Data::BoxTreeCell *c, const Data::BoxTreeCell *original)
{
	if (this->shouldSkip(c))
	{
		this->skip(c);

		return;
	}

	if (!(this->shouldSubdivide(original)))
	{
		return;
	}

	if (!c->isSubdivided())
	{
		//this will only happen if the load tree minimum refinement has been
		// postponed
		if (!original->isSubdivided())
		{
			const_cast<Data::BoxTreeCell *>(original)->subdivide();

			Real load = (*this->load->find(original->getId())).second/static_cast<Real>(original->numChildren());

			/*if (this->loadCalculator->isAddingLoadForNodes())
			{
				load -= this->loadCalculator->loadFor(PMJMesh::getPosition(this->dimension, original));
			}*/

			for (UInt i = 0; i < original->numChildren(); i++)
			{
				ULInt id = original->child(i)->getId();

				//if (this->treeNewCells)
				{
					this->treeNewCells.push_back(id);
				}

#if USE_C__11
				const_cast<Data::RealHashMap *>(this->load)->insert(std::make_pair(id, load));
#else
				const_cast<Data::RealMap *>(this->load)->insert(std::make_pair(id, load));
#endif //#if USE_C__11
			}
		}

		c->subdivide();
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->subdivide(static_cast<Data::BoxTreeCell *>(c->child(i)), static_cast<Data::BoxTreeCell *>(original->child(i)));
	}
}

void PMJMesh::MaxLoadTreePartitioner::buildTree()
{
	Data::BoxTreeCell *originalRoot = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	Data::Box *box = originalRoot->box();

	if (this->dimension == 2)
	{
		Data::Point2D min(box->getMin());
		Data::Point2D max(box->getMax());

		this->partitionTree = new PMJMesh::PartitionQuadTree(min, max);
	}
	else if (this->dimension == 3)
	{
		Data::Point3D min(box->getMin());
		Data::Point3D max(box->getMax());

		this->partitionTree = new PMJMesh::PartitionOctTree(min, max);
	}

	Data::BoxTreeCell *root = static_cast<Data::BoxTreeCell *>(this->partitionTree->getRoot());

	if (dimension == 2)
	{
		static_cast<PMJMesh::PartitionQuadTreeCell *>(root)->setOriginal(originalRoot);
	}
	else
	{
		static_cast<PMJMesh::PartitionOctTreeCell *>(root)->setOriginal(originalRoot);
	}

	this->subdivide(root, originalRoot);
}

#include "PMJMesh/PartitionTreeGenerator.h"

/*#include "PMJMesh/PartitionQuadTree.h"
#include "PMJMesh/PartitionQuadTreeCell.h"
#include "PMJMesh/PartitionOctTree.h"
#include "PMJMesh/PartitionOctTreeCell.h"
#include "PMJMesh/LoadCalculator.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/BoxTreeCell.h"
#include "Data/BoxTree.h"
#include "Data/Box.h"

PMJMesh::PartitionTreeGenerator::PartitionTreeGenerator(UInt dimension, const Data::BoxTree *original,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	Real maxLoad, LoadCalculator *loadCalculator, Data::ULIntVector *originalNewCells) :
	Performer::TreeGenerator()
{
	this->setDimension(dimension);

	this->setOriginal(original);

	this->setLoad(load);

	this->setMaxLoad(maxLoad);

	this->loadCalculator = loadCalculator;

	this->originalNewCells = originalNewCells;
}

PMJMesh::PartitionTreeGenerator::~PartitionTreeGenerator()
{

}

void PMJMesh::PartitionTreeGenerator::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::PartitionTreeGenerator::setOriginal(const Data::BoxTree *original)
{
	this->original = original;
}

#if USE_C__11
void PMJMesh::PartitionTreeGenerator::setLoad(const Data::RealHashMap *load)
#else
void PMJMesh::PartitionTreeGenerator::setLoad(const Data::RealMap *load)
#endif //#if USE_C__11
{
	this->load = load;
}

void PMJMesh::PartitionTreeGenerator::setMaxLoad(Real maxLoad)
{
	this->maxLoad = maxLoad;
}

bool PMJMesh::PartitionTreeGenerator::shouldSubdivide(Real load) const
{
	return (load > this->maxLoad);
}

bool PMJMesh::PartitionTreeGenerator::shouldSubdivide(const Data::BoxTreeCell *original) const
{
	Real load = (*this->load->find(original->getId())).second;

	if (this->shouldSubdivide(load))
	{
		return true;
	}

	return false;

	*//*//if a child has the same load as the original, meaning that the partition
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

	return (noLoadChildren == numChildren - 1);*//*
}

void PMJMesh::PartitionTreeGenerator::subdivide(Data::BoxTreeCell *c, const Data::BoxTreeCell *original)
{
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

			*//*if (this->loadCalculator->isAddingLoadForNodes())
			{
				load -= this->loadCalculator->loadFor(PMJMesh::getPosition(this->dimension, original));
			}*//*

			for (UInt i = 0; i < original->numChildren(); i++)
			{
				ULInt id = original->child(i)->getId();

				if (this->originalNewCells)
				{
					this->originalNewCells->push_back(id);
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

void PMJMesh::PartitionTreeGenerator::execute()
{
	if (!this->original)
	{
		return;
	}

	if ((this->dimension != 2) && (this->dimension != 3))
	{
		return;
	}

	Data::BoxTreeCell *originalRoot = static_cast<Data::BoxTreeCell *>(this->original->getRoot());

	Data::Box *box = originalRoot->box();

	if (this->dimension == 2)
	{
		Data::Point2D min(box->getMin());
		Data::Point2D max(box->getMax());

		this->graph = new PMJMesh::PartitionQuadTree(min, max);
	}
	else if (this->dimension == 3)
	{
		Data::Point3D min(box->getMin());
		Data::Point3D max(box->getMax());

		this->graph = new PMJMesh::PartitionOctTree(min, max);
	}

	Data::BoxTreeCell *root = static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot());

	if (dimension == 2)
	{
		static_cast<PMJMesh::PartitionQuadTreeCell *>(root)->setOriginal(originalRoot);
	}
	else
	{
		static_cast<PMJMesh::PartitionOctTreeCell *>(root)->setOriginal(originalRoot);
	}

	this->subdivide(root, originalRoot);
}*/

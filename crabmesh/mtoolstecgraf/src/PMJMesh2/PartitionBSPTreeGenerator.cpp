#include "PMJMesh2/PartitionBSPTreeGenerator.h"

#include "PMJMesh2/PartitionBSPTree.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "PMJMesh2/Communicator.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh/Partition.h"
#include "Data/Box.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

using namespace PMJMesh2;

PMJMesh2::PartitionBSPTreeGenerator::PartitionBSPTreeGenerator() :
    Performer::TreeGenerator(),
    Parallel::Builder()
{
    this->dimension = 0;
    this->numTasksPerThread = 1;
    this->loadCalculator = NULL;
    this->boxTree = NULL;
    this->removeLayersInLoad = 0;
}

PMJMesh2::PartitionBSPTreeGenerator::~PartitionBSPTreeGenerator()
{

}

void PMJMesh2::PartitionBSPTreeGenerator::setDimension(UInt dimension)
{
    this->dimension = dimension;
}

void PMJMesh2::PartitionBSPTreeGenerator::setNumTasksPerThread(UInt numTasksPerThread)
{
    this->numTasksPerThread = (numTasksPerThread == 0) ? 1 : numTasksPerThread;
}

void PMJMesh2::PartitionBSPTreeGenerator::setBoxTree(const Data::BoxTree *boxTree)
{
    this->boxTree = boxTree;
}

void PMJMesh2::PartitionBSPTreeGenerator::setRemoveLayersInLoad(UInt removeLayersInLoad)
{
    this->removeLayersInLoad = removeLayersInLoad;

#if USE_REMOVED_LAYERS_PROPORTIONS
    this->removedLayersProportions.resize(this->removeLayersInLoad, 1.0);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
}

#if USE_REMOVED_LAYERS_PROPORTIONS
void PMJMesh2::PartitionBSPTreeGenerator::setRemovedLayersProportions(UInt i, Real proportion)
{
    this->removedLayersProportions.resize(std::max(i, this->removeLayersInLoad), 1.0);

    this->removedLayersProportions[i] = proportion;
}

void PMJMesh2::PartitionBSPTreeGenerator::setRemovedLayersProportions(const Data::RealVector &removedLayersProportions)
{
    this->removedLayersProportions = removedLayersProportions;

    this->removedLayersProportions.resize(std::max(static_cast<UInt>(this->removedLayersProportions.size()), this->removeLayersInLoad), 1.0);
}
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

void PMJMesh2::PartitionBSPTreeGenerator::setLoadCalculator(PMJMesh::LoadCalculator *loadCalculator)
{
    this->loadCalculator = loadCalculator;
}

PartitionBSPTree *PMJMesh2::PartitionBSPTreeGenerator::getBsp() const
{
    return static_cast<PartitionBSPTree *>(this->getTree());
}

const Parallel::PartitionVector &PMJMesh2::PartitionBSPTreeGenerator::getPartitions() const
{
    return this->partitions;
}

UInt PMJMesh2::PartitionBSPTreeGenerator::heavier(PartitionBSPTreeCell *cell1, PartitionBSPTreeCell *cell2) const
{
    Real load1 = this->load(cell1);
    Real load2 = this->load(cell2);

    return (load1 >= load2) ? 0 : 1;
}

UInt PMJMesh2::PartitionBSPTreeGenerator::heavierChild(PartitionBSPTreeCell *cell) const
{
    return this->heavier(static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0)), static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1)));
}

/*void PMJMesh2::PartitionBSPTreeGenerator::setTree(Data::TreeNode *n, Data::Tree *tree)
{
    n->setTree(tree);

    if (n->isLeaf())
    {
        return;
    }

    for (UInt i = 0; i < n->numChildren(); i++)
    {
        this->setTree(n->child(i), tree);
    }

#if (!USE_EXPERIMENTAL_TREE)
    tree->addChildren(n);
#endif //#if (!USE_EXPERIMENTAL_TREE)
}*/

void PMJMesh2::PartitionBSPTreeGenerator::subdivide(PartitionBSPTreeCell *cell, const Split &split) const
{
    cell->setSplit(split.direction, split.coord);

    cell->subdivide();
}

Int PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision(PartitionBSPTreeCell *cell)
{
    if (cell->isLeaf())
    {
        Split spl = this->findBestSubdivision(cell);

        if (spl.direction >= this->dimension)
        {
            return -1;
        }

        this->subdivide(cell, spl);
    }

    return this->heavierChild(cell);
}

PMJMesh2::PartitionBSPTreeCell *PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size)
{
    //create the box for the cell
    cell->box();

    if (size <= 1)
    {
        return cell;
    }

    Int disp = this->executeSubdivision(cell);

    if (disp < 0)
    {
        return cell;
    }

    disp = (((size%2) > 0) && (disp == 0)) ? 1 : 0;

    Int sizeLeast = size/2 + disp;

    if (cell->getMaxDistLimit() > cell->getMinDistLimit() + 1)
    {
        static_cast<PartitionBSPTreeCell *>(cell->child(0))->setDistLimits(cell->getMinDistLimit(), cell->getMinDistLimit() + sizeLeast);
        static_cast<PartitionBSPTreeCell *>(cell->child(1))->setDistLimits(cell->getMinDistLimit() + sizeLeast, cell->getMaxDistLimit());
    }

    if (cell->getMaxSharLimit() > cell->getMinSharLimit() + 1)
    {
        static_cast<PartitionBSPTreeCell *>(cell->child(0))->setSharLimits(cell->getMinSharLimit(), cell->getMinSharLimit() + sizeLeast);
        static_cast<PartitionBSPTreeCell *>(cell->child(1))->setSharLimits(cell->getMinSharLimit() + sizeLeast, cell->getMaxSharLimit());
    }

    if (rank < sizeLeast)
    {
        Int peer = this->comm->rank() + sizeLeast;

        if (peer < this->comm->numProcesses())
        {
            cell->setPeer(peer);
        }

        static_cast<PartitionBSPTreeCell *>(cell->child(1))->box();

        return this->executeSubdivision(static_cast<PartitionBSPTreeCell *>(cell->child(0)), rank, sizeLeast);
    }

    Int peer = this->comm->rank() - sizeLeast;

    if (peer >= 0)
    {
        cell->setPeer(peer);
    }

    static_cast<PartitionBSPTreeCell *>(cell->child(0))->box();

    return this->executeSubdivision(static_cast<PartitionBSPTreeCell *>(cell->child(1)), rank - sizeLeast, size - sizeLeast);
}

bool PMJMesh2::PartitionBSPTreeGenerator::less(const Data::BoxTreeCell *cell, UInt dir, Real coord) const
{
    return (0.5*(cell->box()->getMin()->getCoord(dir) + cell->box()->getMax()->getCoord(dir)) < coord);
}

bool PMJMesh2::PartitionBSPTreeGenerator::isOutCell(const Data::BoxTreeCell *c) const
{
    Data::Position pos = PMJMesh::getPosition(this->dimension, c);

    return ((pos == Data::OUT) || (pos == Data::UNKNOWN));
}

bool PMJMesh2::PartitionBSPTreeGenerator::split(const Data::GraphNodeList &cells, UInt dir, Real coord,
    Data::GraphNodeList &cells1, Data::GraphNodeList &cells2, bool skipOut) const
{
    if (cells.empty())
    {
        return false;
    }

    for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        if ((skipOut) && (this->isOutCell(c)))
        {
            continue;
        }

        if ((c->box()->getMin()->getCoord(dir) < coord - Data::getTolerance()) &&
            (c->box()->getMax()->getCoord(dir) > coord + Data::getTolerance()))
        {
            if (c->isLeaf())
            {
                return false;
            }

            for (UInt i = 0; i < c->numChildren(); i++)
            {
                Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(i));

                if ((skipOut) && (this->isOutCell(child)))
                {
                    continue;
                }

                if (this->less(child, dir, coord))
                {
                    cells1.push_back(child);
                }
                else
                {
                    cells2.push_back(child);
                }
            }

            continue;
        }

        if (this->less(c, dir, coord))
        {
            cells1.push_back(c);
        }
        else
        {
            cells2.push_back(c);
        }
    }

    return true;
}

bool PMJMesh2::PartitionBSPTreeGenerator::splitRec(const Data::GraphNodeList &cells, UInt dir, Real coord,
    Data::GraphNodeList &cells1, Data::GraphNodeList &cells2, bool skipOut) const
{
    if (cells.empty())
    {
        return false;
    }

    bool splitted = false;

    for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        if ((skipOut) && (this->isOutCell(c)))
        {
            continue;
        }

        if (c->box()->getMax()->getCoord(dir) <= coord + Data::getTolerance())
        {
            cells1.push_back(c);

            splitted = true;

            continue;
        }

        if (c->box()->getMin()->getCoord(dir) >= coord - Data::getTolerance())
        {
            cells2.push_back(c);

            splitted = true;

            continue;
        }

        if (c->isLeaf())
        {
            if (this->less(c, dir, coord))
            {
                cells1.push_back(c);
            }
            else
            {
                cells2.push_back(c);
            }

            splitted = true;

            continue;
        }

        for (UInt i = 0; i < c->numChildren(); i++)
        {
            Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(i));

            Data::GraphNodeList ch;

            ch.push_back(child);

            bool spl = this->splitRec(ch, dir, coord, cells1, cells2, skipOut);

            splitted = splitted || spl;
        }
    }

    return splitted;
}

/*void PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision(PartitionBSPTreeCell *&cell, Int &rank, Int &size)
{
    Int d = this->executeSubdivision(cell);

    if (d < 0)
    {
        return;
    }

    d = (((size%2) > 0) && (d == 0)) ? 1 : 0;

    Int v = size/2 + d;

    if (rank < v)
    {
        if (rank + v < size)
        {
            cell->setPeer(this->comm->rank() + v);
        }

        cell = static_cast<PartitionBSPTreeCell *>(cell->child(0));
        size = v;
    }
    else
    {
        if (rank - v >= 0)
        {
            cell->setPeer(this->comm->rank() - v);
        }

        cell = static_cast<PartitionBSPTreeCell *>(cell->child(1));

        size -= v;
        rank -= v;
    }
}*/

Parallel::Partition *PMJMesh2::PartitionBSPTreeGenerator::makePartition(const PartitionBSPTreeCell *cell) const
{
    //Parallel::Partition *p = new PMJMesh::Partition(cell->box(), this->load(cell));
    Parallel::Partition *p = new PMJMesh::Partition(cell->box(), cell->getLoad());

    p->setId(cell->getId());

    /*//debug
#if USE_GUI
    {
        static_cast<PMJMesh::Partition *>(p)->cells = cell->cells;
    }
#endif //#if USE_GUI
    //endebug*/

    return p;
}

void PMJMesh2::PartitionBSPTreeGenerator::execute()
{
    if ((!this->boxTree) || (!this->comm) || (!this->loadCalculator))
    {
        return;
    }

    if ((this->dimension != 2) && (this->dimension != 3))
    {
        return;
    }

    Data::Point *min = static_cast<Data::Box *>(this->boxTree->getShape())->getMin();
	Data::Point *max = static_cast<Data::Box *>(this->boxTree->getShape())->getMax();

	this->graph = new PartitionBSPTree(*min, *max);

	PartitionBSPTreeCell *cell = static_cast<PartitionBSPTreeCell *>(this->getBsp()->getRoot());

	Data::GraphNodeList cells(1, cell);

	this->partitions.assign(static_cast<PMJMesh2::Communicator *>(this->comm)->getMaxThreads()*this->numTasksPerThread, NULL);

    Int numProcesses = this->comm->numProcesses();
    Int numThreads = static_cast<PMJMesh2::Communicator *>(this->comm)->getMaxThreads();

    cell->setDistLimits(0, numProcesses);

    cell = this->executeSubdivision(cell, this->comm->rank(), numProcesses);

    if (numThreads > 1)
    {
        cell->setSharLimits(0, numThreads);
    }

    for (Int id = 0; id < numThreads; id++)
    {
        PartitionBSPTreeCell *tcell = this->executeSubdivision(cell, id, numThreads);

        /*if (this->numTasksPerThread > 1)
        {
            tcell->setLimits(0, this->numTasksPerThread);
        }*/

        for (UInt i = 0; i < this->numTasksPerThread; i++)
        {
            PartitionBSPTreeCell *leaf = this->executeSubdivision(tcell, static_cast<Int>(i), this->numTasksPerThread);

            leaf->setLoad(this->load(leaf));

            this->partitions[id*this->numTasksPerThread + i] = this->makePartition(leaf);
        }
    }

    while (!cells.empty())
    {
        PartitionBSPTreeCell *cell = static_cast<PartitionBSPTreeCell *>(cells.front());

        cells.pop_front();

        if (cell->isLeaf())
        {
            continue;
        }

        cell->setLoad(this->innerLoad(cell));

        for (UInt i = 0; i < cell->numChildren(); i++)
        {
            cells.push_back(cell->child(i));
        }
    }

/*#if USE_GUI
    this->adjustBox();
#endif //#if USE_GUI*/
}

void PMJMesh2::PartitionBSPTreeGenerator::adjustBox() const
{
#if USE_EXPERIMENTAL_3
	Data::Point2D min2d, max2d;
	Data::Point3D min3d, max3d;

	Data::Point &min = (this->dimension == 2) ?
        static_cast<Data::Point &>(min2d) : static_cast<Data::Point &>(min3d);
    Data::Point &max = (this->dimension == 2) ?
        static_cast<Data::Point &>(max2d) : static_cast<Data::Point &>(max3d);
#else
	Data::Point min(this->dimension), max(this->dimension);
#endif //#if USE_EXPERIMENTAL_3

	for (UInt i = 0; i < this->dimension; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

	this->findBox(min, max);

	if (this->setBox(static_cast<Data::BSPTreeCell *>(this->getBsp()->getRoot()), min, max))
	{
		Data::Box *box = static_cast<Data::Box *>(static_cast<Data::BSPTree *>(this->getBsp())->getShape());

		for (UInt i = 0; i < this->dimension; i++)
		{
			if (box->getMin()->getCoord(i) < min.getCoord(i))
			{
				box->getMin()->setCoord(i, min.getCoord(i));
			}

			if (box->getMax()->getCoord(i) > max.getCoord(i))
			{
				box->getMax()->setCoord(i, max.getCoord(i));
			}
		}
	}
}

void PMJMesh2::PartitionBSPTreeGenerator::findBox(Data::Point &min, Data::Point &max) const
{
	this->findBox(static_cast<Data::BoxTreeCell *>(this->boxTree->getRoot()), min, max);
}

void PMJMesh2::PartitionBSPTreeGenerator::findBox(const Data::BoxTreeCell *c, Data::Point &min, Data::Point &max) const
{
	Data::Position pos = PMJMesh::getPosition(this->dimension, c);

	if ((pos == Data::UNKNOWN) || (pos == Data::OUT))
	{
		return;
	}

	if ((pos == Data::IN) || (c->isLeaf()))
	{
		Data::Box *b = c->box();

		for (UInt i = 0; i < this->dimension; i++)
		{
			if (b->getMin()->getCoord(i) < min.getCoord(i))
			{
				min.setCoord(i, b->getMin()->getCoord(i));
			}

			if (b->getMax()->getCoord(i) > max.getCoord(i))
			{
				max.setCoord(i, b->getMax()->getCoord(i));
			}
		}

		return;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->findBox(static_cast<Data::BoxTreeCell *>(c->child(i)), min, max);
	}
}

bool PMJMesh2::PartitionBSPTreeGenerator::setBox(Data::BSPTreeCell *c, const Data::Point &min, const Data::Point &max) const
{
	if ((c->getSplitCoord() <= min.getCoord(c->getSplitDirection())) ||
		(c->getSplitCoord() >= max.getCoord(c->getSplitDirection())))
	{
		return false;
	}

	bool changed = false;

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (c->box()->getMin()->getCoord(i) < min.getCoord(i))
		{
			c->box()->getMin()->setCoord(i, min.getCoord(i));

			changed = true;
		}

		if (c->box()->getMax()->getCoord(i) > max.getCoord(i))
		{
			c->box()->getMax()->setCoord(i, max.getCoord(i));

			changed = true;
		}
	}

	if (!changed)
	{
		return false;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->setBox(static_cast<Data::BSPTreeCell *>(c->child(i)), min, max);
	}

	return true;
}

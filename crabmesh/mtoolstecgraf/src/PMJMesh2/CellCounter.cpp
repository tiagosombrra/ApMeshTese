#include "PMJMesh2/CellCounter.h"

#if DEBUG_CELL_COUNTER

#include "PMJMesh2/PartitionBSPTree.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "Parallel/Task.h"
#include "Parallel/Partition.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/OStream.h"

using namespace PMJMesh2;

PMJMesh2::CellCounter::MinTreeRefiner::MinTreeRefiner(UInt dimension, UInt minLevel, Data::BoxTree *tree,
    const MJMesh::Boundary *boundary) :
        PMJMesh::MinTreeRefiner::MinTreeRefiner(dimension, tree, boundary, minLevel, true, NULL)
{
    //this->setMinLevel(minLevel);
}

PMJMesh2::CellCounter::MinTreeRefiner::~MinTreeRefiner()
{

}

UInt PMJMesh2::CellCounter::MinTreeRefiner::findMinLevel() const
{
    return this->getMinLevel();
}

PMJMesh2::CellCounter::CellCounter() :
    Performer::Builder::Builder()
{
    this->dimension = 0;
    this->bsp = NULL;
    this->tree = NULL;
    this->boundary = NULL;
    this->numLayers = 0;
    this->elementsMap = NULL;
    //this->buildHistogram = false;
    this->buildHistogram = true;
    this->excludeLayersInHistogram = true;
    this->onlyINCellsInHistogram = false;

    this->minLevel = 0;
    this->maxLevel = 0;
}

PMJMesh2::CellCounter::~CellCounter()
{

}

void PMJMesh2::CellCounter::setDimension(UInt dimension)
{
    this->dimension = dimension;
}

void PMJMesh2::CellCounter::setBSP(const PartitionBSPTree *bsp)
{
    this->bsp = bsp;
}

void PMJMesh2::CellCounter::setTree(Data::BoxTree *tree)
{
    this->tree = tree;
}

void PMJMesh2::CellCounter::setBoundary(const MJMesh::Boundary *boundary)
{
    this->boundary = boundary;
}

void PMJMesh2::CellCounter::setNumLayers(UInt numLayers)
{
    this->numLayers = numLayers;
}

void PMJMesh2::CellCounter::setTasks(const Parallel::TaskVector &tasks)
{
    this->tasks = tasks;
}

#if USE_C__11
void PMJMesh2::CellCounter::setElementsMap(PMJMesh::FEListHashMap *elementsMap)
#else
void PMJMesh2::CellCounter::setElementsMap(PMJMesh::FEListMap *elementsMap)
#endif //#if USE_C__11
{
    this->elementsMap = elementsMap;
}

void PMJMesh2::CellCounter::setBuildHistogram(bool buildHistogram)
{
    this->buildHistogram = buildHistogram;
}

void PMJMesh2::CellCounter::execute()
{
    if ((this->dimension < 2) || (this->dimension > 3) ||
        (!this->tree) || (!this->bsp) || (!this->boundary) ||
        (!this->elementsMap) || (this->tasks.empty()))
    {
        return;
    }

    Data::cout() << "Will find levels" << Data::endl;
    this->findLevels();
    Data::cout() << "Found levels " << this->minLevel << " and " << this->maxLevel << Data::endl;
    Data::cout() << "Will find cells" << Data::endl;
    this->findCells();
    Data::cout() << "Will count cells in normal levels" << Data::endl;
    this->countNormal();
    if (!this->buildHistogram)
    {
        Data::cout() << "Will refine to level " << this->maxLevel << Data::endl;
        this->refine();
        Data::cout() << "Will count cells in min and max levels" << Data::endl;
        this->count();
    }

    ULInt maxHistogramLevel = 0;

    if (this->buildHistogram)
    {
        for (CellMap::iterator iter = this->map.begin();
             iter != this->map.end(); iter++)
        {
            Data::BSPTreeCell *cell = (*iter).second.cell;

            if (!cell->isLeaf())
            {
                continue;
            }

            ULInt maxLevel = (*(--(*iter).second.histogram.end())).first;

            if (maxLevel > maxHistogramLevel)
            {
                maxHistogramLevel = maxLevel;
            }
        }
    }

    for (CellMap::iterator iter = this->map.begin();
         iter != this->map.end(); iter++)
    {
        Data::BSPTreeCell *cell = (*iter).second.cell;

        if (!cell->isLeaf())
        {
            continue;
        }

        Parallel::Task *task = NULL;

        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            if (cell->getId() == this->tasks[i]->getId())
            {
                task = this->tasks[i];

                break;
            }
        }

        if (!task)
        {
            continue;
        }

        Data::cout().getBuffer().precision(6);
        Data::cout().getBuffer().setf(std::ios::fixed, std::ios::floatfield);

        Data::cout() << "Subdomain = " << cell->getId() << "\n";
        Data::cout() << "Load = " << static_cast<PartitionBSPTreeCell *>(cell)->getLoad() << "\n";

        Data::cout() << "cells IN = " << (*iter).second.count.normalIN << "\n";
        Data::cout() << "cells ON = " << (*iter).second.count.normalON << "\n";
        Data::cout() << "cells = " << (*iter).second.count.normalIN + (*iter).second.count.normalON << "\n";

        for (UInt i = 0; i < numLayers; i++)
        {
            Data::cout() << "cells IN in " << i + 1 << " layers = " << (*iter).second.count.normalINLayer[i] << "\n";
            Data::cout() << "cells IN except in " << i + 1 << " layers = " << (*iter).second.count.normalIN - (*iter).second.count.normalINLayer[i] << "\n";
            Data::cout() << "cells except in " << i + 1 << " layers = " << (*iter).second.count.normalIN + (*iter).second.count.normalON - (*iter).second.count.normalINLayer[i] << "\n";
        }

        if (!this->buildHistogram)
        {
            Data::cout() << "minLevel = " << this->minLevel << "\n";
            Data::cout() << "minLevel, cells IN = " << (*iter).second.count.minIN << "\n";
            Data::cout() << "minLevel, cells ON = " << (*iter).second.count.minON << "\n";
            Data::cout() << "minLevel, cells = " << (*iter).second.count.minIN + (*iter).second.count.minON << "\n";

            for (UInt i = 0; i < numLayers; i++)
            {
                Data::cout() << "minLevel, cells IN in " << i + 1 << " layers = " << (*iter).second.count.minINLayer[i] << "\n";
                Data::cout() << "minLevel, cells IN except in " << i + 1 << " layers = " << (*iter).second.count.minIN - (*iter).second.count.minINLayer[i] << "\n";
                Data::cout() << "minLevel, cells except in " << i + 1 << " layers = " << (*iter).second.count.minIN + (*iter).second.count.minON - (*iter).second.count.minINLayer[i] << "\n";
            }

            Data::cout() << "maxLevel = " << this->maxLevel << "\n";
            Data::cout() << "maxLevel, cells IN = " << (*iter).second.count.maxIN << "\n";
            Data::cout() << "maxLevel, cells ON = " << (*iter).second.count.maxON << "\n";
            Data::cout() << "maxLevel, cells = " << (*iter).second.count.maxIN + (*iter).second.count.maxON << "\n";

            for (UInt i = 0; i < numLayers; i++)
            {
                Data::cout() << "maxLevel, cells IN in " << i + 1 << " layers = " << (*iter).second.count.maxINLayer[i] << "\n";
                Data::cout() << "maxLevel, cells IN except in " << i + 1 << " layers = " << (*iter).second.count.maxIN - (*iter).second.count.maxINLayer[i] << "\n";
                Data::cout() << "maxLevel, cells except in " << i + 1 << " layers = " << (*iter).second.count.maxIN + (*iter).second.count.maxON - (*iter).second.count.maxINLayer[i] << "\n";
            }
        }

        if (this->buildHistogram)
        {
            Data::cout() << "Histogram\n";

            ULInt minLevel = (*(*iter).second.histogram.begin()).first;
            ULInt maxLevel = (*(--(*iter).second.histogram.end())).first;

            for (ULInt i = 0; i < minLevel; i++)
            {
                Data::cout() << "level[" << i << "] = " << 0 << "\n";
            }

            for (Data::ULIntMap::iterator iter2 = (*iter).second.histogram.begin();
                 iter2 != (*iter).second.histogram.end(); iter2++)
            {
                Data::cout() << "level[" << (*iter2).first << "] = " << (*iter2).second << "\n";
            }

            for (ULInt i = maxLevel + 1; i <= maxHistogramLevel; i++)
            {
                Data::cout() << "level[" << i << "] = " << 0 << "\n";
            }
        }

        Data::cout() << Data::endl;
    }
}

void PMJMesh2::CellCounter::findLevels()
{
    this->minLevel = UINT_MAX;
    this->maxLevel = 0;

    this->findLevels(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));

    if (this->maxLevel != this->minLevel)
    {
        UInt allowed = 11;

        if ((!this->buildHistogram) && (this->maxLevel > allowed))
        {
            Data::cout() << "Changing maxLevel from " << this->maxLevel << " to " << allowed << Data::endl;

            this->maxLevel = allowed;
        }

        if ((!this->buildHistogram) && (this->minLevel > allowed))
        {
            Data::cout() << "Changing minLevel from " << this->minLevel << " to " << allowed << Data::endl;

            this->minLevel = allowed;
        }
    }
}

void PMJMesh2::CellCounter::findLevels(const Data::BoxTreeCell *cell)
{
    if (this->isOutCell(cell))
    {
        return;
    }

    if (cell->isLeaf())
    {
        UInt level = cell->level();

        if (level < this->minLevel)
        {
            this->minLevel = level;
        }

        if (level > this->maxLevel)
        {
            this->maxLevel = level;
        }
    }

    for (UInt i = 0; i < cell->numChildren(); i++)
    {
        this->findLevels(static_cast<Data::BoxTreeCell *>(cell->child(i)));
    }
}

void PMJMesh2::CellCounter::findCells()
{
    Data::BSPTreeCell *root = static_cast<Data::BSPTreeCell *>(this->bsp->getRoot());

    this->map[root->getId()].cell = root;
    //this->map[root->getId()].cells.push_back(this->tree->getRoot());
    this->tree->getLeaves(this->map[root->getId()].cells);

    this->findLayers(root);

    this->findCells(root);
}

bool PMJMesh2::CellCounter::include(const Data::BoxTreeCell *c, UInt side, UInt pt, UInt coord, Real cmp) const
{
    if (this->isOutCell(c))
    {
        return false;
    }

    Real value = c->box()->getPoint(pt)->getCoord(coord);

    Real diff = value - cmp;

    //return (side < this->dimension) ? (value >= cmp) : (value <= cmp);
    return (side < this->dimension) ? (diff >= -Data::getTolerance()) : (diff <= Data::getTolerance());
}

void PMJMesh2::CellCounter::findInitialLayer(UInt side, const Data::GraphNodeList &cells,
    UInt pt, UInt coord, Real cmp, PositionCellMap &map) const
{
    for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        if (!this->include(c, side, pt, coord, cmp))
        {
            continue;
        }

        Data::GraphNodeList leaves;

        this->tree->children(c, side, leaves);

        while (!leaves.empty())
        {
            c = static_cast<Data::BoxTreeCell *>(leaves.front());

            leaves.pop_front();

            map[PMJMesh::getPosition(this->dimension, c)].insert(c);
        }
    }
}

void PMJMesh2::CellCounter::findInitialLayer(const Data::BSPTreeCell *cell, UInt side,
    const Data::GraphNodeList &cells, LayerSidePositionCellMap &layers) const
{
    UInt coord = side%this->dimension;
    UInt pt = 1 - side/this->dimension;
    Real cmp = cell->box()->getPoint(pt)->getCoord(coord);

    this->findInitialLayer(side, cells, pt, coord, cmp, layers[0][side]);
}

void PMJMesh2::CellCounter::findLayer(const Data::GraphNodeSet &inCellsPrev,
    UInt oside, UInt opt, UInt coord, Real ocmp, PositionCellMap &map) const
{
    for (Data::GraphNodeSet::const_iterator iter = inCellsPrev.begin();
         iter != inCellsPrev.end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        Data::GraphNodeList neighs = c->neighbors(oside);

        for (Data::GraphNodeList::iterator iter2 = neighs.begin();
             iter2 != neighs.end(); iter2++)
        {
            Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter2));

            if (this->include(n, oside, opt, coord, ocmp))
            {
                continue;
            }

            map[PMJMesh::getPosition(this->dimension, n)].insert(n);
        }
    }
}

void PMJMesh2::CellCounter::findLayer(const Data::BSPTreeCell *cell, UInt layer, UInt side, LayerSidePositionCellMap &layers) const
{
    UInt oside = (this->dimension + side)%(2*this->dimension);

    UInt coord = side%this->dimension;
    UInt opt = side/this->dimension;
    Real ocmp = cell->box()->getPoint(opt)->getCoord(coord);

    PositionCellMap::iterator inCellsPrev = layers[layer - 1][side].find(Data::IN);

    if (inCellsPrev == layers[layer - 1][side].end())
    {
        return;
    }

    this->findLayer((*inCellsPrev).second, oside, opt, coord, ocmp, layers[layer][side]);
}

void PMJMesh2::CellCounter::findLayers(const Data::BSPTreeCell *cell)
{
    if ((this->numLayers == 0) || (!cell->isLeaf()))
    {
        return;
    }

    for (UInt side = 0; side < cell->numSides(); side++)
    {
        this->findInitialLayer(cell, side, this->map[cell->getId()].cells, this->map[cell->getId()].layers);
    }

    for (UInt layer = 1; layer < this->numLayers; layer++)
    {
        for (UInt side = 0; side < cell->numSides(); side++)
        {
            this->findLayer(cell, layer, side, this->map[cell->getId()].layers);
        }
    }
}

void PMJMesh2::CellCounter::addHistogram(const Data::BoxTreeCell *c, Cell &cell)
{
    if (c->isLeaf())
    {
        if (this->excludeLayersInHistogram)
        {
            for (LayerSidePositionCellMap::iterator iter = cell.layers.begin();
                 iter != cell.layers.end(); iter++)
            {
                for (SidePositionCellMap::iterator iter2 = (*iter).second.begin();
                     iter2 != (*iter).second.end(); iter2++)
                {
                    for (PositionCellMap::iterator iter3 = (*iter2).second.begin();
                         iter3 != (*iter2).second.end(); iter3++)
                    {
                        if ((!this->onlyINCellsInHistogram) && ((*iter3).first == Data::ON))
                        {
                            continue;
                        }

                        if ((*iter3).second.find(const_cast<Data::BoxTreeCell *>(c)) != (*iter3).second.end())
                        {
                            return;
                        }
                    }
                }
            }
        }

        std::pair<Data::ULIntMap::iterator, bool> p = cell.histogram.insert(std::make_pair(c->level(), 0));

        ++(*p.first).second;

        return;
    }

    for (UInt i = 0; i < c->numChildren(); i++)
    {
        this->addHistogram(static_cast<Data::BoxTreeCell *>(c->child(i)), cell);
    }
}

void PMJMesh2::CellCounter::findCells(const Data::BSPTreeCell *cell)
{
    if (cell->isLeaf())
    {
        if (this->buildHistogram)
        {
            CellMap::iterator it = this->map.find(cell->getId());

            for (Data::GraphNodeList::iterator iter = (*it).second.cells.begin();
                 iter != (*it).second.cells.end(); iter++)
            {
                this->addHistogram(static_cast<Data::BoxTreeCell *>((*iter)), (*it).second);
            }
        }

        return;
    }

    Data::BSPTreeCell *child1 = static_cast<Data::BSPTreeCell *>(cell->child(0));
    Data::BSPTreeCell *child2 = static_cast<Data::BSPTreeCell *>(cell->child(1));

    Data::GraphNodeList cells1, cells2;

    this->split(this->map[cell->getId()].cells, cell->getSplitDirection(), cell->getSplitCoord(), cells1, cells2, true);

    this->map[child1->getId()].cell = child1;
    this->map[child1->getId()].cells.swap(cells1);

    this->map[child2->getId()].cell = child2;
    this->map[child2->getId()].cells.swap(cells2);

    this->findLayers(child1);
    this->findLayers(child2);

    this->findCells(child1);
    this->findCells(child2);

    if (!cell->isLeaf())
    {
        this->map.erase(cell->getId());
    }
}

void PMJMesh2::CellCounter::refine()
{
    PMJMesh2::CellCounter::MinTreeRefiner refiner(this->dimension, this->maxLevel, this->tree, this->boundary);

    refiner.setElementsMap(this->elementsMap);

    refiner.execute();

    this->elementsMap = NULL;

    this->addRefinedLayers();
}

void PMJMesh2::CellCounter::addRefinedLayers()
{
    for (CellMap::iterator iter = this->map.begin();
         iter != this->map.end(); iter++)
    {
        for (LayerSidePositionCellMap::iterator iter2 = (*iter).second.layers.begin();
             iter2 != (*iter).second.layers.end(); iter2++)
        {
            for (SidePositionCellMap::iterator iter3 = (*iter2).second.begin();
                 iter3 != (*iter2).second.end(); iter3++)
            {
                for (PositionCellMap::iterator iter4 = (*iter3).second.begin();
                     iter4 != (*iter3).second.end(); iter4++)
                {
                    Data::GraphNodeList cells((*iter4).second.begin(), (*iter4).second.end());

                    while (!cells.empty())
                    {
                        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(cells.front());

                        cells.pop_front();

                        if (c->isLeaf())
                        {
                            continue;
                        }

                        for (UInt i = 0; i < c->numChildren(); i++)
                        {
                            Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(i));

                            if (this->isOutCell(child))
                            {
                                continue;
                            }

                            (*iter4).second.insert(child);

                            cells.push_back(child);
                        }
                    }
                }
            }
        }
    }
}

void PMJMesh2::CellCounter::countNormal()
{
    for (CellMap::iterator iter = this->map.begin();
         iter != this->map.end(); iter++)
    {
        Data::BSPTreeCell *cell = (*iter).second.cell;

        //(*iter).second.count += this->countNormal(cell, (*iter).second.cells, (*iter).second.layers);
        (*iter).second.count.sum(this->countNormal(cell, (*iter).second.cells, (*iter).second.layers), this->numLayers);
    }
}

void PMJMesh2::CellCounter::count()
{
    for (CellMap::iterator iter = this->map.begin();
         iter != this->map.end(); iter++)
    {
        Data::BSPTreeCell *cell = (*iter).second.cell;

        //(*iter).second.count += this->count(cell, (*iter).second.cells, (*iter).second.layers);
        (*iter).second.count.sum(this->count(cell, (*iter).second.cells, (*iter).second.layers), this->numLayers);
    }
}

PMJMesh2::CellCounter::Cell::Count PMJMesh2::CellCounter::countNormal(const Data::BSPTreeCell *cell,
    const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers) const
{
    Cell::Count count(this->numLayers);

    if (!cell->isLeaf())
    {
        return count;
    }

    for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        //count += this->countNormal(static_cast<Data::BoxTreeCell *>((*iter)), layers);
        count.sum(this->countNormal(static_cast<Data::BoxTreeCell *>((*iter)), layers), this->numLayers);
    }

    return count;
}

PMJMesh2::CellCounter::Cell::Count PMJMesh2::CellCounter::count(const Data::BSPTreeCell *cell,
    const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers) const
{
    Cell::Count count(this->numLayers);

    if (!cell->isLeaf())
    {
        return count;
    }

    for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        //count += this->count(static_cast<Data::BoxTreeCell *>((*iter)), layers);
        count.sum(this->count(static_cast<Data::BoxTreeCell *>((*iter)), layers), this->numLayers);
    }

    return count;
}

PMJMesh2::CellCounter::Cell::Count PMJMesh2::CellCounter::countNormal(const Data::BoxTreeCell *c,
    const LayerSidePositionCellMap &layers) const
{
    Cell::Count count(this->numLayers);

    Data::Position pos = PMJMesh::getPosition(this->dimension, c);

    if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
    {
        return count;
    }

    if (c->isLeaf())
    {
        if (pos == Data::ON)
        {
            count.normalON++;
        }
        else
        {
            count.normalIN++;

            for (UInt layer = 0; layer < this->numLayers; layer++)
            {
                if (this->inLayer(c, layers, layer))
                {
                    count.normalINLayer[layer]++;
                }
            }
        }
    }

    for (UInt i = 0; i < c->numChildren(); i++)
    {
        Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(i));

        //count += this->countNormal(child, layers);
        count.sum(this->countNormal(child, layers), this->numLayers);
    }

    return count;
}

PMJMesh2::CellCounter::Cell::Count PMJMesh2::CellCounter::count(const Data::BoxTreeCell *c,
    const LayerSidePositionCellMap &layers) const
{
    Cell::Count count(this->numLayers);

    Data::Position pos = PMJMesh::getPosition(this->dimension, c);

    if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
    {
        return count;
    }

    UInt level = c->level();

    bool testedInLayer = false;
    Data::BoolVector inLayer(this->numLayers, false);

    if (level == this->minLevel)
    {
        if (pos == Data::ON)
        {
            count.minON++;
        }
        else
        {
            count.minIN++;

            if (!testedInLayer)
            {
                testedInLayer = true;

                for (UInt layer = 0; layer < this->numLayers; layer++)
                {
                    inLayer[layer] = this->inLayer(c, layers, layer);
                }
            }

            for (UInt layer = 0; layer < this->numLayers; layer++)
            {
                if (inLayer[layer])
                {
                    count.minINLayer[layer]++;
                }
            }
        }
    }

    if (level == this->maxLevel)
    {
        if (pos == Data::ON)
        {
            count.maxON++;
        }
        else
        {
            count.maxIN++;

            if (!testedInLayer)
            {
                testedInLayer = true;

                for (UInt layer = 0; layer < this->numLayers; layer++)
                {
                    inLayer[layer] = this->inLayer(c, layers, layer);
                }
            }

            for (UInt layer = 0; layer < this->numLayers; layer++)
            {
                if (inLayer[layer])
                {
                    count.maxINLayer[layer]++;
                }
            }
        }
    }

    if ((c->isLeaf()) && (pos == Data::IN))
    {
        if (level < this->minLevel)
        {
            UInt diff = this->minLevel - level;

            UInt numberLeavesPerLevel = static_cast<UInt>(std::pow(2, this->dimension));

            UInt numberLeaves = static_cast<UInt>(std::pow(numberLeavesPerLevel, diff));

            count.minIN += numberLeaves;

            if (!testedInLayer)
            {
                testedInLayer = true;

                for (UInt layer = 0; layer < this->numLayers; layer++)
                {
                    inLayer[layer] = this->inLayer(c, layers, layer);
                }
            }

            for (UInt layer = 0; layer < this->numLayers; layer++)
            {
                if (inLayer[layer])
                {
                    count.minINLayer[layer] += numberLeaves;
                }
            }
        }

        if (level < this->maxLevel)
        {
            UInt diff = this->maxLevel - level;

            UInt numberLeavesPerLevel = static_cast<UInt>(std::pow(2, this->dimension));

            UInt numberLeaves = static_cast<UInt>(std::pow(numberLeavesPerLevel, diff));

            count.maxIN += numberLeaves;

            if (!testedInLayer)
            {
                testedInLayer = true;

                for (UInt layer = 0; layer < this->numLayers; layer++)
                {
                    inLayer[layer] = this->inLayer(c, layers, layer);
                }
            }

            for (UInt layer = 0; layer < this->numLayers; layer++)
            {
                if (inLayer[layer])
                {
                    count.maxINLayer[layer] += numberLeaves;
                }
            }
        }
    }

    for (UInt i = 0; i < c->numChildren(); i++)
    {
        Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(i));

        //count += this->count(child, layers);
        count.sum(this->count(child, layers), this->numLayers);
    }

    return count;
}

bool PMJMesh2::CellCounter::inLayer(const Data::BoxTreeCell *c, const LayerSidePositionCellMap &layers, UInt layer) const
{
    for (LayerSidePositionCellMap::const_iterator iter = layers.begin();
         iter != layers.end(); iter++)
    {
        if ((*iter).first > layer)
        {
            continue;
        }

        for (SidePositionCellMap::const_iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end(); iter2++)
        {
            for (PositionCellMap::const_iterator iter3 = (*iter2).second.begin();
                 iter3 != (*iter2).second.end(); iter3++)
            {
                if ((*iter3).second.find(const_cast<Data::BoxTreeCell *>(c)) != (*iter3).second.end())
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool PMJMesh2::CellCounter::isOutCell(const Data::BoxTreeCell *c) const
{
    Data::Position pos = PMJMesh::getPosition(this->dimension, c);

    return ((pos == Data::OUT) || (pos == Data::UNKNOWN));
}

bool PMJMesh2::CellCounter::less(const Data::BoxTreeCell *cell, UInt dir, Real coord) const
{
    return (0.5*(cell->box()->getMin()->getCoord(dir) + cell->box()->getMax()->getCoord(dir)) < coord);
}

bool PMJMesh2::CellCounter::split(const Data::GraphNodeList &cells, UInt dir, Real coord,
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

#endif //#if DEBUG_CELL_COUNTER

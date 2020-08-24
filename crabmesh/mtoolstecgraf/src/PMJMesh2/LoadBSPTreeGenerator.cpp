#include "PMJMesh2/LoadBSPTreeGenerator.h"

#include "Data/OStream.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"

#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Triangle3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementBoxSearchTree.h"
#include "MJMesh/FrontElementSearchTreeGenerator.h"

using namespace PMJMesh2;

/*void debug(const PartitionBSPTreeCell *cell, const LoadBSPTreeGenerator::CellLoadMap &loads)
{
    Data::cout() << "BSP cell: " << cell->getId() << Data::endl;

    LoadBSPTreeGenerator::CellLoadMap::const_iterator it = loads.find(cell->getId());

    if (it == loads.end())
    {
        Data::cout() << "no cells/load for BSP cell" << Data::endl;
    }
    else
    {
        Data::cout() << "cells for BSP cell:";

        for (Data::GraphNodeList::const_iterator iter = (*it).second.cells.begin();
             iter != (*it).second.cells.end(); iter++)
        {
            Data::cout() << " " << (*iter)->getId();
        }

        Data::cout() << "\nload for BSP cell: " << (*it).second.load << Data::endl;
    }
}*/

PMJMesh2::LoadBSPTreeGenerator::Params::Params()
{
    this->dir = 0;
    this->coord = 0.0;

    this->limits[0] = this->limits[1] = 0.0;
    this->cells = NULL;
    this->splittedLoads[0] = this->splittedLoads[1] = 0.0;
    this->fixedLoads[0] = this->fixedLoads[1] = 0.0;
    this->equal = false;
    this->splittingLoad = 0.0;

    this->layersLoads[0] = this->layersLoads[1] = 0.0;
    this->layersLimits[0] = this->layersLimits[1] = 0.0;

#if USE_ARRAY_ON_PARAMS
    this->layers = NULL;
    this->splittedLayers[0] = this->splittedLayers[1] = NULL;
    this->fixedLayers[0] = this->fixedLayers[1] = NULL;
#endif //#if USE_ARRAY_ON_PARAMS
}

#if USE_ARRAY_ON_PARAMS
void PMJMesh2::LoadBSPTreeGenerator::Params::resize(UInt numLayers, UInt numSides, bool layers, bool splitted, bool fixed)
{
    if (numLayers == 0)
    {
        return;
    }

    if (layers)
    {
        if (this->layers)
        {
            layers = false;
        }
        else
        {
            this->layers = new PositionCellMap*[numLayers];

            //Data::cout() << "new layers at " << this->layers << Data::endl;
        }

    }

    if (splitted)
    {
        if ((this->splittedLayers[0]) && (this->splittedLayers[1]))
        {
            splitted = false;
        }
        else
        {
            this->splittedLayers[0] = new PositionCellMap*[numLayers];
            this->splittedLayers[1] = new PositionCellMap*[numLayers];

            //Data::cout() << "new splittedLayers[0] at " << this->splittedLayers[0] << Data::endl;
            //Data::cout() << "new splittedLayers[1] at " << this->splittedLayers[1] << Data::endl;
        }
    }

    if (fixed)
    {
        if ((this->fixedLayers[0]) && (this->fixedLayers[1]))
        {
            fixed = false;
        }
        else
        {
            this->fixedLayers[0] = new PositionCellMap*[numLayers];
            this->fixedLayers[1] = new PositionCellMap*[numLayers];

            //Data::cout() << "new fixedLayers[0] at " << this->fixedLayers[0] << Data::endl;
            //Data::cout() << "new fixedLayers[1] at " << this->fixedLayers[1] << Data::endl;
        }
    }

    for (UInt i = 0; i < numLayers; i++)
    {
        if (layers)
        {
            this->layers[i] = new PositionCellMap[numSides];

            //Data::cout() << "new layers[" << i << "] at " << this->layers[i] << Data::endl;
        }

        if (splitted)
        {
            this->splittedLayers[0][i] = new PositionCellMap[numSides];
            this->splittedLayers[1][i] = new PositionCellMap[numSides];

            //Data::cout() << "new splittedLayers[0][" << i << "] at " << this->splittedLayers[0][i] << Data::endl;
            //Data::cout() << "new splittedLayers[1][" << i << "] at " << this->splittedLayers[1][i] << Data::endl;
        }

        if (fixed)
        {
            this->fixedLayers[0][i] = new PositionCellMap[numSides];
            this->fixedLayers[1][i] = new PositionCellMap[numSides];

            //Data::cout() << "new fixedLayers[0][" << i << "] at " << this->fixedLayers[0][i] << Data::endl;
            //Data::cout() << "new fixedLayers[1][" << i << "] at " << this->fixedLayers[1][i] << Data::endl;
        }
    }
}

void PMJMesh2::LoadBSPTreeGenerator::Params::resize(PositionCellMap **&layers, UInt numLayers, UInt numSides)
{
    if (layers)
    {
        return;
    }

    layers = new PositionCellMap*[numLayers];

    //Data::cout() << "new hanging layers at " << layers << Data::endl;

    for (UInt i = 0; i < numLayers; i++)
    {
        layers[i] = new PositionCellMap[numSides];

        //Data::cout() << "new hanging layers[" << i << "] at " << layers[i] << Data::endl;
    }
}

void PMJMesh2::LoadBSPTreeGenerator::Params::clear(UInt numLayers, bool layers, bool splitted, bool fixed)
{
    if (numLayers == 0)
    {
        return;
    }

    for (UInt i = 0; i < numLayers; i++)
    {
        if ((layers) && (this->layers) && (this->layers[i]))
        {
            //Data::cout() << "delete layers[" << i << "] at " << this->layers[i] << Data::endl;

            delete [] this->layers[i];
        }

        if ((splitted) && (this->splittedLayers[0]) && (this->splittedLayers[0][i]))
        {
            //Data::cout() << "delete splittedLayers[0][" << i << "] at " << this->splittedLayers[0][i] << Data::endl;

            delete [] this->splittedLayers[0][i];
        }

        if ((splitted) && (this->splittedLayers[1]) && (this->splittedLayers[1][i]))
        {
            //Data::cout() << "delete splittedLayers[1][" << i << "] at " << this->splittedLayers[1][i] << Data::endl;

            delete [] this->splittedLayers[1][i];
        }

        if ((fixed) && (this->fixedLayers[0]) && (this->fixedLayers[0][i]))
        {
            //Data::cout() << "delete fixedLayers[0][" << i << "] at " << this->fixedLayers[0][i] << Data::endl;

            delete [] this->fixedLayers[0][i];
        }

        if ((fixed) && (this->fixedLayers[1]) && (this->fixedLayers[1][i]))
        {
            //Data::cout() << "delete fixedLayers[1][" << i << "] at " << this->fixedLayers[1][i] << Data::endl;

            delete [] this->fixedLayers[1][i];
        }
    }

    if ((layers) && (this->layers))
    {
        //Data::cout() << "delete layers at " << this->layers << Data::endl;

        delete [] this->layers;

        this->layers = NULL;
    }

    if ((splitted) && (this->splittedLayers[0]))
    {
        //Data::cout() << "delete splittedLayers[0] at " << this->splittedLayers[0] << Data::endl;

        delete [] this->splittedLayers[0];

        this->splittedLayers[0] = NULL;
    }

    if ((splitted) && (this->splittedLayers[1]))
    {
        //Data::cout() << "delete splittedLayers[1] at " << this->splittedLayers[1] << Data::endl;

        delete [] this->splittedLayers[1];

        this->splittedLayers[1] = NULL;
    }

    if ((fixed) && (this->fixedLayers[0]))
    {
        //Data::cout() << "delete fixedLayers[0] at " << this->fixedLayers[0] << Data::endl;

        delete [] this->fixedLayers[0];

        this->fixedLayers[0] = NULL;
    }

    if ((fixed) && (this->fixedLayers[1]))
    {
        //Data::cout() << "delete fixedLayers[1] at " << this->fixedLayers[1] << Data::endl;

        delete [] this->fixedLayers[1];

        this->fixedLayers[1] = NULL;
    }
}

void PMJMesh2::LoadBSPTreeGenerator::Params::clear(PositionCellMap **&layers, UInt numLayers)
{
    for (UInt i = 0; i < numLayers; i++)
    {
        if ((layers) && (layers[i]))
        {
            //Data::cout() << "delete hanging layers[" << i << "] at " << layers[i] << Data::endl;

            delete [] layers[i];
        }
    }

    if (layers)
    {
        //Data::cout() << "delete hanging layers at " << layers << Data::endl;

        delete [] layers;
    }

    layers = NULL;
}
#endif //#if USE_ARRAY_ON_PARAMS

/*#if USE_GUI
void PMJMesh2::LoadBSPTreeGenerator::subdivide(PartitionBSPTreeCell *cell, const Split &split) const
{
    PMJMesh2::PartitionBSPTreeGenerator::subdivide(cell, split);

    for (UInt i = 0; i < 2; i++)
    {
        CellLoadMap::const_iterator iter = this->loads.find(cell->child(i)->getId());

        Data::GraphNodeList cells = (*iter).second.cells;
        Data::GraphNodeList innerCells;

        while (!cells.empty())
        {
            Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(cells.front());

            cells.pop_front();

            if (c->isLeaf())
            {
                Data::Position pos = PMJMesh::getPosition(this->dimension, c);

                if ((pos == Data::IN) || (pos == Data::ON))
                {
                    bool found = false;

                    UInt numSides = 2*this->dimension;

                    for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
                    {
                        for (UInt side = 0; side < numSides; side++)
                        {
                            if (((*iter).second.layers[layer][side][Data::IN].find(c) != (*iter).second.layers[layer][side][Data::IN].end()))
                            {
                                found = true;

                                break;
                            }
                        }

                        if (found)
                        {
                            break;
                        }
                    }

                    if (!found)
                    {
                        innerCells.push_back(c);
                    }
                }

                continue;
            }

            for (UInt j = 0; j < c->numChildren(); j++)
            {
                cells.push_back(c->child(j));
            }
        }

        static_cast<PartitionBSPTreeCell *>(cell->child(i))->cells = innerCells;
    }
}
#endif //#if USE_GUI*/

PMJMesh2::LoadBSPTreeGenerator::LoadBSPTreeGenerator() :
    PMJMesh2::PartitionBSPTreeGenerator::PartitionBSPTreeGenerator()
{
    //this->treeLoad = NULL;
    this->creatingInnerFrontInDecomposition = false;
    this->proportional = false;
    this->loadThreshold = 0.0;
    this->proportions[0] = this->proportions[1] = 1;

    this->usingFrontTest = false;
    this->boundary = NULL;
    this->searchTree = NULL;

    this->interfaceBadAngleCos = 0.95;
    this->interfaceShiftDelta = 2.0;
}

PMJMesh2::LoadBSPTreeGenerator::~LoadBSPTreeGenerator()
{
#if USE_ARRAY_ON_PARAMS
    if (this->removeLayersInLoad != 0)
    {
        for (CellLoadMap::iterator iter = this->loads.begin();
             iter != this->loads.end(); iter++)
        {
            Params::clear((*iter).second.layers, this->removeLayersInLoad);
        }
    }
#endif //#if USE_ARRAY_ON_PARAMS

    if (this->searchTree)
    {
        delete this->searchTree;
    }
}

void PMJMesh2::LoadBSPTreeGenerator::setCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition)
{
    this->creatingInnerFrontInDecomposition = creatingInnerFrontInDecomposition;
}

void PMJMesh2::LoadBSPTreeGenerator::setProportional(bool proportional)
{
    this->proportional = proportional;
}

void PMJMesh2::LoadBSPTreeGenerator::setLoadThreshold(Real loadThreshold)
{
    this->loadThreshold = loadThreshold;
}

void PMJMesh2::LoadBSPTreeGenerator::setUsingFrontTest(bool usingFrontTest)
{
    this->usingFrontTest = usingFrontTest;
}

bool PMJMesh2::LoadBSPTreeGenerator::isUsingFrontTest() const
{
    return this->usingFrontTest;
}

void PMJMesh2::LoadBSPTreeGenerator::setBoundary(const MJMesh::Boundary *boundary)
{
    this->boundary = boundary;
}

void PMJMesh2::LoadBSPTreeGenerator::setInterfaceBadAngleCos(Real interfaceBadAngleCos)
{
    this->interfaceBadAngleCos = interfaceBadAngleCos;
}

Real PMJMesh2::LoadBSPTreeGenerator::getInterfaceBadAngleCos() const
{
    return this->interfaceBadAngleCos;
}

void PMJMesh2::LoadBSPTreeGenerator::setInterfaceShiftDelta(Real interfaceShiftDelta)
{
    this->interfaceShiftDelta = interfaceShiftDelta;
}

Real PMJMesh2::LoadBSPTreeGenerator::getInterfaceShiftDelta() const
{
    return this->interfaceShiftDelta;
}

Real PMJMesh2::LoadBSPTreeGenerator::load(const PartitionBSPTreeCell *cell) const
{
    CellLoadMap::iterator it = this->loads.find(cell->getId());

    /*//debug
    if ((it != this->loads.end()) && (cell->isLeaf()) &&
        (Data::trace().find("makePartition") != std::string::npos))
    {
        UInt ons, ins, outs, unknowns, total;
        ons = ins = outs = unknowns = total = 0;

        for (Data::GraphNodeList::iterator iter = (*it).second.cells.begin();
             iter != (*it).second.cells.end(); iter++)
        {
            Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

            Data::GraphNodeList leaves = c->getLeaves();

            while (!leaves.empty())
            {
                c = static_cast<Data::BoxTreeCell *>(leaves.front());

                leaves.pop_front();

                Data::Position pos = PMJMesh::getPosition(this->dimension, c);

                switch (pos)
                {
                case Data::IN:
                    ins++;
                    break;
                case Data::OUT:
                    outs++;
                    break;
                case Data::ON:
                    ons++;
                    break;
                case Data::UNKNOWN:
                    unknowns++;
                    break;
                }

                total++;
            }
        }

        Data::GraphNodeSet onCells[cell->numSides()][this->removeLayersInLoad], inCells[cell->numSides()][this->removeLayersInLoad];
        Data::GraphNodeSet allOnCells[this->removeLayersInLoad], allInCells[this->removeLayersInLoad];

        if (this->removeLayersInLoad > 0)
        {
#if USE_GUI
            //bool colorize = (cell->getId() == 362);
            bool colorize = false;

            if (colorize)
            {
                this->boxTree->freeze();
            }
#endif //#if USE_GUI

            for (UInt side = 0; side < cell->numSides(); side++)
            {
                UInt coord = side%this->dimension;
                UInt pt = 1 - side/this->dimension;
                Real cmp = cell->box()->getPoint(pt)->getCoord(coord);

                for (Data::GraphNodeList::iterator iter = (*it).second.cells.begin();
                     iter != (*it).second.cells.end(); iter++)
                {
                    Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

                    Data::Position pos = PMJMesh::getPosition(this->dimension, c);

                    if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
                    {
                        continue;
                    }

                    Data::GraphNodeList leaves = c->getLeaves();

                    while (!leaves.empty())
                    {
                        c = static_cast<Data::BoxTreeCell *>(leaves.front());

                        leaves.pop_front();

                        pos = PMJMesh::getPosition(this->dimension, c);

                        if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
                        {
                            continue;
                        }

                        Real value = c->box()->getPoint(pt)->getCoord(coord);

                        Real diff = value - cmp;

                        bool include = (side < this->dimension) ? diff >= -Data::getTolerance(): diff <= Data::getTolerance();

                        if (include)
                        {
                            if (pos == Data::ON)
                            {
                                onCells[side][0].insert(c);

#if USE_GUI
                                if (colorize)
                                {
                                    c->setColor(0.8, 0.8, 0.0);
                                    c->highlight();
                                }
#endif //#if USE_GUI
                            }
                            else
                            {
                                inCells[side][0].insert(c);

#if USE_GUI
                                if (colorize)
                                {
                                    c->setColor(0.0, 0.8, 0.0);
                                    c->highlight();
                                }
#endif //#if USE_GUI
                            }
                        }
                    }
                }
            }

            for (UInt layer = 1; layer < this->removeLayersInLoad; layer++)
            {
                for (UInt side = 0; side < cell->numSides(); side++)
                {
                    UInt oside = (this->dimension + side)%(2*this->dimension);

                    UInt coord = side%this->dimension;
                    UInt pt = side/this->dimension;
                    Real cmp = cell->box()->getPoint(pt)->getCoord(coord);

                    for (Data::GraphNodeSet::iterator iter = inCells[side][layer - 1].begin();
                         iter != inCells[side][layer-1].end(); iter++)
                    {
                        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

                        Data::GraphNodeList neighs = c->neighbors(oside);

                        for (Data::GraphNodeList::iterator iter2 = neighs.begin();
                             iter2 != neighs.end(); iter2++)
                        {
                            Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter2));

                            Data::Position pos = PMJMesh::getPosition(this->dimension, n);

                            if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
                            {
                                continue;
                            }

                            Real value = c->box()->getPoint(pt)->getCoord(coord);

                            Real diff = value - cmp;

                            bool exclude = (side < this->dimension) ? diff < -Data::getTolerance(): diff > Data::getTolerance();

                            if (exclude)
                            {
                                continue;
                            }

                            if (pos == Data::ON)
                            {
                                onCells[side][layer].insert(n);

#if USE_GUI
                                if (colorize)
                                {
                                    n->setColor(1.0 - (layer + 1)*0.2, 1.0 - (layer + 1)*0.2, 0.0);
                                    n->highlight();
                                }
#endif //#if USE_GUI
                            }
                            else
                            {
                                inCells[side][layer].insert(n);

#if USE_GUI
                                if (colorize)
                                {
                                    n->setColor(0.0, 1.0 - (layer + 1)*0.2, 0.0);
                                    n->highlight();
                                }
#endif //#if USE_GUI
                            }
                        }
                    }
                }
            }

            for (UInt i = 0; i < this->removeLayersInLoad; i++)
            {
                for (UInt j = 0; j < cell->numSides(); j++)
                {
                    allOnCells[i].insert(onCells[j][i].begin(), onCells[j][i].end());
                    allInCells[i].insert(inCells[j][i].begin(), inCells[j][i].end());
                }

                if (i > 0)
                {
                    allOnCells[i].insert(allOnCells[i-1].begin(), allOnCells[i-1].end());
                    allInCells[i].insert(allInCells[i-1].begin(), allInCells[i-1].end());
                }
            }

#if USE_GUI
            if (colorize)
            {
                this->boxTree->unfreeze();

                this->boxTree->clearLists();
            }
#endif //#if USE_GUI
        }

        Data::cout() << "cell " << cell->getId() << ", load " << (*it).second.load
            << ", ins " << ins << ", ons " << ons << ", outs " << outs
            << ", unks " << unknowns << ", total " << total << Data::endl;

        if (this->removeLayersInLoad > 0)
        {
            Data::cout() << "cell " << cell->getId();

            for (UInt i = 0; i < this->removeLayersInLoad; i++)
            {
                Data::cout() << ", ons[" << i << "] " << allOnCells[i].size() << ", ins[" << i << "] " << allInCells[i].size();
            }

            Data::cout() << Data::endl;
        }
    }
    //endebug*/

    return (it == this->loads.end()) ? -1.0 : (*it).second.load;
}

Real PMJMesh2::LoadBSPTreeGenerator::innerLoad(const PartitionBSPTreeCell *cell) const
{
#if USE_C__11
    Data::RealHashMap::iterator it = this->innerLoads.find(cell->getId());
#else
    Data::RealMap::iterator it = this->innerLoads.find(cell->getId());
#endif //#if USE_C__11

    return (it == this->innerLoads.end()) ? -1.0 : (*it).second;
}

PartitionBSPTreeCell *PMJMesh2::LoadBSPTreeGenerator::executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size)
{
    /*if (!this->treeLoad)
    {
        this->treeLoad = &this->loadCalculator->getTreeLoad();
    }*/

    if (cell->isRoot())
    {
        std::pair<CellLoadMap::iterator, bool> p = this->loads.insert(std::make_pair(cell->getId(), CellLoad()));
        this->innerLoads.insert(std::make_pair(cell->getId(), 1.0));

        if (p.second)
        {
            (*p.first).second.cells.push_back(this->boxTree->getRoot());
            (*p.first).second.load = (*this->loadCalculator->getTreeLoad().find(this->boxTree->getRoot()->getId())).second;
            this->findLayers(cell, (*p.first).second.cells, (*p.first).second.layers);
        }
    }

    if ((this->proportional) && (size != 1))
    {
        this->proportions[0] = this->proportions[1] = size/2;

        if (this->proportions[0] + this->proportions[1] != size)
        {
            this->proportions[1]++;
        }
    }
    else
    {
        this->proportions[0] = this->proportions[1] = 1;
    }

    return PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision(cell, rank, size);
}

void PMJMesh2::LoadBSPTreeGenerator::adjustLoadsForLayers(Real &l0, Real &l1) const
{
    if (this->removeLayersInLoad == 0)
    {
        return;
    }

    if (l0 < 0.0)
    {
        l1 += l0;
        l0 = 0.0;
    }
    else if (l1 < 0.0)
    {
        l0 += l1;
        l1 = 0.0;
    }
}

void PMJMesh2::LoadBSPTreeGenerator::adjustProportion(Real &l0, Real &l1) const
{
    if (!this->proportional)
    {
        return;
    }

    UInt min = (l0 < l1) ? 0 : 1;

    l0 *= static_cast<Real>(this->proportions[1 - min]);
    l1 *= static_cast<Real>(this->proportions[min]);
}

bool PMJMesh2::LoadBSPTreeGenerator::better(Real diffLoad1, Real diffLoad2) const
{
    return (diffLoad1 < diffLoad2);
}

bool PMJMesh2::LoadBSPTreeGenerator::better(const Params &params1, const Params &params2) const
{
    //Real l1_0 = params1.splittedLoads[0] + params1.fixedLoads[0];
    //Real l1_1 = params1.splittedLoads[1] + params1.fixedLoads[1];
    Real l1_0 = params1.splittedLoads[0] + params1.fixedLoads[0] - params1.layersLoads[0];
    Real l1_1 = params1.splittedLoads[1] + params1.fixedLoads[1] - params1.layersLoads[1];

    this->adjustLoadsForLayers(l1_0, l1_1);

    //Real l2_0 = params2.splittedLoads[0] + params2.fixedLoads[0];
    //Real l2_1 = params2.splittedLoads[1] + params2.fixedLoads[1];
    Real l2_0 = params2.splittedLoads[0] + params2.fixedLoads[0] - params2.layersLoads[0];
    Real l2_1 = params2.splittedLoads[1] + params2.fixedLoads[1] - params2.layersLoads[1];

    this->adjustLoadsForLayers(l2_0, l2_1);

    this->adjustProportion(l1_0, l1_1);
    this->adjustProportion(l2_0, l2_1);

    //return (std::fabs(l1_0 - l1_1) < std::fabs(l2_0 - l2_1));
    return this->better(std::fabs(l1_0 - l1_1), std::fabs(l2_0 - l2_1));
}

bool PMJMesh2::LoadBSPTreeGenerator::equal(const Params &params) const
{
    //Real l0 = params.splittedLoads[0] + params.fixedLoads[0];
    //Real l1 = params.splittedLoads[1] + params.fixedLoads[1];
    Real l0 = params.splittedLoads[0] + params.fixedLoads[0] - params.layersLoads[0];
    Real l1 = params.splittedLoads[1] + params.fixedLoads[1] - params.layersLoads[1];

    this->adjustLoadsForLayers(l0, l1);

    this->adjustProportion(l0, l1);

    return (std::fabs(l0 - l1) < Data::getTolerance());

    /*Real dif = std::fabs(l0 - l1);

    if (dif < Data::getTolerance())
    {
        return true;
    }

    if (this->loadThreshold > 0.0)
    {
        Real max = std::max(l0, l1);

        if (dif/max <= this->loadThreshold)
        {
            return true;
        }
    }

    return false;*/
}

UInt PMJMesh2::LoadBSPTreeGenerator::heavier(const Params &params) const
{
    //Real l0 = params.splittedLoads[0] + params.fixedLoads[0];
    //Real l1 = params.splittedLoads[1] + params.fixedLoads[1];
    Real l0 = params.splittedLoads[0] + params.fixedLoads[0] - params.layersLoads[0];
    Real l1 = params.splittedLoads[1] + params.fixedLoads[1] - params.layersLoads[1];

    this->adjustLoadsForLayers(l0, l1);

    //this->adjustProportion(l0, l1);

    return (l0 >= l1) ? 0 : 1;
}

void PMJMesh2::LoadBSPTreeGenerator::move(Params &params, UInt i) const
{
    params.fixedCells[i].splice(params.fixedCells[i].end(), params.splittedCells[i]);
    params.fixedLoads[i] += params.splittedLoads[i];
    params.splittedLoads[i] = 0.0;

    if (this->removeLayersInLoad > 0)
    {
        for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
        {
            for (UInt side = 0; side < 2*this->dimension; side++)
            {
                for (PositionCellMap::iterator iter = params.splittedLayers[i][layer][side].begin();
                     iter != params.splittedLayers[i][layer][side].end(); iter++)
                {
                    params.fixedLayers[i][layer][side][(*iter).first].insert((*iter).second.begin(), (*iter).second.end());

                    (*iter).second.clear();
                }
            }
        }
    }
}

/*void PMJMesh2::LoadBSPTreeGenerator::distanceInBoxTree(Real min, Real max, UInt dir, Real coord, UInt &d1, UInt &d2) const
{
    d1 = d2 = 0;

    this->distanceInBoxTree(min, max, dir, coord, static_cast<Data::BoxTreeCell *>(this->boxTree->getRoot()), d1, d2);
}

void PMJMesh2::LoadBSPTreeGenerator::distanceInBoxTree(Real min, Real max, UInt dir, Real coord, const Data::BoxTreeCell *cell, UInt &d1, UInt &d2) const
{
    if (this->isOutCell(cell))
    {
        return;
    }

    if ((cell->box()->getMax()->getCoord(dir) < min + Data::getTolerance()) ||
        (cell->box()->getMin()->getCoord(dir) > max - Data::getTolerance()))
    {
        return;
    }

    if (cell->isLeaf())
    {
        if (this->less(cell, dir, coord))
        {
            d1++;
        }
        else
        {
            d2++;
        }

        return;
    }

    UInt numChildren = cell->numChildren();

    UInt d1Children[numChildren], d2Children[numChildren];

    for (UInt i = 0; i < numChildren; i++)
    {
        d1Children[i] = 0, d2Children[i] = 0;

        this->distanceInBoxTree(min, max, dir, coord, static_cast<Data::BoxTreeCell *>(cell->child(i)), d1Children[i], d2Children[i]);
    }

    this->combineDistancesInBoxTree(dir, d1Children, d2Children, d1, d2);
}

void PMJMesh2::LoadBSPTreeGenerator::combineDistancesInBoxTree(UInt dir, UInt d1Children[], UInt d2Children[], UInt &d1, UInt &d2) const
{
    if (this->dimension == 1)
    {
        d1 = d1Children[0] + d1Children[1];
        d2 = d2Children[0] + d2Children[1];

        return;
    }

    if (this->dimension == 2)
    {
        switch (dir)
        {
        case 0:
            d1 = std::max(d1Children[0], d1Children[3]) + std::max(d1Children[1], d1Children[2]);
            d2 = std::max(d2Children[0], d2Children[3]) + std::max(d2Children[1], d2Children[2]);
            break;

        case 1:
            d1 = std::max(d1Children[0], d1Children[1]) + std::max(d1Children[2], d1Children[3]);
            d2 = std::max(d2Children[0], d2Children[1]) + std::max(d2Children[2], d2Children[3]);
            break;
        }

        return;
    }

    if (this->dimension == 3)
    {
        switch (dir)
        {
        case 0:
            d1 = std::max(std::max(d1Children[0], d1Children[3]), std::max(d1Children[4], d1Children[7])) +
                 std::max(std::max(d1Children[1], d1Children[2]), std::max(d1Children[5], d1Children[6]));
            d2 = std::max(std::max(d2Children[0], d2Children[3]), std::max(d2Children[4], d2Children[7])) +
                 std::max(std::max(d2Children[1], d2Children[2]), std::max(d2Children[5], d2Children[6]));
            break;

        case 1:
            d1 = std::max(std::max(d1Children[0], d1Children[1]), std::max(d1Children[4], d1Children[5])) +
                 std::max(std::max(d1Children[2], d1Children[3]), std::max(d1Children[6], d1Children[7]));
            d2 = std::max(std::max(d2Children[0], d2Children[1]), std::max(d2Children[4], d2Children[5])) +
                 std::max(std::max(d2Children[2], d2Children[3]), std::max(d2Children[6], d2Children[7]));
            break;

        case 2:
            d1 = std::max(std::max(d1Children[0], d1Children[1]), std::max(d1Children[2], d1Children[3])) +
                 std::max(std::max(d1Children[4], d1Children[5]), std::max(d1Children[6], d1Children[7]));
            d2 = std::max(std::max(d2Children[0], d2Children[1]), std::max(d2Children[2], d2Children[3])) +
                 std::max(std::max(d2Children[4], d2Children[5]), std::max(d2Children[6], d2Children[7]));
            break;
        }

        return;
    }
}*/

Real PMJMesh2::LoadBSPTreeGenerator::initialSplitCoord(Real min, Real max, UInt dir, const Data::GraphNodeList &cells) const
{
    return this->initialSplitCoord(min, max, dir, &cells);
}

Real PMJMesh2::LoadBSPTreeGenerator::initialSplitCoord(Real min, Real max, UInt dir, const Data::GraphNodeList *cells) const
{
    //Data::cout() << "dir = " << dir << ", min = " << min << ", max = " << max << ", cells->size = " << cells->size() << ", tol = " << Data::getTolerance() << Data::endl;

    Data::RealSet coords;

    UInt level = UINT_MAX;

    for (Data::GraphNodeList::const_iterator iter = cells->begin();
         iter != cells->end(); iter++)
    {
        Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>((*iter));

        if (this->isOutCell(cell))
        {
            continue;
        }

        //Data::cout() << "A cell = " << cell->getId() << ", cell->level = " << cell->level() << ", level = " << level << ", coords.size = " << coords.size() << Data::endl;

        if (cell->level() < level)
        {
            level = cell->level();

            coords.clear();
        }

        //Data::cout() << "B cell = " << cell->getId() << ", cell->level = " << cell->level() << ", level = " << level << ", coords.size = " << coords.size() << Data::endl;

        if (cell->level() == level)
        {
            Real bmin = cell->box()->getMin()->getCoord(dir);
            Real bmax = cell->box()->getMax()->getCoord(dir);

            bool addMin = (std::fabs(bmin - min) > Data::getTolerance());
            bool addMax = (std::fabs(bmax - max) > Data::getTolerance());

            //Data::cout() << "bmin = " << bmin << ", bmax = " << bmax << ", dmin = " << std::fabs(bmin - min) << ", dmax = " << std::fabs(bmax - max) << Data::endl;
            //Data::cout() << "bmin = " << bmin << ", bmax = " << bmax << ", addmin = " << std::boolalpha << addMin << ", addmax = " << std::boolalpha << addMax << Data::endl;

            if ((!addMin) && (!addMax))
            {
                return 0.5*(bmin + bmax);
            }

            if (addMin)
            {
                coords.insert(bmin);
            }

            if (addMax)
            {
                coords.insert(bmax);
            }
        }
    }

    /*if (coords.size() > 2)
    {
        Real comp = (*coords.begin());

        for (Data::RealSet::iterator iter = ++coords.begin();
             iter != coords.end();)
        {
            if (std::fabs((*iter) - comp) < Data::getTolerance())
            {
                coords.erase(iter++);

                continue;
            }

            comp = (*iter);

            iter++;
        }
    }*/

    if (coords.empty())
    {
        return 0.5*(min + max);
    }

    Data::RealSet::iterator iter = coords.begin();

    std::advance(iter, coords.size()/2);

    return (*iter);
}

PMJMesh2::PartitionBSPTreeGenerator::Split PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision(PartitionBSPTreeCell *cell) const
{
    std::pair<CellLoadMap::iterator, bool> p = this->loads.insert(std::make_pair(cell->getId(), CellLoad()));

    if ((cell->isRoot()) && (p.second))
    {
        (*p.first).second.cells.push_back(this->boxTree->getRoot());
        (*p.first).second.load = (*this->loadCalculator->getTreeLoad().find(this->boxTree->getRoot()->getId())).second;
        this->findLayers(cell, (*p.first).second.cells, (*p.first).second.layers);
    }

    //Data::cout() << "will find best subdivision for BSP cell " << cell->getId() << Data::endl;
    //Data::cout() << "Data::getTolerance() = " << Data::getTolerance() << Data::endl;
    //Data::cout() << "this->dimension = " << this->dimension << Data::endl;
    //Data::cout() << "this->removeLayersInLoad = " << this->removeLayersInLoad << Data::endl;
#if USE_REMOVED_LAYERS_PROPORTIONS
    //Data::cout() << "this->removedLayersProportions = " << this->removedLayersProportions.size() << ":";
    //for (size_t i = 0; i < this->removedLayersProportions.size(); i++) Data::cout() << " " << this->removedLayersProportions[i];
    //Data::cout() << Data::endl;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    //Data::cout() << "this->proportional = " << std::boolalpha << this->proportional << Data::endl;
    //Data::cout() << "this->creatingInnerFrontInDecomposition = " << std::boolalpha << this->creatingInnerFrontInDecomposition << Data::endl;
    //Data::cout() << "total load = " << (*this->loadCalculator->getTreeLoad().find(0)).second << Data::endl;
    //debug(cell, this->loads);

    LoadSplit spl[this->dimension];
    Real difs[this->dimension];
    UInt countBadAngles[this->dimension];

    for (UInt dir = 0; dir < this->dimension; dir++)
    {
        spl[dir] = this->findBestSubdivision(cell, (*p.first).second.cells, (*p.first).second.layers, dir);

        if (spl[dir].direction == this->dimension)
        {
            difs[dir] = -1.0;
        }
        else if (this->proportional)
        {
            Real l0 = spl[dir].load[0];
            Real l1 = spl[dir].load[1];

            this->adjustProportion(l0, l1);

            difs[dir] = std::fabs(l0 - l1);

            if (difs[dir] <= Data::getTolerance())
            {
                difs[dir] = 0.0;
            }
        }
        else
        {
            difs[dir] = std::fabs(spl[dir].load[0] - spl[dir].load[1]);

            if (difs[dir] <= Data::getTolerance())
            {
                difs[dir] = 0.0;
            }
        }

        countBadAngles[dir] = UINT_MAX;

        //Data::cout() << "load split for direction " << dir << " = " << spl[dir].load[0] << ", " << spl[dir].load[1] << ", " << spl[dir].load[2] << Data::endl;
        //Data::cout() << "\tsum = " << spl[dir].load[0] + spl[dir].load[1] + spl[dir].load[2] << Data::endl;
    }

    if (this->usingFrontTest)
    {
        UInt maxBadAngle = 0;
        //Real badAngleCos = 0.95;
        UInt countBadAngleOriginal = 0;

        ///Calcular o delta, e verificar se eh += ou -=;
        //Real delta =  2.0 * this->boxTree->size() / (std::pow(2.0, this->boxTree->depth()));
        Real delta = this->interfaceShiftDelta * this->boxTree->size() / (std::pow(2.0, this->boxTree->depth()));

        //std::cout << "InterfaceBadAngleCos: " << this->interfaceBadAngleCos << " -- InterfaceShiftDelta: " << this->interfaceShiftDelta << std::endl;

        for (UInt dir = 0; dir < this->dimension; dir++)
        {
            if (spl[dir].direction == this->dimension)
            {
                continue;
            }

            Real op = 1.0;
            UInt vezes = 0;
            Real coord_ori = spl[dir].coord;

            while (true)
            {
                Data::Point3D min(cell->shape()->getPoint(0)), max(cell->shape()->getPoint(1));

                min.setCoord(spl[dir].direction, spl[dir].coord - Data::getTolerance());
                max.setCoord(spl[dir].direction, spl[dir].coord + Data::getTolerance());

                Data::Box box(min, max);

                Data::IdentifiableList fels = this->searchTree->inRange(box);

                Data::Vector3D v3d;
                v3d.setCoord(spl[dir].direction, 1.0);

                UInt countBadAngle = 0;

                for (Data::IdentifiableList::iterator iter = fels.begin(); iter != fels.end(); iter++)
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement3D *>((*iter));

                    //std::cout << "direction " << spl[dir].direction << " testing bad angle with fel " << fel->getId() << " (1)" << std::endl;

                    Data::Triangle3D *triangle = static_cast<Data::Triangle3D *>(fel->getShape());

                    Data::Vector3D normal = triangle->normal();

                    Real cos = normal.cosAngle(v3d);

                    if (std::fabs(cos) > this->interfaceBadAngleCos)
                    {
                        //std::cout << "bad angle: dir " << spl[dir].direction << "; coord " << spl[dir].coord << "; fel " << fel->getId() << "; cosangle " << std::fabs(cos) << " (1)" << std::endl;

                        countBadAngle++;
                    }

                    if (countBadAngle > maxBadAngle)
                    {
                        break;
                    }
                }

                //std::cout << "countBadAngle: " << countBadAngle << std::endl;

                if (vezes == 0)
                {
                    countBadAngleOriginal = countBadAngle;
                }

                if (countBadAngle <= maxBadAngle)
                {
                    //std::cout << "- - - - (1) Numero de tentativas para o plano ideal: " << vezes << std::endl;

                    countBadAngles[dir] = countBadAngle;

                    if (vezes != 0)
                    {
                        ///TODO recalcular a carga nos filhos
                        Params params;

                        params.id = cell->getId();
                        params.dir = spl[dir].direction;
                        params.coord = spl[dir].coord;

                        params.limits[0] = cell->box()->getMin()->getCoord(params.dir);
                        params.limits[1] = cell->box()->getMax()->getCoord(params.dir);

                        params.cells = &(*p.first).second.cells;

                        if (this->removeLayersInLoad > 0)
                        {
                            params.layers = (*p.first).second.layers;

                            params.layersLimits[0] = params.limits[0];
                            params.layersLimits[1] = params.limits[1];
                        }

                        this->splitRec(*params.cells, params.dir, params.coord, params.splittedCells[0], params.splittedCells[1], true);

                        this->splitLayers(params);

                        for (UInt i = 0; i < 2; i++)
                        {
                            for (Data::GraphNodeList::iterator iter = params.splittedCells[i].begin();
                                 iter != params.splittedCells[i].end(); iter++)
                            {
                                params.splittedLoads[i] += (*this->loadCalculator->getTreeLoad().find((*iter)->getId())).second;
                            }
                        }

                        this->removeLoadFromLayers(params);

                        this->move(params, 0);
                        this->move(params, 1);

                        spl[dir].load[0] = params.fixedLoads[0] - params.layersLoads[0];
                        spl[dir].load[1] = params.fixedLoads[1] - params.layersLoads[1];
                        this->adjustLoadsForLayers(spl[dir].load[0], spl[dir].load[1]);
                        spl[dir].load[2] = params.splittingLoad;

                        ULInt id1 = 2*this->dimension*cell->getId() + 2*spl[dir].direction + 1;
                        ULInt id2 = id1 + 1;

                        CellLoadMap::iterator it1 = this->loads.find(id1);
                        CellLoadMap::iterator it2 = this->loads.find(id2);

                        (*it1).second.cells.swap(params.fixedCells[0]);
#if USE_ARRAY_ON_PARAMS
                        (*it1).second.layers = params.fixedLayers[0];
#else
                        (*it1).second.layers.swap(params.fixedLayers[0]);
#endif //#if USE_ARRAY_ON_PARAMS
                        (*it1).second.load = spl[dir].load[0];

                        (*it2).second.cells.swap(params.fixedCells[1]);
#if USE_ARRAY_ON_PARAMS
                        (*it2).second.layers = params.fixedLayers[1];
#else
                        (*it2).second.layers.swap(params.fixedLayers[1]);
#endif //#if USE_ARRAY_ON_PARAMS
                        (*it2).second.load = spl[dir].load[1];

#if USE_ARRAY_ON_PARAMS
                        params.clear(this->removeLayersInLoad, false, true, false);
#endif //#if USE_ARRAY_ON_PARAMS

                        if (this->proportional)
                        {
                            Real l0 = spl[dir].load[0];
                            Real l1 = spl[dir].load[1];

                            this->adjustProportion(l0, l1);

                            difs[dir] = std::fabs(l0 - l1);

                            if (difs[dir] <= Data::getTolerance())
                            {
                                difs[dir] = 0.0;
                            }
                        }
                        else
                        {
                            difs[dir] = std::fabs(spl[dir].load[0] - spl[dir].load[1]);

                            if (difs[dir] <= Data::getTolerance())
                            {
                                difs[dir] = 0.0;
                            }
                        }
                    }

                    //std::cout << "- - - - Tentativas: " << vezes << " -- Dif carga: " << abs(spl[dir].load[0] - spl[dir].load[1]) << " -- Dir: " << dir << " -- Inter " << dir << ": " << spl[dir].load[2] << std::endl;

                    break;
                }

                vezes++;

                if (vezes > 20)
                {
                    //std::cout << "(1) Não achou plano ideal" << std::endl;
                    spl[dir].coord = coord_ori;
                    countBadAngles[dir] = countBadAngleOriginal;
                    break;
                }

                op *= -1.0;
                spl[dir].coord += delta * vezes * op;
            }
        }

        UInt countAxesBadAngles = 0;

        for (UInt dir = 0; dir < this->dimension; dir++)
        {
            if (countBadAngles[dir] > maxBadAngle)
            {
                countAxesBadAngles++;
            }
        }

        if ((countAxesBadAngles > 0) && (countAxesBadAngles < this->dimension))
        {
            for (UInt dir = 0; dir < this->dimension; dir++)
            {
                if (countBadAngles[dir] > maxBadAngle)
                {
                    spl[dir].direction = this->dimension;
                }
            }
        }
    }

    //Data::cout() << "will now determine best direction for splitting" << Data::endl;

    UInt best = this->dimension;

    for (UInt dir = 0; dir < this->dimension; dir++)
    {
        //Data::cout() << "testing direction " << dir << Data::endl;
        //Data::cout() << "\tdirection: " << spl[dir].direction << ", coord = " << spl[dir].coord << Data::endl;

        //Data::cout().getBuffer().precision(6);
        //Data::cout().getBuffer().setf(std::ios::fixed, std::ios::floatfield);
        //Data::cout() << "\tloads: " << spl[dir].load[0] << ", " << spl[dir].load[1] << ", " << spl[dir].load[2] << Data::endl;

        //could not split in that direction
        if (spl[dir].direction == this->dimension)
        {
            //Data::cout() << "could not split in direction " << dir << Data::endl;

            continue;
        }

        //best is not set yet
        if (best == this->dimension)
        {
            best = dir;

            //Data::cout() << "best direction is not set yet, so it will now be " << dir << Data::endl;

            continue;
        }

        if ((this->loadThreshold > 0.0) && (spl[dir].load[2] > 0.0) && (spl[best].load[2] > 0.0))
        {
            /*Real l0 = spl[dir].load[0];
            Real l1 = spl[dir].load[1];

            this->adjustProportion(l0, l1);

            Real maxd = std::max(l0, l1);

            l0 = spl[best].load[0];
            l1 = spl[best].load[1];

            this->adjustProportion(l0, l1);

            Real maxb = std::max(l0, l1);

            if ((difs[best]/maxb < this->loadThreshold) && (difs[dir]/maxd < this->loadThreshold))*/
            {
                Real proportion = spl[dir].load[2]/spl[best].load[2];

                Real l0 = spl[dir].load[0];
                Real l1 = spl[dir].load[1];

                this->adjustProportion(l0, l1);

                Real maxd = std::max(l0, l1);

                l0 = spl[best].load[0];
                l1 = spl[best].load[1];

                this->adjustProportion(l0, l1);

                Real maxb = std::max(l0, l1);

                bool okd = ((difs[dir]/maxd <= this->loadThreshold) && (proportion <= this->loadThreshold));
                bool okb = ((difs[best]/maxb <= this->loadThreshold) && (1.0/proportion <= this->loadThreshold));

                //if (proportion < this->loadThreshold)
                //if ((difs[dir]/maxd <= this->loadThreshold) && (proportion <= this->loadThreshold))
                if ((okd) && (!okb))
                {
                    //Data::cout() << "direction " << dir << " has inner load much less than load of direction " << best << Data::endl;

                    spl[best].direction = this->dimension;
                    best = dir;

                    continue;
                }

                /*l0 = spl[best].load[0];
                l1 = spl[best].load[1];

                this->adjustProportion(l0, l1);

                max = std::max(l0, l1);*/

                //if (1.0/proportion < this->loadThreshold)
                //if ((difs[best]/maxb <= this->loadThreshold) && (1.0/proportion <= this->loadThreshold))
                if ((okb) && (!okd))
                {
                    spl[dir].direction = this->dimension;

                    //Data::cout() << "direction " << dir << " is has inner load much greater than load of direction " << best << Data::endl;

                    continue;
                }
            }
        }

        //the load difference is smaller
        if (this->better(difs[dir], difs[best]))
        {
            //Data::cout() << "direction " << dir << " has load better balanced than previous direction " << best << Data::endl;

            spl[best].direction = this->dimension;
            best = dir;

            continue;
        }

        //the load difference is greater
        if (this->better(difs[best], difs[dir]))
        {
            spl[dir].direction = this->dimension;

            //Data::cout() << "direction " << dir << " has load worse balanced than current direction " << best << Data::endl;

            continue;
        }

        //the load difference is the same

        Real diff = spl[dir].load[2] - spl[best].load[2];

        if (std::fabs(diff) <= Data::getTolerance())
        {
            diff = 0.0;
        }

        //the load for splitting is smaller
        if (diff < 0.0)
        {
            //Data::cout() << "direction " << dir << " has smaller inner load than previous direction " << best << Data::endl;

            spl[best].direction = this->dimension;
            best = dir;

            continue;
        }

        //the load for splitting is greater
        if (diff > 0.0)
        {
            spl[dir].direction = this->dimension;

            //Data::cout() << "direction " << dir << " has greater inner load than current direction " << best << Data::endl;

            continue;
        }

        //the load for splitting is the same

        /*Real sizeBest = 1.0;
        Real sizeDir = 1.0;

        for (UInt i = 0; i < this->dimension; i++)
        {
            if (i != spl[dir].direction)
            {
                sizeDir *= cell->box()->getMax()->getCoord(i) - cell->box()->getMin()->getCoord(i)
            }

            if (i != spl[best].direction)
            {
                sizeBest *= cell->box()->getMax()->getCoord(i) - cell->box()->getMin()->getCoord(i)
            }
        }

        //the size (length/area) of the splitting line/plane is smaller
        if (sizeDir < sizeBest)
        {
            spl[best].direction = this->dimension;
            best = dir;

            continue;
        }

        //the size (length/area) of the splitting line/plane is larger
        if (sizeBest < sizeDir)
        {
            spl[dir].direction = this->dimension;

            continue;
        }*/
    }

    UInt count = 0;

    for (UInt dir = 0; dir < this->dimension; dir++)
    {
        if (spl[dir].direction != this->dimension)
        {
            count++;
        }
    }

    //best splitting direction could not be found
    if ((count == 0) ||
        (best == this->dimension))
    {
        Split split;

        split.direction = this->dimension;
        split.coord = 0.0;

        this->innerLoads[cell->getId()] = 0.0;

        //Data::cout() << "best direction NOT found " << Data::endl;

        return split;
    }

    //more than one suitable splitting direction could be found
    if ((count > 1) &&
        (!cell->isRoot()))
    {
        //pick the next direction available
        UInt splDir = static_cast<PartitionBSPTreeCell *>(cell->parent())->getSplitDirection();

        for (UInt i = 0; i < this->dimension; i++)
        {
            UInt dir = (i + splDir + 1)%this->dimension;

            if (spl[dir].direction != this->dimension)
            {
                best = dir;

                //Data::cout() << "picking next direction available: " << dir << Data::endl;

                break;
            }
        }
    }

    Split split;

    split.direction = spl[best].direction;
    split.coord = spl[best].coord;

    this->innerLoads[cell->getId()] = spl[best].load[2];

    //Data::cout() << "best: " << best << Data::endl;
    //Data::cout() << "best direction found: " << split.direction << Data::endl;
    //Data::cout() << "best coord found: " << split.coord << Data::endl;
    //Data::cout() << "innerLoads: " << spl[best].load[2] << Data::endl;
    Data::cout() << "# # # # best direction found: " << split.direction << " -- Dif carga: " << abs(spl[best].load[0] - spl[best].load[1]) << " InnerLoads: " << spl[best].load[2] << Data::endl;

    return split;
}

PMJMesh2::LoadBSPTreeGenerator::LoadSplit PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision(
#if USE_ARRAY_ON_PARAMS
    PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **layers, UInt dir) const
#else
    PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers, UInt dir) const
#endif //#if USE_ARRAY_ON_PARAMS
{
    LoadSplit spl;

    //Data::cout() << "will find best subdivision for BSP cell " << cell->getId() << " in direction " << dir << Data::endl;

    spl.direction = dir;
    //spl.coord = 0.5*(cell->box()->getMin()->getCoord(dir) + cell->box()->getMax()->getCoord(dir));
    spl.coord = this->initialSplitCoord(cell->box()->getMin()->getCoord(dir), cell->box()->getMax()->getCoord(dir), dir, cells);

    //Data::cout() << "initial limits are " << cell->box()->getMin()->getCoord(dir) << ", " << cell->box()->getMax()->getCoord(dir) << Data::endl;
    //Data::cout() << "initial coord = " << spl.coord << Data::endl;

    ULInt id1 = 2*this->dimension*cell->getId() + 2*dir + 1;
    ULInt id2 = id1 + 1;

    std::pair<CellLoadMap::iterator, bool> p1 = this->loads.insert(std::make_pair(id1, CellLoad()));
    std::pair<CellLoadMap::iterator, bool> p2 = this->loads.insert(std::make_pair(id2, CellLoad()));

    CellLoadMap::iterator &it1 = p1.first;
    CellLoadMap::iterator &it2 = p2.first;

    if ((p1.second) || (p2.second))
    {
        Data::GraphNodeList cells1, cells2;
#if USE_ARRAY_ON_PARAMS
        PositionCellMap **layers1, **layers2;
#else
        LayerSidePositionCellMap layers1, layers2;
#endif //#if USE_ARRAY_ON_PARAMS

        bool splitted = this->findBestSubdivision(cell, cells, layers, spl, cells1, layers1, cells2, layers2);

        /*//debug
        if (spl.load[0] + spl.load[1] + spl.load[2] == (*this->loads.find(cell->getId())).second.load)
        {
            Data::cout() << "sum of loads is equal: "
                << spl.load[0] << " + " << spl.load[1] << " + " << spl.load[2] << " = "
                << spl.load[0] + spl.load[1] + spl.load[2] << " == "
                << (*this->loads.find(cell->getId())).second.load
                << "\n in direction " << dir << " of cell " << cell->getId()
                << Data::endl;
        }
        else
        {
            Data::cout() << "Error: sum of loads is different: "
                << spl.load[0] << " + " << spl.load[1] << " + " << spl.load[2] << " = "
                << spl.load[0] + spl.load[1] + spl.load[2] << " != "
                << (*this->loads.find(cell->getId())).second.load
                << "\n in direction " << dir << " of cell " << cell->getId()
                << Data::endl;
        }
        //endebug*/

        if (!splitted)
        {
            spl.direction = this->dimension;
            (*it1).second.load = -1.0;
            (*it2).second.load = -1.0;
#if USE_ARRAY_ON_PARAMS
            (*it1).second.layers = NULL;
            (*it2).second.layers = NULL;
#endif //#if USE_ARRAY_ON_PARAMS
        }
        else
        {
            if (p1.second)
            {
                (*it1).second.cells.swap(cells1);
#if USE_ARRAY_ON_PARAMS
                (*it1).second.layers = layers1;
#else
                (*it1).second.layers.swap(layers1);
#endif //#if USE_ARRAY_ON_PARAMS
                (*it1).second.load = spl.load[0];
            }

            if (p2.second)
            {
                (*it2).second.cells.swap(cells2);
#if USE_ARRAY_ON_PARAMS
                (*it2).second.layers = layers2;
#else
                (*it2).second.layers.swap(layers2);
#endif //#if USE_ARRAY_ON_PARAMS
                (*it2).second.load = spl.load[1];
            }
        }
    }

    return spl;
}

#if USE_ARRAY_ON_PARAMS
bool PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells,
    PositionCellMap **layers, LoadSplit &spl, Data::GraphNodeList &cells1, PositionCellMap **&layers1,
    Data::GraphNodeList &cells2, PositionCellMap **&layers2) const
#else
bool PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells,
    const LayerSidePositionCellMap &layers, LoadSplit &spl, Data::GraphNodeList &cells1, LayerSidePositionCellMap &layers1,
    Data::GraphNodeList &cells2, LayerSidePositionCellMap &layers2) const
#endif //#if USE_ARRAY_ON_PARAMS
{
    //Data::cout() << "will find best subdivision for cell " << cell->getId() << " in direction " << spl.direction << Data::endl;

    Params p;

    p.id = cell->getId();
    p.dir = spl.direction;
    p.coord = spl.coord;

    p.limits[0] = cell->box()->getMin()->getCoord(p.dir);
    p.limits[1] = cell->box()->getMax()->getCoord(p.dir);

    p.cells = &cells;

    if (this->removeLayersInLoad > 0)
    {
        //p.layers = (*this->layers.find(cell->getId())).second;
        p.layers = layers;

        p.layersLimits[0] = p.limits[0];
        p.layersLimits[1] = p.limits[1];

        /*p.splittedLayers[0].resize(this->removeLayersInLoad);
        p.splittedLayers[1].resize(this->removeLayersInLoad);

        p.fixedLayers[0].resize(this->removeLayersInLoad);
        p.fixedLayers[1].resize(this->removeLayersInLoad);

        UInt numSides = 2*this->dimension;

        for (UInt i = 0; i < this->removeLayersInLoad; i++)
        {
            p.splittedLayers[0].resize(numSides);
            p.splittedLayers[1].resize(numSides);

            p.fixedLayers[0].resize(numSides);
            p.fixedLayers[1].resize(numSides);
        }

        UInt nside = p.dir;
        UInt oside = (this->dimension + nside)%(numSides);

        for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
        {
            p.fixedCells[0][layer][oside][Data::ON].swap(p.layers[layer][oside][Data::ON]);
            p.fixedCells[0][layer][oside][Data::IN].swap(p.layers[layer][oside][Data::IN]);
            p.fixedCells[1][layer][nside][Data::ON].swap(p.layers[layer][nside][Data::ON]);
            p.fixedCells[1][layer][nside][Data::IN].swap(p.layers[layer][nside][Data::IN]);
        }*/
    }

    bool splitted = this->findBestSubdivision(p);

    if (!splitted)
    {
        spl.direction = this->dimension;

        spl.load[0] = spl.load[1] = -1.0;
        spl.load[2] = p.splittingLoad;

        return false;
    }

    /*if (this->removeLayersInLoad > 0)
    {
        ULInt id1 = 2*this->dimension*cell->getId() + 2*p.dir + 1;
        ULInt id2 = id1 + 1;

        this->layers[id1].swap(p.fixedLayers[0]);
        this->layers[id2].swap(p.fixedLayers[1]);
    }*/

    spl.coord = p.coord;
    //spl.load[0] = p.fixedLoads[0];
    //spl.load[1] = p.fixedLoads[1];
    spl.load[0] = p.fixedLoads[0] - p.layersLoads[0];
    spl.load[1] = p.fixedLoads[1] - p.layersLoads[1];
    this->adjustLoadsForLayers(spl.load[0], spl.load[1]);
    spl.load[2] = p.splittingLoad;

    cells1.swap(p.fixedCells[0]);
    cells2.swap(p.fixedCells[1]);

#if USE_ARRAY_ON_PARAMS
    layers1 = p.fixedLayers[0];
    layers2 = p.fixedLayers[1];

    p.fixedLayers[0] = NULL;
    p.fixedLayers[1] = NULL;

    p.layers = NULL;

    p.clear(this->removeLayersInLoad, false, true, false);
#else
    layers1.swap(p.fixedLayers[0]);
    layers2.swap(p.fixedLayers[1]);
#endif //#if USE_ARRAY_ON_PARAMS

    return true;
}

bool PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision(Params &params) const
{
    /*struct Print
    {
        static void print(Params &params, UInt layers, UInt sides)
        {
            Data::cout() << "Params:" << Data::endl;
            Data::cout() << "\tdir = " << params.dir << Data::endl;
            Data::cout() << "\tcoord = " << params.coord << Data::endl;
            Data::cout() << "\tlimits = " << params.limits[0] << ", " << params.limits[1] << Data::endl;
            Data::cout() << "\tnum of cells = " << params.cells->size() << Data::endl;
            Data::cout() << "\tfixedLoads = " << params.fixedLoads[0] << ", " << params.fixedLoads[1] << Data::endl;
            Data::cout() << "\tnum of fixedCells[0] = " << params.fixedCells[0].size() << Data::endl;
            Data::cout() << "\tnum of fixedCells[1] = " << params.fixedCells[1].size() << Data::endl;
            Data::cout() << "\tsplittedLoads = " << params.splittedLoads[0] << ", " << params.splittedLoads[1] << Data::endl;
            Data::cout() << "\tnum of splittedCells[0] = " << params.splittedCells[0].size() << Data::endl;
            Data::cout() << "\tnum of splittedCells[1] = " << params.splittedCells[1].size() << Data::endl;
            Data::cout() << "\tequal = " << std::boolalpha << params.equal << Data::endl;
            Data::cout() << "\tsplittingLoad = " << params.splittingLoad << Data::endl;
            Data::cout() << "\tnum of splittingCells = " << params.splittingCells.size() << Data::endl;
            Data::cout() << "\tlayerLimits = " << params.layersLimits[0] << ", " << params.layersLimits[1] << Data::endl;
            Data::cout() << "\tlayerLoads = " << params.layersLoads[0] << ", " << params.layersLoads[1] << Data::endl;
            for (UInt layer = 0; layer < layers; layer++)
            {
                for (UInt side = 0; side < sides; side++)
                {
                    Data::cout() << "\tnum of cells in layers in layer " << layer
                        << " and in side " << side << " as IN = "
                        << params.layers[layer][side][Data::IN].size() << Data::endl;
                    Data::cout() << "\tnum of cells in layers in layer " << layer
                        << " and in side " << side << " as ON = "
                        << params.layers[layer][side][Data::ON].size() << Data::endl;
                    Data::cout() << "\tnum of cells in layers in layer " << layer
                        << " and in side " << side << " as OUT = "
                        << params.layers[layer][side][Data::OUT].size() << Data::endl;
                    Data::cout() << "\tnum of cells in layers in layer " << layer
                        << " and in side " << side << " as UNK = "
                        << params.layers[layer][side][Data::UNKNOWN].size() << Data::endl;
                }
            }
#if USE_ARRAY_ON_PARAMS
            if (params.splittedLayers[0])
#else
            if (!params.splittedLayers[0].empty())
#endif //#if USE_ARRAY_ON_PARAMS
            {
                for (UInt layer = 0; layer < layers; layer++)
                {
                    for (UInt side = 0; side < sides; side++)
                    {
                        Data::cout() << "\tnum of cells in splittedLayers in layer " << layer
                            << " and in side " << side << " as IN = "
                            << params.splittedLayers[0][layer][side][Data::IN].size() << ", " << params.splittedLayers[1][layer][side][Data::IN].size() << Data::endl;
                        Data::cout() << "\tnum of cells in splittedLayers in layer " << layer
                            << " and in side " << side << " as ON = "
                            << params.splittedLayers[0][layer][side][Data::ON].size() << ", " << params.splittedLayers[1][layer][side][Data::ON].size() << Data::endl;
                        Data::cout() << "\tnum of cells in splittedLayers in layer " << layer
                            << " and in side " << side << " as OUT = "
                            << params.splittedLayers[0][layer][side][Data::OUT].size() << ", " << params.splittedLayers[1][layer][side][Data::OUT].size() << Data::endl;
                        Data::cout() << "\tnum of cells in splittedLayers in layer " << layer
                            << " and in side " << side << " as UNK = "
                            << params.splittedLayers[0][layer][side][Data::UNKNOWN].size() << ", " << params.splittedLayers[1][layer][side][Data::UNKNOWN].size() << Data::endl;
                    }
                }
            }
#if USE_ARRAY_ON_PARAMS
            if (params.fixedLayers[0])
#else
            if (!params.fixedLayers[0].empty())
#endif //#if USE_ARRAY_ON_PARAMS
            {
                for (UInt layer = 0; layer < layers; layer++)
                {
                    for (UInt side = 0; side < sides; side++)
                    {
                        Data::cout() << "\tnum of cells in fixedLayers in layer " << layer
                            << " and in side " << side << " as IN = "
                            << params.fixedLayers[0][layer][side][Data::IN].size() << ", " << params.fixedLayers[1][layer][side][Data::IN].size() << Data::endl;
                        Data::cout() << "\tnum of cells in fixedLayers in layer " << layer
                            << " and in side " << side << " as ON = "
                            << params.fixedLayers[0][layer][side][Data::ON].size() << ", " << params.fixedLayers[1][layer][side][Data::ON].size() << Data::endl;
                        Data::cout() << "\tnum of cells in fixedLayers in layer " << layer
                            << " and in side " << side << " as OUT = "
                            << params.fixedLayers[0][layer][side][Data::OUT].size() << ", " << params.fixedLayers[1][layer][side][Data::OUT].size() << Data::endl;
                        Data::cout() << "\tnum of cells in fixedLayers in layer " << layer
                            << " and in side " << side << " as UNK = "
                            << params.fixedLayers[0][layer][side][Data::UNKNOWN].size() << ", " << params.fixedLayers[1][layer][side][Data::UNKNOWN].size() << Data::endl;
                    }
                }
            }
        }
    };*/

    //Data::cout() << "will recursively find best subdivision" << Data::endl;
    //Data::cout() << "\tid = " << params.id << Data::endl;
    //Data::cout() << "\tdir = " << params.dir << Data::endl;
    //Data::cout() << "\tcoord = " << params.coord << Data::endl;
    //Data::cout() << "\tlimits = " << params.limits[0] << ", " << params.limits[1] << Data::endl;
    //Print::print(params, this->removeLayersInLoad, 2*this->dimension);

    bool splitted = this->split(*params.cells, params.dir, params.coord, params.splittedCells[0], params.splittedCells[1], true);

    if (!splitted)
    {
        //Data::cout() << "could NOT split" << Data::endl;

        return false;
    }

    /*if ((params.splittedCells[0].size() == params.cells->size()) || (params.splittedCells[1].size() == params.cells->size()))
    {
        Data::cout() << "could NOT split (equal sizes)" << Data::endl;

        return false;
    }*/

    this->splitLayers(params);

    for (UInt i = 0; i < 2; i++)
    {
        for (Data::GraphNodeList::iterator iter = params.splittedCells[i].begin();
             iter != params.splittedCells[i].end(); iter++)
        {
            params.splittedLoads[i] += (*this->loadCalculator->getTreeLoad().find((*iter)->getId())).second;
        }
    }

    this->removeLoadFromLayers(params);

    //Data::cout() << "could split and removed layers" << Data::endl;
    //Print::print(params, this->removeLayersInLoad, 2*this->dimension);

    /*Data::cout() << "splittedLoads = " << params.splittedLoads[0] << ", " << params.splittedLoads[1] << Data::endl;
    Data::cout() << "sum splittedLoads = " << params.splittedLoads[0] + params.splittedLoads[1] << Data::endl;
    Data::cout() << "sum splittedLoads and fixedLoads = " << params.splittedLoads[0] + params.splittedLoads[1] + params.fixedLoads[0] + params.fixedLoads[1] << Data::endl;*/
    //Data::cout() << "loads[0] = " << params.splittedLoads[0] + params.fixedLoads[0] << Data::endl;
    //Data::cout() << "loads[1] = " << params.splittedLoads[1] + params.fixedLoads[1] << Data::endl;
    //Data::cout() << "loads[2] = " << params.splittingLoad << Data::endl;

    if (this->equal(params))
    {
        this->move(params, 0);
        this->move(params, 1);
        params.equal = true;

        //Data::cout() << "equal loads" << Data::endl;
        //Data::cout() << "loads[0] = " << params.splittedLoads[0] + params.fixedLoads[0] << Data::endl;
        //Data::cout() << "loads[1] = " << params.splittedLoads[1] + params.fixedLoads[1] << Data::endl;
        //Data::cout() << "loads[2] = " << params.splittingLoad << Data::endl;

        //Data::cout() << "equal loads" << Data::endl;
        //Print::print(params, this->removeLayersInLoad, 2*this->dimension);

        return true;
    }

    UInt heavier = this->heavier(params);

    //Data::cout() << "heavier child = " << heavier << Data::endl;

    Params prec;

    prec.id = params.id;
    prec.dir = params.dir;
    //prec.coord = 0.5*(params.limits[heavier] + params.coord);
    //prec.coord = this->initialSplitCoord(params.limits[heavier], params.coord, params.dir, params.splittedCells[heavier]);

    prec.limits[heavier] = params.limits[heavier];
    prec.limits[1 - heavier] = params.coord;
    prec.cells = &params.splittedCells[heavier];

    prec.coord = this->initialSplitCoord(prec.limits[0], prec.limits[1], prec.dir, prec.cells);

    prec.splittedCells[1-heavier] = params.splittedCells[1-heavier];
    prec.splittedLoads[1-heavier] = params.splittedLoads[1-heavier];

    prec.fixedCells[0] = params.fixedCells[0];
    prec.fixedCells[1] = params.fixedCells[1];

    prec.fixedLoads[0] = params.fixedLoads[0];
    prec.fixedLoads[1] = params.fixedLoads[1];

    if (this->removeLayersInLoad > 0)
    {
        prec.layersLimits[0] = params.layersLimits[0];
        prec.layersLimits[1] = params.layersLimits[1];

        UInt numSides = 2*this->dimension;

#if USE_ARRAY_ON_PARAMS
        prec.resize(this->removeLayersInLoad, numSides, true, true, true);

        for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
        {
            for (UInt side = 0; side < numSides; side++)
            {
                prec.fixedLayers[0][layer][side] = params.fixedLayers[0][layer][side];
                prec.fixedLayers[1][layer][side] = params.fixedLayers[1][layer][side];
            }
        }
#else
        prec.fixedLayers[0] = params.fixedLayers[0];
        prec.fixedLayers[1] = params.fixedLayers[1];

        prec.layers.resize(this->removeLayersInLoad);
        prec.splittedLayers[0].resize(this->removeLayersInLoad);
        prec.splittedLayers[1].resize(this->removeLayersInLoad);
#endif //#if USE_ARRAY_ON_PARAMS

        UInt nside = params.dir;
        UInt oside = (params.dir + this->dimension)%(2*this->dimension);

        for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
        {
#if (!USE_ARRAY_ON_PARAMS)
            prec.layers[layer].resize(numSides);
            prec.splittedLayers[0][layer].resize(numSides);
            prec.splittedLayers[1][layer].resize(numSides);
#endif //#if USE_ARRAY_ON_PARAMS

            prec.layers[layer][oside][Data::ON].swap(prec.fixedLayers[0][layer][oside][Data::ON]);
            prec.layers[layer][oside][Data::IN].swap(prec.fixedLayers[0][layer][oside][Data::IN]);

            prec.layers[layer][nside][Data::ON].swap(prec.fixedLayers[1][layer][nside][Data::ON]);
            prec.layers[layer][nside][Data::IN].swap(prec.fixedLayers[1][layer][nside][Data::IN]);

            for (UInt side = 0; side < numSides; side++)
            {
                if ((side == nside) || (side == oside))
                {
                    continue;
                }

                prec.layers[layer][side] = params.splittedLayers[heavier][layer][side];
                prec.splittedLayers[1-heavier][layer][side] = params.splittedLayers[1-heavier][layer][side];
            }
        }
    }

    this->move(prec, 1-heavier);

    //Data::cout() << "created prec" << Data::endl;

    splitted = this->findBestSubdivision(prec);

    if (!splitted)
    {
        this->move(params, 0);
        this->move(params, 1);

#if USE_ARRAY_ON_PARAMS
        prec.clear(this->removeLayersInLoad, true, true, true);
#endif //#if USE_ARRAY_ON_PARAMS

        //Data::cout() << "could NOT split heavier child" << Data::endl;
        //Data::cout() << "loads[0] = " << params.splittedLoads[0] + params.fixedLoads[0] << Data::endl;
        //Data::cout() << "loads[1] = " << params.splittedLoads[1] + params.fixedLoads[1] << Data::endl;
        //Data::cout() << "loads[2] = " << params.splittingLoad << Data::endl;
        //Print::print(params, this->removeLayersInLoad, 2*this->dimension);

        return true;
    }

    /*Real lp0 = params.splittedLoads[0] + params.fixedLoads[0];
    Real lp1 = params.splittedLoads[1] + params.fixedLoads[1];

    Real lr0 = prec.splittedLoads[0] + prec.fixedLoads[0];
    Real lr1 = prec.splittedLoads[1] + prec.fixedLoads[1];

    if ((prec.equal) ||
        (this->better(std::fabs(lr0 - lr1), std::fabs(lp0 - lp1))))*/
    if ((prec.equal) ||
        (this->better(prec, params)))
    {
        //this->move(prec, 0);
        //this->move(prec, 1);

        //Data::cout() << "children splitting is better" << Data::endl;
        //Data::cout() << "children loads[0] = " << prec.splittedLoads[0] + prec.fixedLoads[0] << Data::endl;
        //Data::cout() << "children loads[1] = " << prec.splittedLoads[1] + prec.fixedLoads[1] << Data::endl;
        //Data::cout() << "children loads[2] = " << prec.splittingLoad << Data::endl;
        //Data::cout() << "loads[0] = " << params.splittedLoads[0] + params.fixedLoads[0] << Data::endl;
        //Data::cout() << "loads[1] = " << params.splittedLoads[1] + params.fixedLoads[1] << Data::endl;
        //Data::cout() << "loads[2] = " << params.splittingLoad << Data::endl;

        params.coord = prec.coord;

        params.splittedLoads[0] = prec.splittedLoads[0];
        params.splittedLoads[1] = prec.splittedLoads[1];

        params.splittedCells[0].swap(prec.splittedCells[0]);
        params.splittedCells[1].swap(prec.splittedCells[1]);

        params.fixedLoads[0] = prec.fixedLoads[0];
        params.fixedLoads[1] = prec.fixedLoads[1];

        params.fixedCells[0].swap(prec.fixedCells[0]);
        params.fixedCells[1].swap(prec.fixedCells[1]);

        params.equal = prec.equal;
        params.splittingLoad = prec.splittingLoad;

        params.splittingCells.swap(prec.splittingCells);

        if (this->removeLayersInLoad > 0)
        {
#if USE_ARRAY_ON_PARAMS
            //Data::cout() << "swapping splittedLayers[0] at " << params.splittedLayers[0] << " with " << prec.splittedLayers[0] << Data::endl;
            //Data::cout() << "swapping splittedLayers[1] at " << params.splittedLayers[1] << " with " << prec.splittedLayers[1] << Data::endl;
            //Data::cout() << "swapping fixedLayers[0] at " << params.fixedLayers[0] << " with " << prec.fixedLayers[0] << Data::endl;
            //Data::cout() << "swapping fixedLayers[1] at " << params.fixedLayers[1] << " with " << prec.fixedLayers[1] << Data::endl;

            params.clear(this->removeLayersInLoad, false, true, true);

            params.splittedLayers[0] = prec.splittedLayers[0];
            params.splittedLayers[1] = prec.splittedLayers[1];

            params.fixedLayers[0] = prec.fixedLayers[0];
            params.fixedLayers[1] = prec.fixedLayers[1];

            prec.clear(this->removeLayersInLoad, true, false, false);
#else
            params.splittedLayers[0].swap(prec.splittedLayers[0]);
            params.splittedLayers[1].swap(prec.splittedLayers[1]);

            params.fixedLayers[0].swap(prec.fixedLayers[0]);
            params.fixedLayers[1].swap(prec.fixedLayers[1]);
#endif //#if USE_ARRAY_ON_PARAMS

            params.layersLoads[0] = prec.layersLoads[0];
            params.layersLoads[1] = prec.layersLoads[1];
        }

        //Print::print(params, this->removeLayersInLoad, 2*this->dimension);

        return true;
    }

    this->move(params, 0);
    this->move(params, 1);

#if USE_ARRAY_ON_PARAMS
    prec.clear(this->removeLayersInLoad, true, true, true);
#endif //#if USE_ARRAY_ON_PARAMS

    //Data::cout() << "parent splitting is better" << Data::endl;
    //Data::cout() << "children loads[0] = " << prec.splittedLoads[0] + prec.fixedLoads[0] << Data::endl;
    //Data::cout() << "children loads[1] = " << prec.splittedLoads[1] + prec.fixedLoads[1] << Data::endl;
    //Data::cout() << "children loads[2] = " << prec.splittingLoad << Data::endl;
    //Data::cout() << "loads[0] = " << params.splittedLoads[0] + params.fixedLoads[0] << Data::endl;
    //Data::cout() << "loads[1] = " << params.splittedLoads[1] + params.fixedLoads[1] << Data::endl;
    //Data::cout() << "loads[2] = " << params.splittingLoad << Data::endl;
    //Print::print(params, this->removeLayersInLoad, 2*this->dimension);

    return true;
}

bool PMJMesh2::LoadBSPTreeGenerator::include(const Data::BoxTreeCell *c, UInt side, UInt pt, UInt coord, Real cmp) const
{
    Data::Position pos = PMJMesh::getPosition(this->dimension, c);

    if ((pos == Data::OUT) || (pos == Data::UNKNOWN))
    {
        return false;
    }

    Real value = c->box()->getPoint(pt)->getCoord(coord);

    Real diff = value - cmp;

    //return (side < this->dimension) ? (value >= cmp) : (value <= cmp);
    return (side < this->dimension) ? (diff >= -Data::getTolerance()) : (diff <= Data::getTolerance());
}

void PMJMesh2::LoadBSPTreeGenerator::findInitialLayer(UInt side, const Data::GraphNodeList &cells,
    UInt pt, UInt coord, Real cmp, PositionCellMap &map/*, bool debug*/) const
{
    for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        if (!this->include(c, side, pt, coord, cmp))
        {
            /*if (debug)
            {
                Data::cout() << "debug c = " << c->getId() << ", side = " << side
                    << ", pt = " << pt << ", coord = " << coord << ", cmp = " << cmp
                    << ", pos = " << PMJMesh::getPosition(this->dimension, c)
                    << ", value = " << c->box()->getPoint(pt)->getCoord(coord)
                    << ", include = " << std::boolalpha << ((side < this->dimension) ? (c->box()->getPoint(pt)->getCoord(coord) >= cmp) : (c->box()->getPoint(pt)->getCoord(coord) <= cmp)) << Data::endl;
            }*/

            continue;
        }

        Data::GraphNodeList leaves;

        this->boxTree->children(c, side, leaves);

        while (!leaves.empty())
        {
            c = static_cast<Data::BoxTreeCell *>(leaves.front());

            leaves.pop_front();

            map[PMJMesh::getPosition(this->dimension, c)].insert(c);
        }
    }
}

void PMJMesh2::LoadBSPTreeGenerator::findInitialLayer(const PartitionBSPTreeCell *cell, UInt side,
#if USE_ARRAY_ON_PARAMS
    const Data::GraphNodeList &cells, PositionCellMap **&layers) const
#else
    const Data::GraphNodeList &cells, LayerSidePositionCellMap &layers) const
#endif //#if USE_ARRAY_ON_PARAMS
{
    UInt coord = side%this->dimension;
    UInt pt = 1 - side/this->dimension;
    Real cmp = cell->box()->getPoint(pt)->getCoord(coord);

#if USE_ARRAY_ON_PARAMS
    if (!layers)
    {
        Params::resize(layers, this->removeLayersInLoad, cell->numSides());
    }
#else
    //if (p.second)
    if (layers.size() != this->removeLayersInLoad)
    {
        //(*p.first).second.resize(this->removeLayersInLoad);
        layers.resize(this->removeLayersInLoad);

        UInt numSides = cell->numSides();

        for (UInt i = 0; i < this->removeLayersInLoad; i++)
        {
            //(*p.first).second[i].resize(numSides);
            layers[i].resize(numSides);
        }
    }
#endif //#if USE_ARRAY_ON_PARAMS

    //this->findInitialLayer(side, cells, pt, coord, cmp, (*p.first).second[0][side]);
    this->findInitialLayer(side, cells, pt, coord, cmp, layers[0][side]);

    /*for (Data::GraphNodeList::const_iterator iter = cells.begin();
         iter != cells.end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        if (!this->include(c, side, pt, coord, cmp))
        {
            continue;
        }

        Data::GraphNodeList leaves;

        this->boxTree->children(c, side, leaves);

        while (!leaves.empty())
        {
            c = static_cast<Data::BoxTreeCell *>(leaves.front());

            leaves.pop_front();

            (*p.first).second[0][side][PMJMesh::getPosition(this->dimension, c)].insert(c);
        }
    }*/
}

#if USE_C__11
void PMJMesh2::LoadBSPTreeGenerator::findLayer(const Data::GraphNodeHash &inCellsPrev,
#else
void PMJMesh2::LoadBSPTreeGenerator::findLayer(const Data::GraphNodeSet &inCellsPrev,
#endif //#if USE_C__11
    UInt oside, UInt opt, UInt coord, Real ocmp, PositionCellMap &map) const
{
#if USE_C__11
    for (Data::GraphNodeHash::const_iterator iter = inCellsPrev.begin();
#else
    for (Data::GraphNodeSet::const_iterator iter = inCellsPrev.begin();
#endif //#if USE_C__11
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

#if USE_ARRAY_ON_PARAMS
void PMJMesh2::LoadBSPTreeGenerator::findLayer(const PartitionBSPTreeCell *cell, UInt layer, UInt side, PositionCellMap **&layers) const
#else
void PMJMesh2::LoadBSPTreeGenerator::findLayer(const PartitionBSPTreeCell *cell, UInt layer, UInt side, LayerSidePositionCellMap &layers) const
#endif //#if USE_ARRAY_ON_PARAMS
{
    UInt oside = (this->dimension + side)%(2*this->dimension);

    UInt coord = side%this->dimension;
    UInt opt = side/this->dimension;
    Real ocmp = cell->box()->getPoint(opt)->getCoord(coord);

    //PositionCellMap::iterator inCellsPrev = (*this->layers.find(cell->getId())).second[layer - 1][side].find(Data::IN);
    PositionCellMap::iterator inCellsPrev = layers[layer - 1][side].find(Data::IN);

    //if (inCellsPrev == (*this->layers.find(cell->getId())).second[layer - 1][side].end())
    if (inCellsPrev == layers[layer - 1][side].end())
    {
        return;
    }

    //PositionCellMap *cells = &(*this->layers.find(cell->getId())).second[layer][side];
    PositionCellMap *cells = &layers[layer][side];

    this->findLayer((*inCellsPrev).second, oside, opt, coord, ocmp, *cells);

/*#if USE_C__11
    for (Data::GraphNodeHash::iterator iter = inCellsPrev->begin();
#else
    for (Data::GraphNodeSet::iterator iter = inCellsPrev->begin();
#endif //#if USE_C__11
         iter != inCellsPrev->end(); iter++)
    {
        Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

        Data::GraphNodeList neighs = c->neighbors(oside);

        for (Data::GraphNodeList::iterator iter2 = neighs.begin();
             iter2 != neighs.end(); iter2++)
        {
            Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter2));

            if (!this->include(n, side, pt, coord, cmp))
            {
                continue;
            }

            (*cells)[PMJMesh::getPosition(this->dimension, n)].insert(n);
        }
    }*/
}

#if USE_ARRAY_ON_PARAMS
void PMJMesh2::LoadBSPTreeGenerator::findLayers(const PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **&layers) const
#else
void PMJMesh2::LoadBSPTreeGenerator::findLayers(const PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, LayerSidePositionCellMap &layers) const
#endif //#if USE_ARRAY_ON_PARAMS
{
    if (this->removeLayersInLoad == 0)
    {
        return;
    }

    /*if (this->loads.find(cell->getId()) != this->loads.end())
    {
        return;
    }*/

    /*CellLoadMap::iterator it = this->loads.find(cell->getId());

    if (it == this->loads.end())
    {
        return;
    }

    if (!(*it).second.layers.empty())
    {
        return;
    }*/

    for (UInt side = 0; side < cell->numSides(); side++)
    {
        this->findInitialLayer(cell, side, cells, layers);
    }

    for (UInt layer = 1; layer < this->removeLayersInLoad; layer++)
    {
        for (UInt side = 0; side < cell->numSides(); side++)
        {
            this->findLayer(cell, layer, side, layers);
        }
    }
}

void PMJMesh2::LoadBSPTreeGenerator::splitLayers(Params &params) const
{
    if (this->removeLayersInLoad == 0)
    {
        return;
    }

    UInt numSides = 2*this->dimension;

    /*//debug
    //if ((params.id == 1) && (params.dir == 1))
    if (params.id == 1)
    {
        Data::cout() << "debug" << Data::endl;
    }

    Data::cout() << "splitting cell " << params.id << Data::endl;
    for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
    {
        for (UInt side = 0; side < numSides; side++)
        {
            Data::cout() << "layer " << layer << ", side " << side << ", pos ON, size = " << params.layers[layer][side][Data::ON].size() << Data::endl;
            Data::cout() << "layer " << layer << ", side " << side << ", pos IN, size = " << params.layers[layer][side][Data::IN].size() << Data::endl;
        }
    }
    //endebug*/

#if USE_ARRAY_ON_PARAMS
    params.resize(this->removeLayersInLoad, numSides, false, true, true);
#else
    params.splittedLayers[0].resize(this->removeLayersInLoad);
    params.splittedLayers[1].resize(this->removeLayersInLoad);

    params.fixedLayers[0].resize(this->removeLayersInLoad);
    params.fixedLayers[1].resize(this->removeLayersInLoad);

    for (UInt i = 0; i < this->removeLayersInLoad; i++)
    {
        params.splittedLayers[0][i].resize(numSides);
        params.splittedLayers[1][i].resize(numSides);

        params.fixedLayers[0][i].resize(numSides);
        params.fixedLayers[1][i].resize(numSides);
    }
#endif //#if USE_ARRAY_ON_PARAMS

    UInt nside = params.dir;
    UInt oside = (this->dimension + nside)%(numSides);

    for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
    {
#if USE_ARRAY_ON_PARAMS
        params.fixedLayers[0][layer][oside][Data::ON] = params.layers[layer][oside][Data::ON];
        params.fixedLayers[0][layer][oside][Data::IN] = params.layers[layer][oside][Data::IN];

        params.fixedLayers[1][layer][nside][Data::ON] = params.layers[layer][nside][Data::ON];
        params.fixedLayers[1][layer][nside][Data::IN] = params.layers[layer][nside][Data::IN];
#else
        params.fixedLayers[0][layer][oside][Data::ON].swap(params.layers[layer][oside][Data::ON]);
        params.fixedLayers[0][layer][oside][Data::IN].swap(params.layers[layer][oside][Data::IN]);

        params.fixedLayers[1][layer][nside][Data::ON].swap(params.layers[layer][nside][Data::ON]);
        params.fixedLayers[1][layer][nside][Data::IN].swap(params.layers[layer][nside][Data::IN]);
#endif //#if USE_ARRAY_ON_PARAMS
    }

    this->findInitialLayer(nside, params.splittedCells[0], 1, params.dir, params.coord, params.splittedLayers[0][0][nside]);
    this->findInitialLayer(oside, params.splittedCells[1], 0, params.dir, params.coord, params.splittedLayers[1][0][oside]);

    for (UInt layer = 1; layer < this->removeLayersInLoad; layer++)
    {
        this->findLayer(params.splittedLayers[0][layer - 1][nside][Data::IN], oside, 0, params.dir, params.layersLimits[0], params.splittedLayers[0][layer][nside]);
        this->findLayer(params.splittedLayers[1][layer - 1][oside][Data::IN], nside, 1, params.dir, params.layersLimits[1], params.splittedLayers[1][layer][oside]);
    }

#if USE_REMOVED_LAYERS_PROPORTIONS
    bool allOne = true;

    for (UInt i = 0; i < std::min(this->removeLayersInLoad, static_cast<UInt>(this->removedLayersProportions.size())); i++)
    {
        if (std::fabs(this->removedLayersProportions[i] - 1.0) > Data::getTolerance())
        {
            allOne = false;

            break;
        }
    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

#if USE_C__11
    Data::GraphNodeHash splCells;
#else
    Data::GraphNodeSet splCells;
#endif //#if USE_C__11

#if USE_REMOVED_LAYERS_PROPORTIONS
    CellLayerSet layerMap;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

    for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
    {
#if USE_REMOVED_LAYERS_PROPORTIONS
        if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
        {
            splCells.insert(params.splittedLayers[0][layer][nside][Data::IN].begin(), params.splittedLayers[0][layer][nside][Data::IN].end());
            splCells.insert(params.splittedLayers[1][layer][oside][Data::IN].begin(), params.splittedLayers[1][layer][oside][Data::IN].end());

            /*for (PositionCellMap::const_iterator iter = params.splittedLayers[0][layer][nside].begin();
                 iter != params.splittedLayers[0][layer][nside].end(); iter++)
            {
                splCells.insert((*iter).second.begin(), (*iter).second.end());
            }

            for (PositionCellMap::const_iterator iter = params.splittedLayers[1][layer][oside].begin();
                 iter != params.splittedLayers[1][layer][oside].end(); iter++)
            {
                splCells.insert((*iter).second.begin(), (*iter).second.end());
            }*/
        }
#if USE_REMOVED_LAYERS_PROPORTIONS
        else
        {
#if USE_C__11
            for (Data::GraphNodeHash::iterator iter = params.splittedLayers[0][layer][nside][Data::IN].begin(), end = params.splittedLayers[0][layer][nside][Data::IN].end();
#else
            for (Data::GraphNodeSet::iterator iter = params.splittedLayers[0][layer][nside][Data::IN].begin(), end = params.splittedLayers[0][layer][nside][Data::IN].end();
#endif //#if USE_C__11
                 iter != end; iter++)
            {
                layerMap.insert(NodeLayer((*iter), layer));
            }

#if USE_C__11
            for (Data::GraphNodeHash::iterator iter = params.splittedLayers[1][layer][oside][Data::IN].begin(), end = params.splittedLayers[1][layer][oside][Data::IN].end();
#else
            for (Data::GraphNodeSet::iterator iter = params.splittedLayers[1][layer][oside][Data::IN].begin(), end = params.splittedLayers[1][layer][oside][Data::IN].end();
#endif //#if USE_C__11
                 iter != end; iter++)
            {
                layerMap.insert(NodeLayer((*iter), layer));
            }
        }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    }

    //UInt debug = 0;

    for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
    {
        for (UInt side = 0; side < numSides; side++)
        {
            if ((side == nside) || (side == oside))
            {
                continue;
            }

            for (PositionCellMap::iterator iter = params.layers[layer][side].begin();
                 iter != params.layers[layer][side].end(); iter++)
            {
                Data::Position pos = (*iter).first;

#if USE_C__11
                for (Data::GraphNodeHash::iterator iter2 = (*iter).second.begin();
#else
                for (Data::GraphNodeSet::iterator iter2 = (*iter).second.begin();
#endif //#if USE_C__11
                     iter2 != (*iter).second.end(); iter2++)
                {
                    Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter2));

                    /*if (c->level() > debug)
                    {
                        debug = c->level();
                    }*/

                    if ((params.id != 0) && (pos == Data::IN))
					{
#if USE_REMOVED_LAYERS_PROPORTIONS
						if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
						{
							splCells.erase(c);
						}
#if USE_REMOVED_LAYERS_PROPORTIONS
						else
						{
							layerMap.erase(NodeLayer(c, 0));
						}
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
					}

                    if (this->less(c, params.dir, params.coord))
                    {
                        /*//if (c->getId() == 122281456)
                        if (params.splittedLayers[0][layer][side].empty())
                        {
                            std::cout << "c = " << c->getId() << " at " << c << std::endl;
                            std::cout << "layer = " << layer << std::endl;
                            std::cout << "side = " << side << std::endl;
                            std::cout << "pos = " << pos << std::endl;
                            std::cout << "splittedLayers[0].size() = " << params.splittedLayers[0].size() << " size > layer = " << std::boolalpha << (params.splittedLayers[0].size() > layer) << std::endl;
                            std::cout << "splittedLayers[0][layer].size() = " << params.splittedLayers[0][layer].size() << " size > side = " << std::boolalpha << (params.splittedLayers[0][layer].size() > side) << std::endl;
                            std::cout << "splittedLayers[0][layer][side].size() = " << params.splittedLayers[0][layer][side].size() << " have(pos) = " << std::boolalpha << (params.splittedLayers[0][layer][side].find(pos) != params.splittedLayers[0][layer][side].end()) << std::endl;
                        }*/

                        //Data::cout() << "cell " << c->getId() << ", layer " << layer << ", side " << side << ", pos " << pos << ", less" << Data::endl;

                        params.splittedLayers[0][layer][side][pos].insert(c);
                    }
                    else
                    {
                        //Data::cout() << "cell " << c->getId() << ", layer " << layer << ", side " << side << ", pos " << pos << ", NOT less" << Data::endl;

                        params.splittedLayers[1][layer][side][pos].insert(c);
                    }
                }

#if (!USE_ARRAY_ON_PARAMS)
                (*iter).second.clear();
#endif //#if (!USE_ARRAY_ON_PARAMS)
            }
        }
    }

    if (params.id != 0)
    {
        for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
        {
            for (UInt side = 0; side < numSides; side++)
            {
                if ((side == nside) || (side == oside))
                {
                    continue;
                }

#if USE_C__11
                for (Data::GraphNodeHash::iterator iter = params.fixedLayers[0][layer][side][Data::IN].begin();
#else
                for (Data::GraphNodeSet::iterator iter = params.fixedLayers[0][layer][side][Data::IN].begin();
#endif //#if USE_C__11
                     iter != params.fixedLayers[0][layer][side][Data::IN].end(); iter++)
                {
#if USE_REMOVED_LAYERS_PROPORTIONS
                    if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
                    {
                        splCells.erase((*iter));
                    }
#if USE_REMOVED_LAYERS_PROPORTIONS
                    else
                    {
                        layerMap.erase(NodeLayer((*iter), 0));
                    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
                }

#if USE_C__11
                for (Data::GraphNodeHash::iterator iter = params.fixedLayers[1][layer][side][Data::IN].begin();
#else
                for (Data::GraphNodeSet::iterator iter = params.fixedLayers[1][layer][side][Data::IN].begin();
#endif //#if USE_C__11
                     iter != params.fixedLayers[1][layer][side][Data::IN].end(); iter++)
                {
#if USE_REMOVED_LAYERS_PROPORTIONS
                    if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
                    {
                        splCells.erase((*iter));
                    }
#if USE_REMOVED_LAYERS_PROPORTIONS
                    else
                    {
                        layerMap.erase(NodeLayer((*iter), 0));
                    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
                }
            }
        }
    }

    /*if (debug > 0)
    {
        Data::cout() << "max level = " << debug << Data::endl;
    }*/

    params.splittingLoad = 0.0;

#if USE_REMOVED_LAYERS_PROPORTIONS
    if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    {
#if USE_C__11
        for (Data::GraphNodeHash::iterator iter = splCells.begin();
#else
        for (Data::GraphNodeSet::iterator iter = splCells.begin();
#endif //#if USE_C__11
             iter != splCells.end(); iter++)
        {
            params.splittingCells.push_back((*iter));

            params.splittingLoad += (*this->loadCalculator->getTreeLoad().find((*iter)->getId())).second;
        }
    }
#if USE_REMOVED_LAYERS_PROPORTIONS
    else
    {
        //to avoid roundoff errors on each layer
        Real sumLayer[this->removeLayersInLoad];

        for (UInt i = 0; i < this->removeLayersInLoad; i++)
        {
            sumLayer[i] = 0.0;
        }

        for (CellLayerSet::iterator iter = layerMap.begin(), end = layerMap.end();
             iter != end; iter++)
        {
            params.splittingCells.push_back((*iter).node);

            sumLayer[(*iter).layer] += (*this->loadCalculator->getTreeLoad().find((*iter).node->getId())).second*this->removedLayersProportions[(*iter).layer];
        }

        for (UInt i = 0; i < this->removeLayersInLoad; i++)
        {
            params.splittingLoad += sumLayer[i];
        }
    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
}

void PMJMesh2::LoadBSPTreeGenerator::removeLoadFromLayers(Params &params) const
{
    if (this->removeLayersInLoad == 0)
    {
        return;
    }

#if USE_REMOVED_LAYERS_PROPORTIONS
    bool allOne = true;

    for (UInt i = 0; i < std::min(this->removeLayersInLoad, static_cast<UInt>(this->removedLayersProportions.size())); i++)
    {
        if (std::fabs(this->removedLayersProportions[i] - 1.0) > Data::getTolerance())
        {
            allOne = false;

            break;
        }
    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

    //Data::cout() << "allOne = " << std::boolalpha << allOne << Data::endl;

    for (UInt i = 0; i < 2; i++)
    {
#if USE_C__11
        Data::GraphNodeHash cells;
#else
        Data::GraphNodeSet cells;
#endif //#if USE_C__11

#if USE_REMOVED_LAYERS_PROPORTIONS
        CellLayerSet layerMap;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        UInt numSides = 2*this->dimension;

        for (UInt layer = 0; layer < this->removeLayersInLoad; layer++)
        {
            for (UInt side = 0; side < numSides; side++)
            {
#if USE_REMOVED_LAYERS_PROPORTIONS
                if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
                {
                    cells.insert(params.fixedLayers[i][layer][side][Data::IN].begin(), params.fixedLayers[i][layer][side][Data::IN].end());
                    cells.insert(params.splittedLayers[i][layer][side][Data::IN].begin(), params.splittedLayers[i][layer][side][Data::IN].end());

                    /*for (PositionCellMap::const_iterator iter = params.fixedLayers[i][layer][side].begin();
                         iter != params.fixedLayers[i][layer][side].end(); iter++)
                    {
                        cells.insert((*iter).second.begin(), (*iter).second.end());
                    }

                    for (PositionCellMap::const_iterator iter = params.splittedLayers[i][layer][side].begin();
                         iter != params.splittedLayers[i][layer][side].end(); iter++)
                    {
                        cells.insert((*iter).second.begin(), (*iter).second.end());
                    }*/

                    //Data::cout() << "numCells = " << cells.size() << Data::endl;
                }
#if USE_REMOVED_LAYERS_PROPORTIONS
                else
                {
#if USE_C__11
                    for (Data::GraphNodeHash::iterator iter = params.fixedLayers[i][layer][side][Data::IN].begin(), end = params.fixedLayers[i][layer][side][Data::IN].end();
#else
                    for (Data::GraphNodeSet::iterator iter = params.fixedLayers[i][layer][side][Data::IN].begin(), end = params.fixedLayers[i][layer][side][Data::IN].end();
#endif //#if USE_C__11
                         iter != end; iter++)
                    {
                        layerMap.insert(NodeLayer((*iter), layer));
                    }

#if USE_C__11
                    for (Data::GraphNodeHash::iterator iter = params.splittedLayers[i][layer][side][Data::IN].begin(), end = params.splittedLayers[i][layer][side][Data::IN].end();
#else
                    for (Data::GraphNodeSet::iterator iter = params.splittedLayers[i][layer][side][Data::IN].begin(), end = params.splittedLayers[i][layer][side][Data::IN].end();
#endif //#if USE_C__11
                         iter != end; iter++)
                    {
                        layerMap.insert(NodeLayer((*iter), layer));
                    }

                    //Data::cout() << "numCells = " << layerMap.size() << Data::endl;
                }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
            }
        }

        params.layersLoads[i] = 0.0;

        //Data::cout() << "params.layersLoads["<<i<<"] = " << params.layersLoads[i] << Data::endl;

#if USE_REMOVED_LAYERS_PROPORTIONS
        if (allOne)
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
        {
#if USE_C__11
            for (Data::GraphNodeHash::iterator iter = cells.begin();
#else
            for (Data::GraphNodeSet::iterator iter = cells.begin();
#endif //#if USE_C__11
                 iter != cells.end(); iter++)
            {
                params.layersLoads[i] += (*this->loadCalculator->getTreeLoad().find((*iter)->getId())).second;

                //Data::cout() << "params.layersLoads["<<i<<"] = " << params.layersLoads[i] << Data::endl;
            }
        }
#if USE_REMOVED_LAYERS_PROPORTIONS
        else
        {
            //to avoid roundoff errors on each layer
            Real sumLayer[this->removeLayersInLoad];

            for (UInt j = 0; j < this->removeLayersInLoad; j++)
            {
                sumLayer[j] = 0.0;
            }

            for (CellLayerSet::iterator iter = layerMap.begin(), end = layerMap.end();
                 iter != end; iter++)
            {
                sumLayer[(*iter).layer] += (*this->loadCalculator->getTreeLoad().find((*iter).node->getId())).second*this->removedLayersProportions[(*iter).layer];
            }


            for (UInt j = 0; j < this->removeLayersInLoad; j++)
            {
                params.layersLoads[i] += sumLayer[j];

                //Data::cout() << "params.layersLoads["<<i<<"] = " << params.layersLoads[i] << Data::endl;
            }
        }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    }
}

void PMJMesh2::LoadBSPTreeGenerator::execute()
{
    if ((!this->boundary) || (this->dimension != 3))
    {
        this->usingFrontTest = false;
    }

    if (this->usingFrontTest)
    {
        MJMesh::FrontElementSearchTreeGenerator treegenerator(this->boundary, this->boxTree);

        treegenerator.execute();

        this->searchTree = static_cast<Data::BoxSearchTree *>(treegenerator.getTree());
    }

    PMJMesh2::PartitionBSPTreeGenerator::execute();
}

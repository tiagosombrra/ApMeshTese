#include "PMJMesh2/GeometryBSPTreeGenerator.h"

#include "PMJMesh2/Communicator.h"
#include "PMJMesh2/PartitionBSPTree.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "PMJMesh/LoadCalculator.h"
#include "Data/Box.h"

using namespace PMJMesh2;

PMJMesh2::GeometryBSPTreeGenerator::GeometryBSPTreeGenerator() :
    PMJMesh2::PartitionBSPTreeGenerator::PartitionBSPTreeGenerator()
{

}

PMJMesh2::GeometryBSPTreeGenerator::~GeometryBSPTreeGenerator()
{

}

Real PMJMesh2::GeometryBSPTreeGenerator::load(const PartitionBSPTreeCell *cell) const
{
    CellLoadMap::iterator it = this->loads.find(cell->getId());

    return (it == this->loads.end()) ? -1.0 : (*it).second.load;
}

Real PMJMesh2::GeometryBSPTreeGenerator::innerLoad(const PartitionBSPTreeCell */*cell*/) const
{
    return 1.0;
}

bool PMJMesh2::GeometryBSPTreeGenerator::better(Real diffLoad1, Real diffLoad2) const
{
    return (diffLoad1 < diffLoad2);
}

PartitionBSPTreeCell *PMJMesh2::GeometryBSPTreeGenerator::executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size)
{
    if (cell->isRoot())
    {
        std::pair<CellLoadMap::iterator, bool> p = this->loads.insert(std::make_pair(cell->getId(), CellLoad()));

        if (p.second)
        {
            (*p.first).second.cells.push_back(this->boxTree->getRoot());
            (*p.first).second.load = (*this->loadCalculator->getTreeLoad().find(this->boxTree->getRoot()->getId())).second;
        }
    }

    return PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision(cell, rank, size);
}

PMJMesh2::PartitionBSPTreeGenerator::Split PMJMesh2::GeometryBSPTreeGenerator::findBestSubdivision(PartitionBSPTreeCell *cell) const
{
    std::pair<CellLoadMap::iterator, bool> p = this->loads.insert(std::make_pair(cell->getId(), CellLoad()));

    if ((cell->isRoot()) && (p.second))
    {
        (*p.first).second.cells.push_back(this->boxTree->getRoot());
        (*p.first).second.load = (*this->loadCalculator->getTreeLoad().find(this->boxTree->getRoot()->getId())).second;
    }

    Split spl[this->dimension];
    Real difs[this->dimension];
    UInt better = this->dimension;

    for (UInt dir = 0; dir < this->dimension; dir++)
    {
        LoadSplit split = this->findBestSubdivision(cell, (*p.first).second.cells, dir);

        spl[dir].direction = split.direction;
        spl[dir].coord = split.coord;

        if (split.direction == this->dimension)
        {
            difs[dir] = -1.0;

            continue;
        }

        difs[dir] = std::fabs(split.load[0] - split.load[1]);

        if ((better == this->dimension) ||
            (this->better(difs[dir], difs[better])))
        {
            better = dir;
        }
    }

    if (better == this->dimension)
    {
        Split split;

        split.direction = this->dimension;
        split.coord = 0.0;

        return split;
    }

    return spl[better];
}

PMJMesh2::GeometryBSPTreeGenerator::LoadSplit PMJMesh2::GeometryBSPTreeGenerator::findBestSubdivision(
    PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, UInt dir) const
{
    LoadSplit spl;

    spl.direction = dir;
    spl.coord = 0.5*(cell->box()->getMin()->getCoord(dir) + cell->box()->getMax()->getCoord(dir));

    ULInt id1 = 2*this->dimension*cell->getId() + 2*dir + 1;
    ULInt id2 = id1 + 1;

    std::pair<CellLoadMap::iterator, bool> p1 = this->loads.insert(std::make_pair(id1, CellLoad()));
    std::pair<CellLoadMap::iterator, bool> p2 = this->loads.insert(std::make_pair(id2, CellLoad()));

    CellLoadMap::iterator it1 = p1.first;
    CellLoadMap::iterator it2 = p2.first;

    if ((p1.second) || (p2.second))
    {
        Data::GraphNodeList cells1, cells2;

        bool splitted = this->split(cells, dir, spl.coord, cells1, cells2);

        if (!splitted)
        {
            spl.direction = this->dimension;
            (*it1).second.load = -1.0;
            (*it2).second.load = -1.0;
        }
        else
        {
            if (p1.second)
            {
                (*it1).second.cells.swap(cells1);
                (*it1).second.load = 0.0;

                for (Data::GraphNodeList::iterator iter = (*it1).second.cells.begin();
                     iter != (*it1).second.cells.end(); iter++)
                {
                    (*it1).second.load += (*this->loadCalculator->getTreeLoad().find((*iter)->getId())).second;
                }
            }

            if (p2.second)
            {
                (*it2).second.cells.swap(cells2);
                (*it2).second.load = 0.0;

                for (Data::GraphNodeList::iterator iter = (*it2).second.cells.begin();
                     iter != (*it2).second.cells.end(); iter++)
                {
                    (*it2).second.load += (*this->loadCalculator->getTreeLoad().find((*iter)->getId())).second;
                }
            }
        }
    }

    spl.load[0] = (*it1).second.load;
    spl.load[1] = (*it2).second.load;

    return spl;
}

#include "PMJMesh2/FileBSPTreeGenerator.h"

#if USE_FILE_BASED_BSP

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Box.h"
#include "Data/BSPTree.h"
#include "Data/BSPTreeCell.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"

using namespace PMJMesh2;

PMJMesh2::FileBSPTreeGenerator::FileBSPTreeGenerator() :
    PMJMesh2::LoadBSPTreeGenerator::LoadBSPTreeGenerator()
{
    this->currCell = NULL;
}

PMJMesh2::FileBSPTreeGenerator::~FileBSPTreeGenerator()
{
    for (UInt i = 0; i < this->bsps.size(); i++)
    {
        delete this->bsps[i];
    }
}

void PMJMesh2::FileBSPTreeGenerator::setBSPFileName(const std::string &bspFileName)
{
    this->bspFileName = bspFileName;
}

std::string PMJMesh2::FileBSPTreeGenerator::getBSPFileName() const
{
    return this->bspFileName;
}

void PMJMesh2::FileBSPTreeGenerator::execute()
{
    this->readFile();

    if (this->bsps.empty())
    {
        return;
    }

    PMJMesh2::LoadBSPTreeGenerator::execute();
}

void PMJMesh2::FileBSPTreeGenerator::readFile()
{
    std::ifstream file;

    std::string type = this->bspFileName.substr(this->bspFileName.find_last_of('.'));

    if (type != ".aacop")
    {
        return;
    }

    file.open(this->bspFileName.c_str());

	if (!file.is_open())
	{
		return;
	}

	UInt level, nprocs;

    file >> level;
    file >> nprocs;

    this->bsps.resize(nprocs);

    for (UInt i = 0; i < nprocs; i++)
    {
        Data::Point *min = static_cast<Data::Box *>(this->boxTree->getShape())->getMin();
        Data::Point *max = static_cast<Data::Box *>(this->boxTree->getShape())->getMax();

        if (min->dimension() == 2)
        {
            min = new Data::Point2D(min);
            max = new Data::Point2D(max);
        }
        else
        {
            min = new Data::Point3D(min);
            max = new Data::Point3D(max);
        }

        this->bsps[i] = new Data::BSPTree(min, max);

        Data::BSPTreeCell *root = new Data::BSPTreeCell(this->bsps[i]);

        root->makeShape();

        this->bsps[i]->setRoot(root);
    }

    while (!file.eof())
    {
        std::string line;

        std::getline(file, line);

        if ((line.empty()) || (line[0] != 'r'))
        {
            continue;
        }

        std::stringstream linestr;

        linestr << line;

        std::string r;

        linestr >> r;

        UInt proc = 0;

        linestr >> proc;

        Data::BSPTreeCell *root = static_cast<Data::BSPTreeCell *>(this->bsps[proc]->getRoot());

        this->readFile(file, root, NULL);
    }

	file.close();
}

void PMJMesh2::FileBSPTreeGenerator::readFile(std::ifstream &file, Data::BSPTreeCell *lchild, Data::BSPTreeCell *rchild)
{
    std::string line;

    std::getline(file, line);

    std::stringstream linestr;

    linestr << line;

    std::string cc;

    linestr >> cc;

    std::string c;

    linestr >> c;

    Data::BSPTreeCell *cell = lchild;

    if (c == "1")
    {
        cell = rchild;
    }

    if (cc == "cl")
    {
        cell->setSplit(this->dimension + 1, 0.0);

        return;
    }

    UInt dir;

    linestr >> dir;

    Real coord;

    linestr >> coord;

    cell->setSplit(dir, coord);

    cell->subdivide();

    this->readFile(file, static_cast<Data::BSPTreeCell *>(cell->child(0)), static_cast<Data::BSPTreeCell *>(cell->child(1)));
}

PartitionBSPTreeCell *PMJMesh2::FileBSPTreeGenerator::executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size)
{
    if (!this->currCell)
    {
        this->currCell = static_cast<Data::BSPTreeCell *>(this->bsps[rank]->getRoot());
    }
    else
    {
        while (this->currCell->level() != cell->level())
        {
            this->currCell = (this->currCell->child(1)->isLeaf()) ?
                static_cast<Data::BSPTreeCell *>(this->currCell->child(0)) :
                static_cast<Data::BSPTreeCell *>(this->currCell->child(1));
        }
    }

    PartitionBSPTreeCell *c = PMJMesh2::LoadBSPTreeGenerator::executeSubdivision(cell, rank, size);

    if (size <= 1)
    {
        this->currCell = NULL;
    }

    return c;
}

PMJMesh2::LoadBSPTreeGenerator::LoadSplit PMJMesh2::FileBSPTreeGenerator::findBestSubdivision(
#if USE_ARRAY_ON_PARAMS
    PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **layers, UInt dir) const
#else
    PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers, UInt dir) const
#endif //#if USE_ARRAY_ON_PARAMS
{
    if (dir != this->currCell->getSplitDirection())
    {
        PMJMesh2::LoadBSPTreeGenerator::LoadSplit spl;

        spl.direction = this->dimension;

        return spl;
    }

    return PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision(cell, cells, layers, dir);
}

bool PMJMesh2::FileBSPTreeGenerator::equal(const Params &params) const
{
    return std::fabs(this->currCell->getSplitCoord() - params.coord) <= Data::getTolerance();
}

UInt PMJMesh2::FileBSPTreeGenerator::heavier(PartitionBSPTreeCell */*cell1*/, PartitionBSPTreeCell */*cell2*/) const
{
    if (!this->currCell->child(0)->isLeaf())
    {
        return 0;
    }

    if (!this->currCell->child(1)->isLeaf())
    {
        return 1;
    }

    if (static_cast<Data::BSPTreeCell *>(this->currCell->child(0))->getSplitDirection() == this->dimension + 1)
    {
        return 0;
    }

    return 1;
}

UInt PMJMesh2::FileBSPTreeGenerator::heavier(const Params &params) const
{
    return (params.coord < this->currCell->getSplitCoord()) ? 1 : 0;
}

/*PMJMesh2::FileBSPTreeGenerator::LoadSplit PMJMesh2::FileBSPTreeGenerator::findBestSubdivision(
    PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, UInt dir) const
{
    Data::BSPTreeCell *bspcell = static_cast<Data::BSPTreeCell *>(this->bsps[this->rank]->getRoot());

    while (bspcell->level() != cell->level())
    {
        bspcell = static_cast<Data::BSPTreeCell *>(bspcell->child(0));
    }

    LoadSplit spl;

    if (dir != bspcell->getSplitDirection())
    {
        spl.direction = this->dimension;
        spl.coord = 0.0;

        return spl;
    }

    spl.direction = dir;
    spl.coord = bspcell->getSplitCoord();

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
}*/

#endif //#if USE_FILE_BASED_BSP

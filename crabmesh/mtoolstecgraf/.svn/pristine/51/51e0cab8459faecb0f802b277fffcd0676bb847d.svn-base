#ifndef _PMJMESH2_FILE_BSP_TREE_GENERATOR_H_
#define _PMJMESH2_FILE_BSP_TREE_GENERATOR_H_

#include "PMJMesh2/Definitions.h"

#if USE_FILE_BASED_BSP

#include "PMJMesh2/LoadBSPTreeGenerator.h"

namespace PMJMesh2
{
    class FileBSPTreeGenerator : public PMJMesh2::LoadBSPTreeGenerator
    {
    public:

        FileBSPTreeGenerator();
        virtual ~FileBSPTreeGenerator();

        virtual void setBSPFileName(const std::string &bspFileName);
        virtual std::string getBSPFileName() const;

        virtual void execute();

    protected:

        virtual void readFile();
        virtual void readFile(std::ifstream &file, Data::BSPTreeCell *lchild, Data::BSPTreeCell *rchild);

        using PMJMesh2::LoadBSPTreeGenerator::executeSubdivision;
        virtual PartitionBSPTreeCell *executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size);

        using PMJMesh2::LoadBSPTreeGenerator::findBestSubdivision;
        virtual PMJMesh2::LoadBSPTreeGenerator::LoadSplit findBestSubdivision(
#if USE_ARRAY_ON_PARAMS
            PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **layers, UInt dir) const;
#else
            PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers, UInt dir) const;
#endif //#if USE_ARRAY_ON_PARAMS

        virtual bool equal(const Params &params) const;

        using PMJMesh2::LoadBSPTreeGenerator::heavier;
        virtual UInt heavier(PartitionBSPTreeCell *cell1, PartitionBSPTreeCell *cell2) const;
        virtual UInt heavier(const Params &params) const;

        //virtual PMJMesh2::FileBSPTreeGenerator::LoadSplit findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, UInt dir) const;

    protected:

        std::string bspFileName;

        typedef std::vector<Data::BSPTree *> BSPTreeVector;

        BSPTreeVector bsps;

        mutable Data::BSPTreeCell *currCell;
    };
}

#endif //#if USE_FILE_BASED_BSP

#endif //#ifndef _PMJMESH2_FILE_BSP_TREE_GENERATOR_H_

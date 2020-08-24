#ifndef _PMJMESH2_GEOMETRY_BSP_TREE_GENERATOR_H_
#define _PMJMESH2_GEOMETRY_BSP_TREE_GENERATOR_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh2/PartitionBSPTreeGenerator.h"

namespace PMJMesh2
{
    class GeometryBSPTreeGenerator : public PMJMesh2::PartitionBSPTreeGenerator
    {
    public:

        GeometryBSPTreeGenerator();
        virtual ~GeometryBSPTreeGenerator();

    protected:

        struct LoadSplit : public PMJMesh2::PartitionBSPTreeGenerator::Split
        {
            Real load[2];
        };

    protected:

        virtual Real load(const PartitionBSPTreeCell *cell) const;
        virtual Real innerLoad(const PartitionBSPTreeCell *cell) const;

        using PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision;
        virtual PartitionBSPTreeCell *executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size);

        virtual PMJMesh2::PartitionBSPTreeGenerator::Split findBestSubdivision(PartitionBSPTreeCell *cell) const;
        virtual LoadSplit findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, UInt dir) const;

        virtual bool better(Real diffLoad1, Real diffLoad2) const;

    protected:

        struct CellLoad
        {
            CellLoad() : load(-1.0) {};
            Data::GraphNodeList cells;
            Real load;
        };

#if USE_C_11
        typedef std::unordered_map<ULInt, CellLoad> CellLoadMap;
#else
        typedef std::map<ULInt, CellLoad> CellLoadMap;
#endif //#if USE_C_11

        mutable CellLoadMap loads;
    };
}

#endif //#ifndef _PMJMESH2_GEOMETRY_BSP_TREE_GENERATOR_H_

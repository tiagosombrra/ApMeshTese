#ifndef _PMJMESH2_PARTITION_BSP_TREE_GENERATOR_H_
#define _PMJMESH2_PARTITION_BSP_TREE_GENERATOR_H_

#include "PMJMesh2/Definitions.h"
#include "Performer/TreeGenerator.h"
#include "Parallel/Builder.h"

namespace PMJMesh2
{
    class PartitionBSPTreeGenerator : public Performer::TreeGenerator, public Parallel::Builder
    {
    public:

        PartitionBSPTreeGenerator();
        virtual ~PartitionBSPTreeGenerator();

        virtual void setDimension(UInt dimension);

        virtual void setNumTasksPerThread(UInt numTasksPerThread);

        virtual void setBoxTree(const Data::BoxTree *boxTree);

        virtual void setRemoveLayersInLoad(UInt removeLayersInLoad);
#if USE_REMOVED_LAYERS_PROPORTIONS
        virtual void setRemovedLayersProportions(UInt i, Real proportion);
        virtual void setRemovedLayersProportions(const Data::RealVector &removedLayersProportions);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        virtual void setLoadCalculator(PMJMesh::LoadCalculator *loadCalculator);

        virtual PartitionBSPTree *getBsp() const;

        virtual const Parallel::PartitionVector &getPartitions() const;

        virtual void execute();

    protected:

        struct Split
        {
            UInt direction;
            Real coord;
        };

    protected:

        virtual Real load(const PartitionBSPTreeCell *cell) const = 0;
        virtual Real innerLoad(const PartitionBSPTreeCell *cell) const = 0;

        virtual UInt heavier(PartitionBSPTreeCell *cell1, PartitionBSPTreeCell *cell2) const;
        virtual UInt heavierChild(PartitionBSPTreeCell *cell) const;

#if USE_GUI
        using Performer::TreeGenerator::setTree;
#endif //#if USE_GUI
        //virtual void setTree(Data::TreeNode *n, Data::Tree *tree);

        //returns cell with greatest load
        virtual void subdivide(PartitionBSPTreeCell *cell, const Split &split) const;
        virtual Split findBestSubdivision(PartitionBSPTreeCell *cell) const = 0;
        virtual Int executeSubdivision(PartitionBSPTreeCell *cell);
        virtual PartitionBSPTreeCell *executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size);
        //virtual void subdivide(PartitionBSPTreeCell *&cell, Int &rank, Int &size);

        virtual bool less(const Data::BoxTreeCell *cell, UInt dir, Real coord) const;

        virtual bool isOutCell(const Data::BoxTreeCell *c) const;
        virtual bool split(const Data::GraphNodeList &cells, UInt dir, Real coord,
            Data::GraphNodeList &cells1, Data::GraphNodeList &cells2, bool skipOut = false) const;
        virtual bool splitRec(const Data::GraphNodeList &cells, UInt dir, Real coord,
            Data::GraphNodeList &cells1, Data::GraphNodeList &cells2, bool skipOut = false) const;

        virtual Parallel::Partition *makePartition(const PartitionBSPTreeCell *cell) const;

        virtual void adjustBox() const;
        virtual void findBox(Data::Point &min, Data::Point &max) const;
		virtual void findBox(const Data::BoxTreeCell *c, Data::Point &min, Data::Point &max) const;
		virtual bool setBox(Data::BSPTreeCell *c, const Data::Point &min, const Data::Point &max) const;

    protected:

        UInt dimension;

        UInt numTasksPerThread;

        const Data::BoxTree *boxTree;
        UInt removeLayersInLoad;
#if USE_REMOVED_LAYERS_PROPORTIONS
        Data::RealVector removedLayersProportions;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        PMJMesh::LoadCalculator *loadCalculator;

        Parallel::PartitionVector partitions;

    };
}

#endif //#ifndef _PMJMESH2_PARTITION_BSP_TREE_GENERATOR_H_

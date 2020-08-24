#ifndef _PMTOOLS2_PARALLEL_MESH_GENERATOR_H_
#define _PMTOOLS2_PARALLEL_MESH_GENERATOR_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "PMJMesh2/ParallelMeshGenerator.h"
#include "OctTree.hpp"

namespace PMTools2
{
    class ParallelMeshGenerator : virtual public PMJMesh2::ParallelMeshGenerator
    {
    public:

        ParallelMeshGenerator();
        virtual ~ParallelMeshGenerator();

        virtual void setFTree(const FTools::OctTree *ftree);

        virtual void setUseUniformRefinement(bool useUniformRefinement);
        virtual void setUseUniformSize(bool useUniformSize);

        virtual void finalize();

    protected:

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
        virtual std::string timing(const PMJMesh2::Task *task, Real timeTotal, const Data::ULI4Tuple &prev) const;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

        virtual const PMJMesh2::Task *cast(const Parallel::Task *t) const;
        virtual PMJMesh2::Task *cast(Parallel::Task *t) const;

        virtual bool setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
            Data::BoxTree *tree, Performer::IdManager *idManager) const;
        virtual void getParameters(const Parallel::Task *t) const;
        virtual bool isInError(const Parallel::Task *t) const;

        virtual void executeFrontCheck();

    protected:

        const FTools::OctTree *ftree;

        bool useUniformRefinement;
        bool useUniformSize;

    };
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_PARALLEL_MESH_GENERATOR_H_

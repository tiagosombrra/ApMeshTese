#ifndef _PMTOOLS2_A_PRIORI_PARALLEL_MESH_GENERATOR_H_
#define _PMTOOLS2_A_PRIORI_PARALLEL_MESH_GENERATOR_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "PMJMesh2/APrioriParallelMeshGenerator.h"
#include "PMTools2/ParallelMeshGenerator.h"

namespace PMTools2
{
    class APrioriParallelMeshGenerator :
        public PMJMesh2::APrioriParallelMeshGenerator,
        public PMTools2::ParallelMeshGenerator
    {
    public:

        APrioriParallelMeshGenerator();
        virtual ~APrioriParallelMeshGenerator();

        virtual void execute();

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

    };
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_A_PRIORI_PARALLEL_MESH_GENERATOR_H_

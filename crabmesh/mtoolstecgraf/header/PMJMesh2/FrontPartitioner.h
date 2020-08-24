#ifndef _PMJMESH2_FRONT_PARTITIONER_H_
#define _PMJMESH2_FRONT_PARTITIONER_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh/FrontPartitioner.h"

namespace PMJMesh2
{
    class FrontPartitioner : public PMJMesh::FrontPartitioner
    {
    public:

        FrontPartitioner(UInt dimension, bool addInRejected, PMJMesh::MJMeshTask *task,
            const MJMesh::Front *front);
        virtual ~FrontPartitioner();

        virtual void setTask(PMJMesh::MJMeshTask *task);

        virtual void setAddInRejected(bool addInRejected);

#if USE_C__11
        virtual Data::FrontElementHash *getRejected() const;
#else
        virtual Data::FrontElementSet *getRejected() const;
#endif //#if USE_C__11

        virtual void execute();

    protected:

        using PMJMesh::FrontPartitioner::belongTo;
        virtual bool belongTo(const MJMesh::FrontElement *e, PMJMesh::Partition *p, bool &exclusive) const;

    protected:

        PMJMesh::MJMeshTask *task;

        bool addInRejected;

#if USE_C__11
        Data::FrontElementHash *rejected;
#else
        Data::FrontElementSet *rejected;
#endif //#if USE_C__11
    };
}

#endif //#ifndef _PMJMESH2_FRONT_PARTITIONER_H_

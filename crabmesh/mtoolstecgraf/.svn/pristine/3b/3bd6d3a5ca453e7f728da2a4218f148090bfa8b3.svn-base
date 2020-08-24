#ifndef _PMJMESH2_MJMESH_TASK_H_
#define _PMJMESH2_MJMESH_TASK_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh2/Task.h"
#include "PMJMesh/MJMeshTask.h"

namespace PMJMesh2
{
    class MJMeshTask :
        public PMJMesh2::Task,
        public PMJMesh::MJMeshTask
    {
    public:

        MJMeshTask();
		MJMeshTask(PMJMesh::Partition *partition,
			PMJMesh2::WorkerMainDrive *builder,
			PMJMesh2::Communicator *comm = NULL);
		virtual ~MJMeshTask();

		virtual void makeBuilder();

		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

    protected:

        using PMJMesh2::Task::packSize;
        using PMJMesh::MJMeshTask::packSize;
        virtual UInt packSize(const Parallel::Communicator *comm,
			ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
			ULInt numFrontVertices, ULInt numFrontFrontElements, ULInt numAdvanced) const;

		using PMJMesh2::Task::pack;
		using PMJMesh::MJMeshTask::pack;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			const Data::Mesh *mesh, const Data::Front *front,
#if USE_C__11
            const Data::ULIntHash &advancedIds
#else
            const Data::ULIntSet &advancedIds
#endif //#if USE_C__11
        ) const;

        using PMJMesh2::Task::unpack;

    };
}

#endif //#ifndef _PMJMESH2_MJMESH_TASK_H_

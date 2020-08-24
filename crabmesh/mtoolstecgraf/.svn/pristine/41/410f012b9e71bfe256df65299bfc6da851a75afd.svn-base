#ifndef _PMTOOLS2_MTOOLS_TASK_H_
#define _PMTOOLS2_MTOOLS_TASK_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "PMJMesh2/Task.h"
#include "PMTools/MToolsTask.h"

namespace PMTools2
{
    class MToolsTask :
        public PMJMesh2::Task,
        public PMTools::MToolsTask
    {
    public:

        MToolsTask();
		MToolsTask(PMJMesh::Partition *partition,
			PMTools2::WorkerMainDrive *builder,
			PMTools2::Communicator *comm = NULL);
		virtual ~MToolsTask();

		virtual void makeBuilder();

		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

    protected:

        using PMJMesh2::Task::packSize;
        using PMTools::MToolsTask::packSize;
        virtual UInt packSize(const Parallel::Communicator *comm,
			ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
			ULInt numFrontVertices, ULInt numFrontFrontElements, ULInt numAdvanced) const;

		using PMJMesh2::Task::pack;
		using PMTools::MToolsTask::pack;
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

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_MTOOLS_TASK_H_

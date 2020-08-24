#ifndef _PMTOOLS_MTOOLS_TASK_H_
#define _PMTOOLS_MTOOLS_TASK_H_

#include "PMTools/Definitions.h"

#if (USE_MTOOLS)

#include "PMJMesh/MJMeshTask.h"

namespace PMTools
{
	class MToolsTask : public PMJMesh::MJMeshTask
	{
	public:

		MToolsTask();
		MToolsTask(PMJMesh::Partition *partition,
			PMJMesh::WorkerMainDrive *builder,
			PMJMesh::Communicator *comm = NULL);
		virtual ~MToolsTask();

		virtual void setEnableBackTracking(bool enableBackTracking);

		virtual void makeBuilder();

		virtual void execute();

		using PMJMesh::MJMeshTask::packSize;
		virtual UInt packSize(const Parallel::Communicator *comm,
			ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
			ULInt numFrontVertices, ULInt numFrontFrontElements) const;
		using PMJMesh::MJMeshTask::pack;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			//Data::VertexList &meshVertices, Data::FrontElementList &meshFels, Data::ElementList &meshEls,
			const Data::Mesh *mesh,
			//Data::VertexList &frontVertices, Data::FrontElementList &frontBoundary, Data::FrontElementList &frontElements,
			//Data::FrontElementList &frontRejected, Data::FrontElementList &frontCrossing) const;
			const Data::Front *front) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

	protected:

		bool enableBackTracking;
	};
}

#endif //#if (USE_MTOOLS)

#endif //#define _PMTOOLS_MTOOLS_TASK_H_

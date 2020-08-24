#ifndef _PMJMESH_FRONT_UPDATER_H_
#define _PMJMESH_FRONT_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "Parallel/Builder.h"

namespace PMJMesh
{
	class FrontUpdater : public Parallel::Builder
	{
	public:

		FrontUpdater(UInt dimension, bool shared, Data::Boundary *boundary,
			PMJMesh::Front *front, MJMesh::Mesh *mesh,
			const PMJMesh::TaskManager *taskManager = NULL,
			Parallel::Communicator *comm = NULL);
		virtual ~FrontUpdater();

		virtual void setDimension(UInt dimension);

		virtual void setShared(bool shared);

		virtual void setBoundary(Data::Boundary *boundary);

		virtual void setFront(PMJMesh::Front *front);

		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual void setTask(PMJMesh::MJMeshTask *task);
		virtual void setPartition(PMJMesh::Partition *partition);

		virtual void setTaskManager(const PMJMesh::TaskManager *taskManager);

		virtual bool isUpdated() const;

		virtual void initialize();
		virtual void execute();
		virtual void finalize();

	protected:

		virtual bool updateFrontShared() = 0;
		virtual bool updateFrontDistributed() = 0;

	protected:

		const PMJMesh::TaskManager *taskManager;

		UInt dimension;

		bool shared;
		bool updated;

		Data::Boundary *boundary;

		PMJMesh::Front *front;

		MJMesh::Mesh *mesh;

		PMJMesh::MJMeshTask *task;
		PMJMesh::Partition *partition;

		Data::VertexSet frontVertices;
		Data::FrontElementSet frontFels;
	};
}

#endif //#if (!USE_NEW_FRONT_UPDATER)

#endif //#ifndef _PMJMESH_FRONT_UPDATER_H_

#ifndef _PMJMESH_ABSTRACT_MESH_UPDATER_H_
#define _PMJMESH_ABSTRACT_MESH_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if USE_NEW_FRONT_UPDATER

namespace PMJMesh
{
	class AbstractMeshUpdater
	{
	public:

		AbstractMeshUpdater(UInt dimension, Data::AbstractMesh *mesh, Parallel::Communicator *comm, bool external = true);
		virtual ~AbstractMeshUpdater();

		virtual void setDimension(UInt dimension);
		virtual void setMesh(Data::AbstractMesh *mesh);
		virtual void setComm(Parallel::Communicator *comm);
		virtual void setExternal(bool external);

		virtual void setMasterTask(MJMeshTask *masterTask);
		virtual void setWorkerTask(MJMeshTask *workerTask);

		virtual bool isUpdated() const;

		virtual void initialize();
		virtual void finalize();

		virtual void execute();

		virtual void finalizeMesh();

	protected:

		virtual void replace(MJMesh::Vertex *temporary, MJMesh::Vertex *definitive, bool checkExistence, PMJMesh::Front *except = NULL) const;
		virtual void replace(MJMesh::FrontElement *temporary, MJMesh::FrontElement *definitive/*, bool checkExistence*/) const;

		virtual void initializeShared() = 0;
		virtual void initializeMaster() = 0;
		virtual void initializeWorker() = 0;

		virtual void finalizeShared() = 0;
		virtual void finalizeMaster() = 0;
		virtual void finalizeWorker() = 0;

		virtual void executeShared() = 0;
		virtual void executeMaster() = 0;
		virtual void executeWorker() = 0;

		virtual void finalizeMeshShared() = 0;
		virtual void finalizeMeshMaster() = 0;
		virtual void finalizeMeshWorker() = 0;

	protected:

		UInt dimension;

		Data::AbstractMesh *mesh;

		Parallel::Communicator *comm;

		bool updated;

		//external means that an external mesh generator will be used, not
		//  the internal MJMesh
		bool external;

		MJMeshTask *masterTask;

		//for distributed memory only
		MJMeshTask *workerTask;
	};
}

#endif //#if USE_NEW_FRONT_UPDATER

#endif //#ifndef _PMJMESH_ABSTRACT_MESH_UPDATER_H_

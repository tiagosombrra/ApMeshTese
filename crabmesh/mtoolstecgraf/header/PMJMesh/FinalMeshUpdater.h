#ifndef _PMJMESH_FINAL_MESH_UPDATER_H_
#define _PMJMESH_FINAL_MESH_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/EmptyMeshUpdater.h"

namespace PMJMesh
{
	class FinalMeshUpdater : public PMJMesh::EmptyMeshUpdater
	{
	public:

		FinalMeshUpdater(const PMJMesh::Front *front,
			UInt smoothingLayer, MJMesh::Boundary *boundary, Performer::IdManager *idManager,
			UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm, bool external = true);
		virtual ~FinalMeshUpdater();

	protected:

		virtual void initializeShared();
		virtual void initializeMaster();
		virtual void initializeWorker();

		virtual void finalizeShared();
		virtual void finalizeMaster();
		virtual void finalizeWorker();

		virtual void executeShared();
		virtual void executeMaster();
		virtual void executeWorker();

		virtual void finalizeMeshShared();
		virtual void finalizeMeshMaster();
		virtual void finalizeMeshWorker();

		virtual void splitMeshShared();
		virtual void splitMeshMaster();
		virtual void splitMeshWorker();

		virtual MJMesh::Mesh **splitMeshDistributed(ULInt *ids, Int size);

	protected:

		bool empty;
		MJMesh::Mesh *backup;
		MJMesh::Boundary *backupBoundary;

		//for worker only
		ULInt maxBoundaryFelId;
	};
}

#endif //#if USE_NEW_FRONT_UPDATER

#endif //#ifndef _PMJMESH_FINAL_MESH_UPDATER_H_

#ifndef _PMJMESH_EMPTY_MESH_UPDATER_H_
#define _PMJMESH_EMPTY_MESH_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/PartialMeshUpdater.h"

namespace PMJMesh
{
	class EmptyMeshUpdater : public PMJMesh::PartialMeshUpdater
	{
	public:

		EmptyMeshUpdater(const PMJMesh::Front *front,
			UInt smoothingLayer, MJMesh::Boundary *boundary, Performer::IdManager *idManager,
			UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm, bool noBoundary, bool external);
		virtual ~EmptyMeshUpdater();

		virtual void setFront(const PMJMesh::Front *front);

		virtual void splitMesh();

	protected:

		//debug
		//virtual void print() const;
		//endebug

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

	protected:

		const PMJMesh::Front *front;

		PMJMesh::Front *accumFront;

		//for workers only
		Data::FrontList fronts;

		//when noBoundary = true
		PMJMesh::Front *originalFront;
		UInt numMeshesFirstTime;

		bool deleteAccumFront;
	};
}

#endif //#if USE_NEW_FRONT_UPDATER

#endif //#ifndef _PMJMESH_EMPTY_MESH_UPDATER_H_

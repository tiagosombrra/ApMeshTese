#ifndef _PMJMESH_FINAL_FRONT_UPDATER_H_
#define _PMJMESH_FINAL_FRONT_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/EmptyFrontUpdater.h"

namespace PMJMesh
{
	class FinalFrontUpdater : public PMJMesh::EmptyFrontUpdater
	{
	public:

		FinalFrontUpdater(UInt dimension, UInt smoothingLayer, bool parallel,
			bool shared, Data::Boundary *boundary, PMJMesh::Front *front,
			MJMesh::Mesh *mesh, const PMJMesh::TaskManager *taskManager,
			Parallel::Communicator *comm);
		virtual ~FinalFrontUpdater();

		//virtual void initialize();
		//virtual void finalize();

	protected:

		//virtual void updateMeshShared();

		virtual void splitMeshShared();

		virtual void finalizeMeshShared();
	};
}

#endif //#if (!USE_NEW_FRONT_UPDATER)

#endif //#ifndef _PMJMESH_FINAL_FRONT_UPDATER_H_

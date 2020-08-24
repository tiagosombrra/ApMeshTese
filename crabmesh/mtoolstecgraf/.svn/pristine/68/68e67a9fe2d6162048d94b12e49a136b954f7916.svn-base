#ifndef _PMJMESH_MESH_UPDATER_H_
#define _PMJMESH_MESH_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/AbstractMeshUpdater.h"

namespace PMJMesh
{
	class MeshUpdater : public PMJMesh::AbstractMeshUpdater
	{
	public:

		MeshUpdater(UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm, bool external = true);
		virtual ~MeshUpdater();

	protected:

		virtual MJMesh::Mesh *getMesh();

		virtual void createMaps(const PMJMesh::Front *front,
#if USE_C__11
			Data::VertexHashMap &vMap, Data::FrontElementHashMap &felMap) const;
#else
			Data::VertexMap &vMap, Data::FrontElementMap &felMap) const;
#endif //#if USE_C__11

		virtual void merge(MJMesh::Mesh *mesh, PMJMesh::Front *update,
#if USE_C__11
			Data::VertexHashMap &vReplace, Data::FrontElementHashMap &felReplace);
#else
			Data::VertexMap &vReplace, Data::FrontElementMap &felReplace);
#endif //#if USE_C__11

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

	protected:

	};
}

#endif //#if USE_NEW_FRONT_UPDATER

#endif //#ifndef _PMJMESH_MESH_UPDATER_H_

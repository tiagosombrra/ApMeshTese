#ifndef _PMJMESH_ACTUAL_FRONT_UPDATER_H_
#define _PMJMESH_ACTUAL_FRONT_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/AbstractMeshUpdater.h"

namespace PMJMesh
{
	class ActualFrontUpdater : public PMJMesh::AbstractMeshUpdater
	{
	public:

		ActualFrontUpdater(UInt dimension, Front *front, Parallel::Communicator *comm, bool external = true);
		virtual ~ActualFrontUpdater();

		virtual void setFront(Front *front);

		virtual void finalizeFront();

	protected:

		virtual Front *getFront();

		virtual void initializeShared();
		virtual void initializeMaster();
		virtual void initializeWorker();

		virtual void finalizeShared();
		virtual void finalizeMaster();
		virtual void finalizeWorker();

		virtual void executeShared();
		virtual void executeMaster();
		virtual void executeWorker();

		virtual void executeSharedExternal(Front *oldFront, Front *commonFront, Front *newFront);
		virtual void executeSharedInternal(Front *oldFront, Front *commonFront, Front *newFront, MJMesh::Mesh *mesh);

		virtual void finalizeMeshShared();
		virtual void finalizeMeshMaster();
		virtual void finalizeMeshWorker();

	protected:

		struct CommonVertex
		{
			MJMesh::Vertex *orig;
			MJMesh::Vertex *temp;
			bool found;
#if USE_C__11
			Data::FrontElementHash oldFront;
			Data::FrontElementHash commonFront;
			//Data::FrontElementHash newFront;
#else
			Data::FrontElementSet oldFront;
			Data::FrontElementSet commonFront;
			//Data::FrontElementSet newFront;
#endif //#if USE_C__11
		};

#if USE_C__11
		typedef std::unordered_map<ULInt, CommonVertex> CommonVertexMap;
#else
		typedef std::map<ULInt, CommonVertex> CommonVertexMap;
#endif //#if USE_C__11

	protected:

		//for shared memory only
#if USE_C__11
		Data::VertexHashMap vMap;
		Data::FrontElementHashMap felMap;
#else
		Data::VertexMap vMap;
		Data::FrontElementMap felMap;
#endif //#if USE_C__11

		//for shared memory internal only
		Data::VertexList deletedVertices;
		Data::FrontElementList deletedFrontElements;
		CommonVertexMap tempMap;
	};
}

#endif //#if USE_NEW_FRONT_UPDATER

#endif //#ifndef _PMJMESH_ACTUAL_FRONT_UPDATER_H_

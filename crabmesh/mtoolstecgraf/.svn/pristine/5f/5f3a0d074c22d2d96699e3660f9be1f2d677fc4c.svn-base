#ifndef _PMJMESH_PARTIAL_MESH_UPDATER_H_
#define _PMJMESH_PARTIAL_MESH_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/MeshUpdater.h"

namespace PMJMesh
{
	class PartialMeshUpdater : public PMJMesh::MeshUpdater
	{
	public:

		PartialMeshUpdater(UInt smoothingLayer, MJMesh::Boundary *boundary, Performer::IdManager *idManager,
			UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm, bool noBoundary, bool external);
		virtual ~PartialMeshUpdater();

		virtual void setSmoothingLayer(UInt smoothingLayer);
		virtual void setBoundary(MJMesh::Boundary *boundary);
		virtual void setIdManager(Performer::IdManager *idManager);
		virtual Performer::IdManager *getIdManager() const;

		//virtual void setProcess(Int process);
		//virtual void setCreatedFrontElements(bool created);

		virtual const MJMesh::Mesh *getInnerMesh() const;

	protected:

		virtual void removeFromBoundary(const PMJMesh::Front *front,
			MJMesh::Mesh *mesh, const PMJMesh::Front *except = NULL);
		virtual void addToBoundary();
		virtual void adjustSharedVerticesAdjacencies();
		virtual void readjustSharedVerticesAdjacencies();

		using PMJMesh::MeshUpdater::replace;
		virtual void replace(MJMesh::Mesh *mesh, MJMesh::Mesh *outside,
			const MJMesh::Boundary &sharedBoundary, bool addBoundary,
			const PMJMesh::Front *front = NULL);

		virtual void split(const PMJMesh::Front *oldFront,
			const PMJMesh::Front *commonFront, const PMJMesh::Front *newFront,
			MJMesh::Mesh *mesh, MJMesh::Mesh *outside, MJMesh::Mesh *inside,
			bool addCommon, bool testExcept, bool addBoundary);

		virtual bool split(PMJMesh::WorkerMainDrive *drive, MJMesh::Mesh *mesh,
			MJMesh::Mesh *outside, MJMesh::Mesh *inside,
			bool addCommon, bool testExcept, bool addBoundary);

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

		template <typename T>
		struct TMap
		{
			T original;
			T temporary;
		};

		typedef TMap<Data::Vertex *> VMap;
		typedef TMap<Data::FrontElement *> FEMap;

		typedef std::list<VMap> VMapList;
		typedef std::list<FEMap> FEMapList;

#if USE_C__11
		typedef std::unordered_map<Data::Vertex *, Data::FrontElementList> VertexFEListHashMap;
#endif //#if USE_C__11
		typedef std::map<Data::Vertex *, Data::FrontElementList> VertexFEListMap;

	protected:

		//This attribute stores how close to the original or updated front a
		//  vertex/front element/element from the generated mesh must be to be
		//  improved by the master process, in the finalizing step. This "close"
		//  measure is a number of layers of elements distant from the front.
		UInt smoothingLayer;

		//The generated mesh will be split in two parts, which will be stored
		//  in two meshes, this->mesh and this->innerMesh. this->innerMesh
		//  stores the part of the mesh that is not close to the original or to
		//  the updated front.
		MJMesh::Mesh *innerMesh;

		MJMesh::Boundary *boundary;

		Performer::IdManager *idManager;

		//Largest ids for boundary vertices and front elements.
		//ULInt idBoundary[2];

		//Parallel::TaskList tasks;

		//bool useInnerMesh;
		static const bool useInnerMesh;

		Data::MeshList meshes;

		//for shared memory only
		VMapList newBoundaryVertices;
		FEMapList newBoundaryFrontElements;

		//For (master) distributed memory only
		//Int process;

		//for (master) distributed memory only
#if USE_C__11
		Data::VertexHashMap oldVertices;
		Data::FrontElementHashMap oldFrontElements;
#else
		Data::VertexMap oldVertices;
		Data::FrontElementMap oldFrontElements;
#endif //#if USE_C__11

		/*//For external tools only. Indicates that front elements were created and
		//  added to the boundary by this class.
		bool createdFels;*/

		//when noBoundary = true
		bool noBoundary;
		bool firstTime;

#if USE_C__11
		Data::VertexHashMap boundaryVertices;
		Data::FrontElementHashMap boundaryFrontElements;
#else
		Data::VertexMap boundaryVertices;
		Data::FrontElementMap boundaryFrontElements;
#endif //#if USE_C__11

#if USE_C__11
		Data::VertexHashMap sharedBoundaryVertices;
		VertexFEListHashMap sharedBoundaryVerticesAdjacency;
#else
		Data::VertexMap sharedBoundaryVertices;
		VertexFEListMap sharedBoundaryVerticesAdjacency;
#endif //#if USE_C__11
	};
}

#endif //#if USE_NEW_FRONT_UPDATER

#endif //#ifndef _PMJMESH_PARTIAL_MESH_UPDATER_H_

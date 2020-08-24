#ifndef _PMJMESH_EMPTY_FRONT_UPDATER_H_
#define _PMJMESH_EMPTY_FRONT_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/PartialFrontUpdater.h"

namespace PMJMesh
{
	class EmptyFrontUpdater : public PartialFrontUpdater
	{
	public:

		EmptyFrontUpdater(UInt dimension, UInt smoothingLayer, bool parallel,
			bool shared, Data::Boundary *boundary, PMJMesh::Front *front,
			MJMesh::Mesh *mesh, const PMJMesh::TaskManager *taskManager,
			Parallel::Communicator *comm);
		virtual ~EmptyFrontUpdater();

		virtual void initialize();
		virtual void finalize();

		virtual void splitMesh();

	protected:

		virtual void add(Data::Mesh *mesh, MJMesh::Vertex *v, bool forceInner);
		virtual void add(Data::Mesh *mesh, MJMesh::FrontElement *fel);

		//using PartialFrontUpdater::updateMesh;

		using PartialFrontUpdater::findNewBoundary;
		virtual void findNewBoundary(MJMesh::Mesh *mesh,
			const Data::FrontElementMap &felMap,
			Data::FrontElementSet &boundaryFels, Data::VertexSet &boundaryVs);

		using PartialFrontUpdater::splitMesh;

		virtual void splitMeshShared();
		virtual void splitMeshMaster(const Data::VertexMap &vMap, const Data::FrontElementMap &felMap);
		virtual void splitMeshMaster();
		virtual void splitMeshWorker();

		virtual void updateMeshShared();

		virtual bool updateFrontShared();
		using PartialFrontUpdater::updateFrontDistributed;
		virtual bool updateFrontDistributed(Data::VertexMap &vMap, Data::FrontElementMap &felMap, bool forceInner = true);
		virtual bool updateMeshWorker();

		using PartialFrontUpdater::updateFrontVertices;
		using PartialFrontUpdater::updateFrontElements;
		//mix of FullFrontUpdater::updateMesh(vertices, vMap, felMap) and
		//  FullFrontUpdater::updateFront(vertices, vMap, deleted)
		//virtual bool updateFront(Data::VertexList &vertices, Data::VertexMap &vMap, const Data::FrontElementMap &felMap);
		virtual bool updateFrontVertices(Data::Front *front, Data::VertexMap &vMap, const Data::FrontElementMap &felMap);
		//mix of FullFrontUpdater::updateMesh(fels, felMap) and
		//  FullFrontUpdater::updateFront(fels, felMap, deleted)
		//virtual bool updateFront(Data::FrontElementList &fels, Data::FrontElementMap &felMap);
		virtual bool updateFrontElements(Data::Front *front, Data::FrontElementMap &felMap);

	protected:

		Parallel::TaskVector tasks;

		Data::VertexSet boundaryVs;
		Data::FrontElementSet boundaryFels;
	};
}

#endif //#if (!USE_NEW_FRONT_UPDATER)

#endif //#ifndef _PMJMESH_EMPTY_FRONT_UPDATER_H_s

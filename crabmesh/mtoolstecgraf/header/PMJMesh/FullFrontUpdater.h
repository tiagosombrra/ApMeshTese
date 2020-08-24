#ifndef _PMJMESH_FULL_FRONT_UPDATER_H_
#define _PMJMESH_FULL_FRONT_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/FrontUpdater.h"

namespace PMJMesh
{
	class FullFrontUpdater : public FrontUpdater
	{
	public:

		FullFrontUpdater(UInt dimension, bool shared, Data::Boundary *boundary,
			PMJMesh::Front *front, MJMesh::Mesh *mesh,
			const PMJMesh::TaskManager *taskManager = NULL,
			Parallel::Communicator *comm = NULL);
		virtual ~FullFrontUpdater();

	protected:

		//virtual void createVMap(Data::VertexList &vertices, Data::VertexMap &vMap) const;
		virtual void createVMap(const Data::Front *front, Data::VertexMap &vMap) const;
		//virtual void createFelMap(Data::FrontElementList &fels, Data::FrontElementMap &felMap) const;
		virtual void createFelMap(const Data::Front *front, Data::FrontElementMap &felMap) const;

		virtual void replace(MJMesh::Vertex *v, MJMesh::Vertex *original, Data::ElementList &els) const;
		virtual void replace(MJMesh::Vertex *v, MJMesh::Vertex *original, const Data::FrontElementMap &felMap, Data::FrontElementList &fels, bool forceRemove) const;
		virtual void replace(MJMesh::Vertex *v, MJMesh::Vertex *original, const Data::FrontElementMap &felMap, bool forceRemove) const;
		virtual void replace(MJMesh::FrontElement *fel, MJMesh::FrontElement *original) const;

		virtual void clear(Data::VertexList &vertices/*, Data::Front *front*/) const;
		virtual void clear(Data::FrontElementList &fels/*, Data::Front *front*/) const;

		virtual void add(Data::Mesh *mesh, MJMesh::Vertex *v, bool forceInner);
		virtual void add(Data::Mesh *mesh, MJMesh::FrontElement *fel);

		//virtual bool updateMesh(Data::VertexList &vertices,
		virtual bool updateMeshVertices(const Data::Mesh *mesh, Data::VertexList &vertices,
			const Data::VertexMap &vMap, const Data::FrontElementMap &felMap,
			bool forceInner = true, bool forceRemove = false);
		//virtual bool updateMesh(Data::FrontElementList &fels,
		virtual bool updateMeshFrontElements(const Data::Mesh *mesh, Data::FrontElementList &fels,
			const Data::FrontElementMap &felMap);
		//virtual bool updateMesh(Data::ElementList &els);
		virtual bool updateMeshElements(const Data::Mesh *mesh);

		//virtual bool updateFront(Data::VertexList &vertices,
		virtual bool updateFrontVertices(Data::Front *front,
			Data::VertexMap &vMap, Data::VertexList &deleted);
		//virtual bool updateFront(Data::FrontElementList &fels,
		virtual bool updateFrontElements(Data::Front *front,
			Data::FrontElementMap &felMap, Data::FrontElementList &deleted);

		virtual void updateMeshShared();

		virtual bool updateFrontShared();

		virtual bool updateFrontDistributed(Data::VertexMap &vMap, Data::FrontElementMap &felMap, bool forceInner = true);
		virtual bool updateFrontDistributed();
	};
}

#endif //#if (!USE_NEW_FRONT_UPDATER)

#endif //#ifndef _PMJMESH_FULL_FRONT_UPDATER_H_

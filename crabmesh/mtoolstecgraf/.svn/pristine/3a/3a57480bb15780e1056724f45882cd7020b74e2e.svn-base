#ifndef _PMJMESH_PARTIAL_FRONT_UPDATER_H_
#define _PMJMESH_PARTIAL_FRONT_UPDATER_H_

#include "PMJMesh/Definitions.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/FullFrontUpdater.h"

namespace PMJMesh
{
	class PartialFrontUpdater : public FullFrontUpdater
	{
	public:

		PartialFrontUpdater(UInt dimension, UInt smoothingLayer, bool parallel,
			bool shared, Data::Boundary *boundary, PMJMesh::Front *front,
			MJMesh::Mesh *mesh, const PMJMesh::TaskManager *taskManager,
			Parallel::Communicator *comm);
		virtual ~PartialFrontUpdater();

		virtual void setParallel(bool parallel);

		virtual void setProcess(Int process);

		virtual void setSmoothingLayer(UInt smoothingLayer);

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual void setCreatedFels(bool createdFels);

		virtual MJMesh::Mesh *getMesh() const;

		virtual MJMesh::Mesh *getInnerMesh() const;

		virtual void initialize();
		virtual void finalize();

		virtual void finalizeMesh();

		//static void print(MJMesh::Mesh *);

	protected:

		virtual void buildBoundaryMaps();
		virtual void clearBoundaryMaps();

		virtual Data::FrontElementList findAdjacentFrontElements(const MJMesh::Vertex *v, const Data::Element *e) const;

		virtual void restoreBoundary();

		using PMJMesh::FullFrontUpdater::add;
		virtual void add(Data::Mesh *mesh, MJMesh::Vertex *v, bool forceInner);

		virtual void findNewBoundary(const MJMesh::Mesh *newMesh,
			const MJMesh::Front *newFront, const Data::FrontElementMap &felMap,
			Data::VertexList &newBoundaryVertices,
			Data::FrontElementList &newBoundaryElements);

		virtual void splitMesh(
			const Data::FrontElementList &oldFront,
			const Data::FrontElementList &newFront,
			const Data::FrontElementList &commonFront,
			const MJMesh::Mesh *newMesh);

		//remove from the given elements list the elements that are not in the
		//  given element set.
		virtual void adjustElements(Data::ElementList &elements, const Data::ElementSet &in);
		virtual void adjustElements(Data::ElementList &elements, const Data::Mesh *mesh);

		virtual bool insertInInnerBoundary(Data::Element *e, const Data::VertexSet &boundaryVertices) const;
		virtual bool insertInInnerBoundary(Data::FrontElement *e, const Data::VertexSet &boundaryVertices) const;

		virtual void splitMesh(
			const Data::FrontElementList &oldFront,
			const Data::FrontElementList &newFront,
			const Data::FrontElementList &commonFront,
			const MJMesh::Mesh *newMesh,
			Data::VertexSet &boundaryVertices,
			Data::FrontElementSet &boundaryFels,
			Data::FrontElementSet &unsmoothedLayerFels,
			Data::ElementSet &unsmoothedLayerEls,
			bool worker = false);

		virtual void updateMeshShared();

		virtual bool updateFrontShared();

		virtual Data::Vertex *copy(Data::Vertex *v) const;
		virtual Data::FrontElement *copy(Data::FrontElement *fel, const Data::VertexMap &mapVertices) const;

		virtual bool updateFrontDistributed(Data::VertexMap &vMap, Data::FrontElementMap &felMap, bool forceInner = true);
		virtual bool updateMeshMaster();
		virtual bool updateMeshWorker();

		virtual bool updateFrontDistributed();

		virtual void finalizeMeshShared();
		virtual void finalizeMeshMaster();
		virtual void finalizeMeshWorker();

	protected:

		typedef std::map<Int, Data::VertexMap> ProcessVertexMap;
		typedef std::map<Int, Data::FrontElementMap> ProcessFrontElementMap;
		typedef std::map<Int, Data::ElementMap> ProcessElementMap;

		bool parallel;

		Int process;

		//vMap stores, for each process, the original vertices in the updated
		//  boundary, indexed by their ids. This means: let P be a process; let
		//  S be one of its subdomains, let FE be the set of front elements
		//  generated for S; let FE' be a subset of FE such that each fe in FE'
		//  has only one adjacent element, is neither in the original front nor
		//  in the updated front for S; let V be the set of vertices of the
		//  front elements in FE'. Then, each v in V is an entry in vMap[P],
		//  i.e, vMap[P][v->id] = v.
		ProcessVertexMap vMap;
		//felMap stores, for each process, the original vertices in the updated
		//  boundary, indexed by their ids. This means: let P be a process; let
		//  S be one of its subdomains, let FE be the set of front elements
		//  generated for S; let FE' be a subset of FE such that each fe in FE'
		//  has only one adjacent element, is neither in the original front nor
		//  in the updated front for S. Then, each fe in FE' is an entry in
		//  felMap[P], i.e, felMap[P][fe->id] = fe.
		ProcessFrontElementMap felMap;
		//vMap stores, for each process, the original vertices in the updated
		//  boundary, indexed by their ids. This means: let P be a process; let
		//  S be one of its subdomains, let FE be the set of front elements
		//  generated for S; let FE' be a subset of FE such that each fe in FE'
		//  has only one adjacent element, is neither in the original front nor
		//  in the updated front for S; let V be the set of vertices of the
		//  front elements in FE'; let E be the set of elements in generated
		//  for S such that each e in E has one vertex in V, i.e., E is the set
		//  of elements adjacent to vertices in V. Then, each e in E is an
		//  entry in elMap[P], i.e, elMap[P][e->id] = e.
		ProcessElementMap elMap;

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

		//innerBoundaryFels stores the layer of front elements in
		//  this->innerMesh that is also adjacent to the updated boundary
		//  vertices
		Data::FrontElementSet innerBoundaryFels;
		//innerBoundaryEls stores the layer of elements in this->innerMesh that
		//  is also adjacent to the updated boundary vertices
		Data::ElementSet innerBoundaryEls;

		Performer::IdManager *idManager;

		ULInt idBoundaryVertex;
		ULInt idBoundaryFrontElement;

		bool createdFels;
		bool builtVMap;
		bool builtFEMap;
	};
}

#endif //#if (!USE_NEW_FRONT_UPDATER)

#endif //#ifndef _PMJMESH_PARTIAL_FRONT_UPDATER_H_

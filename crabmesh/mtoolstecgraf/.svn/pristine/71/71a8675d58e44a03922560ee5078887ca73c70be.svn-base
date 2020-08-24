#ifndef _PMJMESH2_APRIORI_PARALLEL_MESH_GENERATOR_H_
#define _PMJMESH2_APRIORI_PARALLEL_MESH_GENERATOR_H_

#include "MJMesh/Element2D.h"
#include "Data/Triangle3D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/ElementSurface.h"

#include "Tools/InnerSurfacesIdentifier.h"
#include "PMJMesh2/APrioriSupportMeshLaplacianSmoother.h"

#include "PMJMesh2/Definitions.h"
#include "PMJMesh2/ParallelMeshGenerator.h"

#include "TecGraf/MshSurf/MainDrive.h"
#include "TecGraf/Definitions.h"

#include <stdio.h>
#include <stdlib.h>

#if USE_TECGRAF_MSH_SURF

namespace PMJMesh2
{
	class APrioriParallelMeshGenerator : virtual public PMJMesh2::ParallelMeshGenerator
	{
	public:

		APrioriParallelMeshGenerator();
		virtual ~APrioriParallelMeshGenerator();

        virtual void setInterfaceElementSizeFactor(Real interfaceElementSizeFactor);
        virtual Real getInterfaceElementSizeFactor() const;

        virtual void execute();

    protected:

        virtual bool setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
            Data::BoxTree *tree, Performer::IdManager *idManager) const;

        virtual Data::Box *box(PMJMesh::MJMeshTask *box) const;
        virtual void moveFromOldToNewFront(PMJMesh::MJMeshTask *task) const;
        virtual void sharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;

        //virtual void executeDistributedFrontPartitioning();
        virtual void executeDistributedFrontPartitioningInParallel();

        virtual void PartitioningThisFront(bool first, PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, Data::Box *box1, Data::Box *box2, UInt direction, Data::VertexSet &common_vertices_list, Performer::IdManager *idManager);

        //virtual void executeFrontPartitioning();



        virtual void buildSubDomain(Data::Box *box, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, ULInt taskId, UInt side, bool updateBox);

        virtual void buildSubDomain3D(Data::Box *box, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, ULInt taskId, UInt side, bool updateBox);

        virtual void orient(MJMesh::FrontElementSurface *current, Data::FrontElementHash &right, const Data::FrontElementHash &wrong, bool detour) const;

        virtual Data::Mesh* generateSuportMesh( Data::VertexList internal_vertexs, Data::ElementList internal_elements, MJMesh::Boundary *surface_boundary, Real maxElementSize, ULInt vId, ULInt feId );

        virtual void translatePlane( Data::VertexList &internal_vertexs, UInt side, Real new_pos );

        virtual void flatSurfaceVertices( MJMesh::Boundary *surface_boundary, UInt side, Real new_pos, double minElementSize );

        virtual void prepareSurfacesMeshSurf( PMJMesh::Front *sub_front, PMJMesh::Front *local_front, const Performer::InnerSurfacesIdentifier::SurfaceTree *root, Data::VertexSet &common_vertices_list, Data::VertexHashMap &original_surface_vertexes, Data::VertexList internal_vertexs, Data::ElementList internal_elements, Performer::IdManager *id_Manager, ULInt taskId, bool invert_Mesh, UInt side, Real &meshSurf_Processing);

        virtual void improveBoundaryLoop( PMJMesh::Front *sub_front, Data::VertexHashMap &original_surface_vertexes, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements );

        virtual void removeOddEdges( Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements, Data::FrontElementHash &wrongElements);

        virtual void fixBoundaryLoops( PMJMesh::Front *sub_front, Data::VertexHashMap &original_surface_vertexes, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements, Data::Triangle3D &box_plane, Data::FrontElementHash &wrongElements );

        virtual void createSurfaceBoundary(PMJMesh::Front *sub_front, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements, Data::VertexHashMap &original_surface_vertexes, Data::Triangle3D &box_plane, UInt side);

        virtual void createInterface3D(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Data::VertexHashMap &original_surface_vertexes, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements, const Data::VertexList &internal_vertexs, const Data::ElementList &internal_elements, Performer::IdManager *id_Manager, ULInt taskId, bool invert_Mesh, UInt side, Real &meshSurf_Processing);

        virtual bool improveSurfaceMesh( PMJMesh::Front *sub_front, Data::Mesh *surfaceMesh, MJMesh::Boundary *surface_boundary, Performer::IdManager *mesh_surf_idManager, const Data::VertexList &surface_vertexs, const Data::FrontElementList &surface_elements, const Data::VertexHashMap &original_surface_vertexes, int side, Real pos_plane, Data::Point3D p_min, Data::Point3D p_max );

        virtual bool checkSurfaceMesh( PMJMesh::Front *sub_front, Data::Mesh *surfaceMesh, MJMesh::Boundary *surface_boundary, Performer::IdManager *mesh_surf_idManager, const Data::VertexList &surface_vertexs, const Data::FrontElementList &surface_elements, const Data::VertexHashMap &original_surface_vertexes, int side, Real pos_plane, Data::Point3D p_min, Data::Point3D p_max );

        virtual void clearSurfaceMesh( Data::Mesh *surfaceMesh, MJMesh::Boundary *surface_boundary );

        virtual void cleanOutFaces(PMJMesh::Front *sub_front, const Data::VertexHashMap &surface_vertexes_map);

        virtual bool isElementInside(Data::Box plane, Data::TreeNode *root);

		virtual void interceptPartitionFront(Data::Box *box, Data::TreeNode *root,
                                            std::list<Data::TreeNode *> &topBoundaryCells, std::list<Data::TreeNode *> &baseBoundaryCells,
                                            std::list<Data::TreeNode *> &leftBoundaryCells, std::list<Data::TreeNode *> &rightBoundaryCells, UInt side);

        //virtual void interceptPartitionFront(Data::Box * box, Data::Box plane, Data::TreeNode *root, Data::VertexList &internal_vertexs, Data::ElementList &internal_elements, bool invert_SupportMesh);
        virtual void interceptPartitionFront(Data::Box * box, Data::Box plane, int dir, Data::TreeNode *root, std::list<Data::Point3D> &internal_points);

        virtual MJMesh::Vertex* findInternal(Data::Point3D p, Data::VertexList &internal_vertexs);

		static bool compareBoxY(Data::TreeNode *first, Data::TreeNode *second);
		static bool compareBoxX(Data::TreeNode *first, Data::TreeNode *second);

		virtual MJMesh::Vertex *findOrCreate(ULInt taskId, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Performer::IdManager *id_Manager, Real x, Real y, Real z = 0.0);

		virtual MJMesh::Vertex *simpleFindOrCreate(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Performer::IdManager *id_Manager, Data::VertexList &vertices, Real x, Real y, Real z = 0.0);

		virtual MJMesh::Vertex* bestVertex(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Performer::IdManager *id_Manager, Data::Edge2D *e, ULInt taskId);

		MJMesh::Vertex* nearestVertex( PMJMesh::Front *local_front, MJMesh::Vertex* vertex);

		virtual MJMesh::Vertex* nearestCrossVertex(PMJMesh::Front *sub_front, MJMesh::Vertex* vertex);

		virtual MJMesh::Vertex* nearestCrossVertexNotAdj(PMJMesh::Front *sub_front, MJMesh::Vertex* vertex);

        virtual MJMesh::Vertex* findElementVertex(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, MJMesh::Vertex* vertex);

		virtual MJMesh::FrontElement2D * InitialEdge(ULInt taskId, Data::Box *box, PMJMesh::Front *sub_front, Data::FrontElementList &interfaceEdges, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, MJMesh::Vertex *vMin, MJMesh::FrontElement2D *e, bool invert);

		virtual bool includeInModel(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::FrontElementList &interfaceEdges, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, MJMesh::FrontElement2D *e, MJMesh::Vertex *v1, MJMesh::Vertex *v2, ULInt taskId);

		virtual std::list<Data::FrontElement *> interceptFront( PMJMesh::Front *local_front, MJMesh::FrontElement2D *e);

		virtual bool changeVertex(PMJMesh::Front *sub_front, MJMesh::Vertex *&v, MJMesh::Vertex *v_ref);

		bool unusedVertex(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::FrontElementList &interfaceEdges, MJMesh::Vertex* vertex);

		Data::FrontElementSet edgesInternal(MJMesh::FrontElement2D *e, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list);

		bool addAdjCross(PMJMesh::Front *sub_front, Data::FrontElementList &swappingEdges, MJMesh::Vertex* v_cross, MJMesh::Vertex* vertex);

		bool edgeOk(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::FrontElementList &interfaceEdges, Performer::IdManager *id_Manager, MJMesh::Vertex *&v, ULInt taskId);

		MJMesh::Vertex* nearestVertexSubFront(Data::Box *box, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, MJMesh::FrontElement2D *e, MJMesh::Vertex* vertex, ULInt taskId);

		Data::FrontElement* existEdgeInGlobal(MJMesh::FrontElement *e, PMJMesh::Front *local_front);

		bool boxEdgeTest(Data::Box *box, Data::TreeNode *root);

		bool boxOnEdgeTest(Data::Box *box, Data::TreeNode *root);

		void correctHoles(PMJMesh::Front *sub_front, PMJMesh::Front *local_front);

		void cleanOutEdges(PMJMesh::Front *sub_front);

		//adicionado por Markos em 16/09/2015
		virtual void splitMesh(PMJMesh::MJMeshTask *task);

		virtual void makeMaps() const;

		virtual void checkAndAddInFront(MJMesh::FrontElement *temp, MJMesh::FrontElement *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const;
        virtual void checkAndAddInFront(MJMesh::Vertex *temp, MJMesh::Vertex *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const;

#if USE_DELETE_FELS
        virtual void distributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;
#endif //#if USE_DELETE_FELS

    private:

        Data::VertexSet common_vertices_set;

        //TecGraf::MshSurf::MainDrive *mesh_surf_driver;


    protected:

        //adicionado por Markos em 16/09/2015
/*#if USE_C__11
        //tem que ser mapa, para manter ordenado
        typedef std::map<Data::ULInt, Data::FrontElementHash> ULIntFrontElementListMap;
#else
        typedef std::map<Data::ULInt, Data::FrontElementSet> ULIntFrontElementListMap;
#endif //#if USE_C__11

        mutable ULIntFrontElementListMap interfaceFrontElements;*/

        Real interfaceElementSizeFactor;

#if USE_C__11
        Data::FrontElementHashMap interfaceFrontElements;
        Data::VertexHashMap interfaceVertices;
#else
        Data::FrontElementMap interfaceFrontElements;
        Data::VertexMap interfaceVertices;
#endif //#if USE_C__11

#if USE_DELETE_FELS
        mutable Data::FrontElementList frontElementsToDelete;
#endif //#if USE_DELETE_FELS

	};
}

#endif //#if USE_TECGRAF_MSH_SURF

#endif //#ifndef _PMJMESH2_APRIORI_PARALLEL_MESH_GENERATOR_H_

#ifndef _PMJMESH2_PARALLEL_MESH_GENERATOR_H_
#define _PMJMESH2_PARALLEL_MESH_GENERATOR_H_

#include "PMJMesh2/Definitions.h"
#include "Performer/MeshGenerator.h"
#include "Parallel/Builder.h"

namespace PMJMesh2
{
    class ParallelMeshGenerator : public Performer::MeshGenerator, public Parallel::Builder
    {
    public:

        ParallelMeshGenerator();
        virtual ~ParallelMeshGenerator();

        virtual void setDimension(UInt dimension);
        virtual void setBoundarySorted(bool boundarySorted);
        virtual void setNumImproves(UInt numImproves);
        virtual void setSmoothingFactor(Real smoothingFactor);
        virtual void setLocakBackTrackingEnabled(bool localBackTrackingEnabled);
        virtual void setMeshSorted(bool meshSorted);
        virtual void setMaxBackTracks(UInt maxBackTracks);

        virtual void setNumTasksPerThread(UInt numTasksPerThread);
        virtual void setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition);
        virtual void setRangeProportion(Real rangeProportion);
		virtual void setImprovementInSlavesEnabled(bool improvementInSlavesEnabled);
		virtual void setImprovementLayers(UInt improvementLayers);
		virtual void setUpdateType(enum MeshUpdateType updateType);
		virtual void setFinalMeshState(enum FinalMeshState finalMeshState);

        virtual void setBoundary(const MJMesh::Boundary *boundary);
		virtual void setTree(const Data::BoxTree *tree);
		virtual void setBsp(const PartitionBSPTree *bsp);

		virtual void setTasks(const Parallel::TaskVector &tasks);

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual PMJMesh::Front *getFront() const;

        virtual void execute();

#if USE_NUMERICAL
        virtual void executeHotellingBackTransform(PMJMesh::HotellingTransformer *hotellingTransformer);
#endif //#if USE_NUMERICAL

        virtual void finalize();

    public:

		enum
		{
			NOT_INIT = 1,
			FRONT_PARTITIONED,
			SHARED_MEMORY_MESH_GENERATED,
			DISTRIBUTED_MEMORY_MESH_GENERATED,
			SUCCESS,

			ERROR = 100,
			ERROR_MESH_NOT_FULLY_GENERATED
		};

		static const Performer::Status notInit;
		static const Performer::Status frontPartitioned;
		static const Performer::Status sharedMemoryMeshGenerated;
		static const Performer::Status distributedMemoryMeshGenerated;
		static const Performer::Status success;

		static const Performer::Status error;
		static const Performer::Status errorMeshNotFullyGenerated;

    protected:

#if ((USE_MPI) && (USE_THREAD_COLOR))
        virtual void colorizeMesh(Parallel::TMCommunicator *comm, Data::Mesh *mesh, Real r, Real g, Real b) const;
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

#if ((USE_MPI) && (USE_THREAD_COLOR))
        virtual void finalizeJoinSharedMeshes(Real r, Real g, Real b);
#else
        virtual void finalizeJoinSharedMeshes();
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))
#if USE_MPI
        virtual void finalizeJoinDistributedMeshes();
        virtual void finalizeDistributedMeshes();
#endif //#if USE_MPI
#if USE_DELETE_FELS
        virtual void finalizeClearMesh();
#endif //#if USE_DELETE_FELS

    protected:

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
        virtual Data::ULI4Tuple makeTuple(const PMJMesh::MJMeshTask *task) const;
        virtual std::string timing(const PMJMesh2::Task *task, Real timeTotal, const Data::ULI4Tuple &prev) const;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

        virtual PMJMesh::Front *makeFront(const MJMesh::Boundary *boundary) const;
        virtual Performer::IdManager *makeIdManager(const Parallel::TMCommunicator *comm, Int id) const;

        virtual const PMJMesh2::Task *cast(const Parallel::Task *t) const;
        virtual PMJMesh2::Task *cast(Parallel::Task *t) const;

        virtual bool setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
            Data::BoxTree *tree, Performer::IdManager *idManager) const;
        virtual void getParameters(const Parallel::Task *t) const;
        virtual bool isInError(const Parallel::Task *t) const;

//#if USE_OPENMP
        virtual PartitionBSPTreeCell *findCommonParent() const;
        virtual PartitionBSPTreeCell *findCommonParent(UInt min, UInt max) const;
//#endif //#if USE_OPENMP
#if USE_OPENMP
        virtual void setLock(PartitionBSPTreeCell *cell, bool set) const;
#endif //#if USE_OPENMP

    protected:

        virtual void executeFrontPartitioning();

        virtual void mergeFromSharedTask(MJMesh::Mesh *mesh);

        virtual bool removeFromBoundary(MJMesh::Vertex *v, MJMesh::Mesh *newMesh, MJMesh::Boundary *newBoundary,
            bool fromMap = false, PMJMesh::Front *front = NULL) const;
#if USE_DELETE_FELS
        virtual void createNewFrontElements(MJMesh::Vertex *v, MJMesh::Mesh *newMesh, MJMesh::Boundary *newBoundary, MJMesh::Mesh *mesh, UInt threadId) const;
#endif //#if USE_DELETE_FELS

        virtual void mergeBoxesAndId(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;

        virtual void mergeCommonFront(PMJMesh::MJMeshTask *task) const;
        virtual void move(PMJMesh::Front *from, PMJMesh::Front *to,
            const PMJMesh::Front *exceptFrom = NULL, const PMJMesh::Front *exceptTo = NULL) const;

        virtual Data::Box *box(PMJMesh::MJMeshTask *box) const;

        virtual void sharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;
        virtual void fullSharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;
        virtual void partialSharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const;
        virtual void emptySharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const;

        virtual void makeMaps() const;
        virtual void checkAndAddInFront(MJMesh::FrontElement *temp, MJMesh::FrontElement *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const;
        virtual void checkAndAddInFront(MJMesh::Vertex *temp, MJMesh::Vertex *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const;

        virtual void distributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;
        virtual void fullDistributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const;
        virtual void partialDistributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const;
        virtual void emptyDistributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const;

        virtual bool executeShared(PMJMesh::MJMeshTask *task, UInt id, bool shouldMerge, bool &error
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            , Real &generation, Real &wait, Real &merge
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            );

        virtual void executeSharedMeshGeneration();

        virtual void splitMesh(PMJMesh::MJMeshTask *task);
        virtual void addRejectedFrontElementsThatAdvanced(PMJMesh::MJMeshTask *task);
        virtual void removeBoundaryFromMesh();
        virtual void addBoundaryToMesh(PMJMesh::MJMeshTask *task);
        virtual void clearDrive(PMJMesh::MJMeshTask *task);
        virtual void send(ULInt ids[3], PMJMesh::MJMeshTask *task, Int peer);
        virtual PMJMesh::MJMeshTask *receive(ULInt ids[3], Int peer);

        virtual bool executeDistributed(PMJMesh::MJMeshTask *task, bool &error
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            , Real &communication, Real &merge, Real &generation
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            );

        virtual void executeDistributedMeshGeneration();

        virtual void executeFrontCheck();

    protected:

        UInt dimension;
        bool boundarySorted;
        UInt numImproves;
        Real smoothingFactor;
        bool localBackTrackingEnabled;
        bool meshSorted;
        UInt maxBackTracks;

        UInt numTasksPerThread;
        bool createInnerFrontInDecomposition;
        Real rangeProportion;
		bool improvementInSlavesEnabled;
		UInt improvementLayers;
		enum MeshUpdateType updateType;
		enum FinalMeshState finalMeshState;

		bool createCrackInInnerFront;
		bool displaceVerticesInInnerFront;

		const MJMesh::Boundary *boundary;
		const Data::BoxTree *tree;
		const PartitionBSPTree *bsp;

        PMJMesh::Front *front;

        Performer::IdManager *idManager;
        Performer::IdManagerVector idManagers;
		mutable ULInt idoffset;
		ULInt idrange;

        Parallel::TaskVector tasks;

//#if USE_OPENMP
        PartitionBSPTreeCell *commonParent;
        PMJMesh2::CrossingReplacer *replacer;
//#endif //#if USE_OPENMP

#if USE_MPI
        bool createBoundaryInFullDistributedMerge;

#if USE_C__11
        mutable Data::VertexHashMap bVertices;
        mutable Data::FrontElementHashMap bFrontElements;
        mutable Data::FrontElementHash rejectedFrontElements;
#else
        mutable Data::VertexMap bVertices;
        mutable Data::FrontElementMap bFrontElements;
        mutable Data::FrontElementSet rejectedFrontElements;
#endif //#if USE_C__11
#endif //#if USE_MPI
    };
}

#endif //#ifndef _PMJMESH2_PARALLEL_MESH_GENERATOR_H_

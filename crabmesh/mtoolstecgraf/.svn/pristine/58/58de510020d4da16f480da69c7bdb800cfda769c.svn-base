#ifndef _PMJMESH_GLOBAL_GEOMETRY_TASK_MANAGER_H_
#define _PMJMESH_GLOBAL_GEOMETRY_TASK_MANAGER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/TaskManager.h"
#include "PMJMesh/CrossingReplacer.h"

namespace PMJMesh
{
	class GlobalTaskManager : public TaskManager
	{
	public:

		GlobalTaskManager(bool sharedParallel, UInt dimension,
			MJMesh::Boundary *boundary, Data::BoxTree *tree, Real percentageNonWorkingTask,
			bool heightDependentInLoad, UInt removeLayersInLoad, bool smoothInSlavesEnabled, UInt numImproves,
			Real smoothingFactor,
#if (!USE_DELETE_FELS)
			UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			UInt smoothingLayer,
			bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks,
			enum PartitionerType partitionerType,
			enum MeshUpdateType updateType, enum ShiftType shiftType,
			bool usePartitioningThreshold, Real partitioningThreshold,
			bool sorted, bool usingSearchTrees, Real rangeProportions,
			UInt minRefinementLevel, bool skippingInnerInDecomposition,
			bool createInnerFrontInDecomposition,
			enum Parallel::AvailableLoadBalancers balancerType,
			Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm);
		virtual ~GlobalTaskManager();

		virtual void setPartitionerType(enum PartitionerType partitionerType);
		virtual void setUpdateType(enum MeshUpdateType updateType);
		virtual void setShiftType(enum ShiftType shiftType);

		virtual void setFront(PMJMesh::Front *front);
		virtual PMJMesh::Front *getFront() const;

		virtual void setMesh(MJMesh::Mesh *mesh);
		virtual MJMesh::Mesh *getMesh() const;

#if USE_NEW_FRONT_UPDATER
		virtual const Data::Mesh *getUnsmoothedMesh() const;
#else
		virtual Data::Mesh *getUnsmoothedMesh() const;
#endif //#if USE_NEW_FRONT_UPDATER

		virtual void setPercentageNonWorkingTask(Real percentageNonWorkingTask);

		virtual void setUsingSearchTrees(bool usingSearchTrees);
		virtual void setRangeProportion(Real rangeProportion);

		virtual void setRemoveLayersInLoad(UInt removeLayersInLoad);
		virtual void setHeightDependentInLoad(bool heightDependentInLoad);

		virtual void setSmoothInSlavesEnabled(bool smoothInSlavesEnabled);
		virtual void setNumImproves(UInt numImproves);
		virtual void setSmoothingFactor(Real smoothingFactor);
#if (!USE_DELETE_FELS)
		virtual void setUnsmoothingLayer(UInt unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)
		virtual void setSmoothingLayer(UInt smoothingLayer);
		virtual void setLocalBackTrackingEnabled(bool localBackTrackingEnabled);
		virtual void setMeshSorted(bool meshSorted);
		virtual void setMaxBackTracks(UInt maxBackTracks);

		virtual Parallel::Task *makeTask() const;

		virtual void splitMesh();
		virtual void finalizeMesh();

		virtual void execute();

	public:

		enum
		{
			SUCCESS = 0,
			NOT_INIT,
			INIT,

			/*SHARED_LOAD_BALANCED,
			SHARED_MESH_GENERATED,
			SHARED_FRONT_UPDATED,
			SHARED_TREE_SHIFTED,
			SHARED_FRONT_DIVIDED,
			SHARED_FINALIZED,

			DISTRIBUTED_LOAD_BALANCED,
			DISTRIBUTED_TASKS_SENT,
			DISTRIBUTED_TASKS_RECEIVED,
			DISTRIBUTED_FRONT_UPDATED,
			DISTRIBUTED_TREE_SHIFTED,
			DISTRIBUTED_FRONT_DIVIDED,
			DISTRIBUTED_FINALIZED,*/

			LOAD_BALANCED,
			WORKERS_MESH_GENERATED,
			FRONT_UPDATED,
			TREE_SHIFTED,
			FRONT_DIVIDED,
			WORKERS_FINALIZED,

			PARALLEL_MESH_GENERATED,
			SERIAL_MESH_GENERATED,

			ERROR = 100
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status success;
		static const Performer::Status error;

		/*static const Performer::Status sharedLoadBalanced;
		static const Performer::Status sharedMeshGenerated;
		static const Performer::Status sharedFrontUpdated;
		static const Performer::Status sharedTreeShifted;
		static const Performer::Status sharedFrontDivided;
		static const Performer::Status sharedFinalized;

		static const Performer::Status distributedLoadBalanced;
		static const Performer::Status distributedTasksSent;
		static const Performer::Status distributedTasksReceived;
		static const Performer::Status distributedFrontUpdated;
		static const Performer::Status distributedTreeShifted;
		static const Performer::Status distributedFrontDivided;
		static const Performer::Status distributedFinalized;*/

		static const Performer::Status loadBalanced;
		static const Performer::Status workersMeshGenerated;
		static const Performer::Status frontUpdated;
		static const Performer::Status treeShifted;
		static const Performer::Status frontDivided;
		static const Performer::Status workersFinalized;

		static const Performer::Status parallelMeshGenerated;
		static const Performer::Status serialMeshGenerated;

	protected:

		//master
		virtual void initializeMaster();
		virtual void executeSharedOrDistributed();
		//virtual void executeShared();
		//virtual void executeDistributed();
		virtual Real calculateAbsLoadThreshold() const;
		virtual bool underAbsThreshold(Real load) const;
		virtual bool underThreshold(Parallel::Task *t) const;
		virtual void splitTasks(Parallel::TaskVector &working, Parallel::TaskVector &nonWorking) const;
		virtual void balanceLoad(bool shared);
		virtual void sendTasksForSlaves();
		virtual void receiveTasksFromSlaves();

		virtual bool updateFront(bool shared, bool updateTree = true);
		virtual void sendTerminationMessage();
		virtual void shiftTree(bool originalPosition = false);
		virtual void divideFront();
		virtual void sortFront();
		virtual void executeGeometryGeneration();

		//slave
		virtual void initializeSlave();
		virtual bool receiveTasksFromMaster();
		virtual void generateMeshInCells();
		virtual void sendTasksForMaster(bool updateTree = true);

		//all
		virtual void clearDone();

		//master
		virtual UInt shiftState() const;
		virtual void initializeGeneratedTasks();
		virtual void updateGeneratedTasks(bool shared);
		virtual bool completelyGenerated() const;
		//virtual void debugHighlightInterTasks();
		virtual void relatedTasks(const Data::GraphNodeList &neighCells, Parallel::TaskList &neighTasks) const;
		virtual bool foundNonGenerated(const Parallel::TaskList &tasks, Parallel::TaskList *nonGenerated = NULL) const;
		virtual bool foundNonGenerated(const Data::GraphNodeList &neighs, Parallel::TaskList *nonGenerated) const;
		virtual bool foundNonGenerated(const Data::GraphNodeList &neighs) const;
		virtual bool isGeneratedExcept(const MJMeshTask *task, UInt except) const;
		virtual bool isInterTask(const MJMeshTask *task) const;
		virtual bool checkInnerFront(const Data::VertexList &vertices, const Data::FrontElementList &fels,
			const PMJMesh::Partition *partition, Real size) const;
		virtual void createInnerFront(PMJMesh::Partition *p);
		virtual void createInnerFront(PMJMesh::FrontPartitioner *fp);
		virtual void backReplaceInnerFront();

		virtual LoadCalculator *makeLoadCalculator(bool initializing) const;
#if USE_C__11
		virtual TreePartitioner *makeTreePartitioner(const Data::RealHashMap *load, PMJMesh::LoadCalculator *loadCalculator) const;
#else
		virtual TreePartitioner *makeTreePartitioner(const Data::RealMap *load, PMJMesh::LoadCalculator *loadCalculator) const;
#endif //#if USE_C__11
#if USE_NEW_FRONT_UPDATER
		virtual ActualFrontUpdater *makeFrontUpdater() const;
		virtual MeshUpdater *makeMeshUpdater() const;
#endif //#if USE_NEW_FRONT_UPDATER
		virtual FrontPartitioner *makeFrontPartitioner() const;
		virtual BoxTreeShifter *makePartitionTreeShifter() const;

	protected:

		virtual bool setTaskParameters(Parallel::Task *t,
			MJMesh::Boundary *boundary, Data::BoxTree *tree,
			Performer::IdManager *idManager) const;
		virtual bool isInError(const Parallel::Task *t) const;
		virtual void getParameters(const Parallel::Task *t) const;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		virtual std::string timing(const Parallel::Task *task, double timeTotal) const;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	protected:

		Parallel::TaskVector nonWorkingTasks;

		//bool sorted;

		bool usePartitioningThreshold;
		Real partitioningThreshold;
		Real maxCalculatedLoad;
		Real percentageNonWorkingTask;

		enum PartitionerType partitionerType;
		enum MeshUpdateType updateType;
		enum ShiftType shiftType;
		enum Parallel::AvailableLoadBalancers balancerType;

		PMJMesh::Front *front;
		MJMesh::Mesh *mesh;

		Data::BoxTree *partitionTree;
		Parallel::PartitionVector partitions;

		BoxTreeShifter *shifter;

		bool considerOnCellsInLoad;
		bool heightDependentInLoad;
		UInt removeLayersInLoad;

#if USE_NEW_FRONT_UPDATER
		MeshUpdater *meshUpdater;
		ActualFrontUpdater *frontUpdater;

		Performer::RangedIdManager *slaveIdManager;
#else
		FrontUpdater *frontUpdater;
#endif //#if USE_NEW_FRONT_UPDATER

		CrossingReplacer *replacer;

		bool taskSent;
		///safety check -> the tree has shifted to every possible position, but
		///  no more mesh was generated
		UInt completelyGeneratedSafetyCheck;
		UInt maxCompletelyGeneratedSafetyCheck;

	protected:

		bool sorted;
		bool usingSearchTrees;
		Real rangeProportion;

		bool smoothInSlavesEnabled;
		UInt numImproves;
		Real smoothingFactor;
#if (!USE_DELETE_FELS)
		UInt unsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)
		UInt smoothingLayer;
		bool localBackTrackingEnabled;
		bool meshSorted;
		UInt maxBackTracks;
		UInt minRefinementLevel;

		Real numLayersMissingInGeneration;
		bool useAbsLoadThreshold;
		bool useSumLoads;
		Real absLoadThreshold;

		bool skippingInnerInDecomposition;
		bool createInnerFrontInDecomposition;
		bool createCrackInInnerFront;
		bool displaceVerticesInInnerFront;
		bool deleteFelsInInnerFront;

#if USE_C__11
		PMJMesh::CrossingReplacer::VertexReplacementHashMap createdInnerVertices;
		PMJMesh::CrossingReplacer::FrontElementReplacementHashMap createdInnerFels;
#else
		PMJMesh::CrossingReplacer::VertexReplacementMap createdInnerVertices;
		PMJMesh::CrossingReplacer::FrontElementReplacementMap createdInnerFels;
#endif //#if USE_C__11

#if USE_DELETE_FELS
        bool deleteExtraFrontElements;
#endif //#if USE_DELETE_FELS

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	public:

		mutable Real timeMeshGeneratorMasterTreeDecomposition,
			timeMeshGeneratorMasterFrontDecomposition,
			timeMeshGeneratorWorkerMeshGeneration,
			timeMeshGeneratorWorkerMeshImprovement,
			timeMeshGeneratorWorkerTreeClassification,
			timeMeshGeneratorWorkerFromMToolsConversion,
			timeMeshGeneratorWorkerToMToolsConversion,
			timeMeshGeneratorWorkerOthers,
			timeMeshGeneratorMasterSendTasks,
			timeMeshGeneratorMasterReceiveTasks,
			timeMeshGeneratorMasterMeshUpdate,
			timeMeshGeneratorMasterTreeShift,
			timeMeshGeneratorMasterFinalization,
			timeMeshGeneratorMasterOthers;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	};
}

#endif //#ifndef _PMJMESH_GLOBAL_GEOMETRY_TASK_MANAGER_H_

#ifndef _PMJMESH_MAIN_DRIVE_H_
#define _PMJMESH_MAIN_DRIVE_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/MainDrive.h"
#include "Parallel/MainDrive.h"

namespace PMJMesh
{
	class MainDrive : virtual public MJMesh::MainDrive, virtual public Parallel::MainDrive
	{
	public:

		MainDrive(Parallel::Communicator *comm = NULL,
			enum Parallel::AvailableLoadBalancers type = Parallel::GREEDY_SERIAL_STATIC);
		virtual ~MainDrive();

		virtual void setAlgorithm(enum AvailableAlgorithms algorithm);
		virtual enum AvailableAlgorithms getAlgorithm() const;

		virtual void setParallel(bool parallel);

		virtual void setMinRefinementPostponed(bool minRefinementPostponed);
		virtual bool isMinRefinementPostponed() const;

		virtual void setClassifyTreeOnConstruction(bool classifyTreeOnConstruction);

		virtual void setRangeProportion(Real rangeProportion);

		virtual void setUsePartitioningThreshold(bool usePartitioningThreshold);

		virtual void setPartitioningThreshold(Real partitioningThreshold);
		virtual void setPartitioningThreshold(UInt partitioningThreshold);

		virtual void setPercentageNonWorkingTask(Real percentageNonWorkingTask);

		virtual void setRemoveLayersInLoad(UInt removeLayersInLoad);
		virtual void setHeightDependentInLoad(bool heightDependentInLoad);

		virtual void setSmoothInSlavesEnabled(bool smoothInSlavesEnabled);

#if (!USE_DELETE_FELS)
		virtual void setUnsmoothingLayer(UInt unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)

		virtual void setSmoothingLayer(UInt smoothingLayer);

		virtual void setPartitionerType(enum PartitionerType partitionerType);
		virtual enum PartitionerType getPartitionerType() const;

		virtual void setUpdateType(enum MeshUpdateType updateType);
		virtual enum MeshUpdateType getUpdateType() const;

		virtual void setShiftType(enum ShiftType shiftType);
		virtual enum ShiftType getShiftType() const;

		virtual void setLoadBalancerType(enum Parallel::AvailableLoadBalancers type);

		virtual void setSkippingInnerInDecomposition(bool skippingInnerInDecomposition);
		virtual void setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition);

#if USE_GUI
		virtual void setBoundary(MJMesh::Boundary *boundary);
		virtual void setTree(Data::BoxTree *tree);
		virtual void setFront(MJMesh::Front *front);
		virtual void setMesh(MJMesh::Mesh *mesh);

#if USE_NEW_FRONT_UPDATER
		virtual const Data::Mesh *getUnsmoothedMesh() const;
#else
		virtual Data::Mesh *getUnsmoothedMesh() const;
#endif //#if USE_NEW_FRONT_UPDATER
#endif //#if USE_GUI

		virtual Parallel::TaskVector getTasks() const;

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual void execute();

	public:

		enum
		{
#if USE_NUMERICAL
			RUNTIME_INFO_SENT = HOTELLING_BACK_TRANSFORM_APPLIED + 1,
#else
			RUNTIME_INFO_SENT = MESH_IMPROVED + 1,
#endif //#if USE_NUMERICAL

			ERROR_PJMESH = ERROR_MESH_NOT_GENERATED_TOPOLOGY + 1,
		};

		static const Performer::Status errorPMJMesh;

		static const Performer::Status runtimeInfoSent;

	protected:

		virtual void clear();

		virtual bool executeInit();
		virtual bool executeBoundaryBuilder(bool del);
#if USE_NUMERICAL
#if DEBUG_TIMING_PMJMESH
		virtual bool executeHotellingTransform();
#endif //#if DEBUG_TIMING_PMJMESH
#endif //#if USE_NUMERICAL
		virtual bool executeTreeGenerator(bool del);
		virtual bool executeTreeMinRefiner(bool del);
		virtual bool executeTreeNeighborRefiner(bool del);
		virtual bool executeSendRuntimeInfo();
		virtual bool executeFrontGeometryAdvancer();
		virtual bool executeFrontTopologyAdvancer();
		virtual bool executeFrontBackTrackingAdvancer();
		virtual bool executeMeshImprover();
#if USE_NUMERICAL
#if DEBUG_TIMING_PMJMESH
		virtual bool executeHotellingBackTransform();
#endif //#if DEBUG_TIMING_PMJMESH
#endif //#if USE_NUMERICAL
		virtual bool executeFinalize();

#if (!USE_WORKER_FILE_LOADER)
		virtual InputData *makeInputData() const;
		virtual InputData *makeAndFillInputData() const;
		virtual void fillWithInputData(const InputData *input);

		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
#endif //#if (!USE_WORKER_FILE_LOADER)
#if USE_NUMERICAL
        virtual MJMesh::HotellingTransformer *makeHotellingTransformer() const;
#endif //#if USE_NUMERICAL
		virtual Performer::TreeGenerator *makeTreeGenerator() const;
		virtual MJMesh::MinTreeRefiner *makeTreeMinRefiner() const;
		virtual MJMesh::NeighborTreeRefiner *makeTreeNeighborRefiner() const;
		virtual Parallel::Communicator *makeCommunicator() const;
		virtual Parallel::LoadBalancer *makeLoadBalancer() const;
		virtual TaskManager *makeTaskManager() const;

	protected:

		///TODO verify if this attribute can be removed from here and from
		///  wherever it appears consequently
		bool parallel;

		bool classifyTreeOnConstruction;

		Real rangeProportion;

		bool minRefinementPostponed;
		UInt minRefinementLevel;

		bool usePartitioningThreshold;
		Real partitioningThreshold;

		Real percentageNonWorkingTask;

		UInt removeLayersInLoad;
		bool heightDependentInLoad;

		bool smoothInSlavesEnabled;
#if (!USE_DELETE_FELS)
		UInt unsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)
		UInt smoothingLayer;

		enum PartitionerType partitionerType;
		enum MeshUpdateType updateType;
		enum ShiftType shiftType;

#if USE_TIMING
		Real treeGenerationTime;
#endif //#if USE_TIMING

		enum AvailableAlgorithms algorithm;
		enum Parallel::AvailableLoadBalancers loadBalancerType;

		bool skippingInnerInDecomposition;
		bool createInnerFrontInDecomposition;

		Parallel::LoadBalancer *balancer;

		TaskManager *geometryManager;

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
		FEListHashMap *elementsMap;
#else
		FEListMap *elementsMap;
#endif //#if USE_C__11
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if DEBUG_TIMING_PMJMESH
	protected:

		Real timePMJMeshInit,
			timePMJMeshBoundaryBuilder,
#if USE_NUMERICAL
			timePMJMeshHotellingTransformer,
#endif //#if USE_NUMERICAL
			timePMJMeshTreeGeneratorGeneration,
			timePMJMeshTreeGeneratorClassification,
			timePMJMeshTreeGeneratorOthers,
			timePMJMeshMeshGeneratorMasterTreeDecomposition,
			timePMJMeshMeshGeneratorMasterFrontDecomposition,
			timePMJMeshMeshGeneratorWorkerMeshGeneration,
			timePMJMeshMeshGeneratorWorkerMeshImprovement,
			timePMJMeshMeshGeneratorWorkerTreeClassification,
			timePMJMeshMeshGeneratorWorkerOthers,
			timePMJMeshMeshGeneratorMasterSendTasks,
			timePMJMeshMeshGeneratorMasterReceiveTasks,
			timePMJMeshMeshGeneratorMasterMeshUpdate,
			timePMJMeshMeshGeneratorMasterTreeShift,
			timePMJMeshMeshGeneratorMasterFinalization,
			timePMJMeshMeshGeneratorMasterOthers,
			timePMJMeshMeshImprovement,
#if USE_NUMERICAL
			timePMJMeshHotellingBackTransformer,
#endif //#if USE_NUMERICAL
			timePMJMeshOverhead;
#endif //#if DEBUG_TIMING_PMJMESH
	};
}

#endif //#ifndef _PMJMESH_MAIN_DRIVE_H_

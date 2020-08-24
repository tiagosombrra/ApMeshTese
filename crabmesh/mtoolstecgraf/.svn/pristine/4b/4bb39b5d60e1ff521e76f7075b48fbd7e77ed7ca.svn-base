#ifndef _PMJMESH2_MAIN_DRIVE_H_
#define _PMJMESH2_MAIN_DRIVE_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh/MainDrive.h"

namespace PMJMesh2
{
    class MainDrive : virtual public MJMesh::MainDrive, virtual public Parallel::MainDrive
    {
    public:

        MainDrive(Parallel::Communicator *comm = NULL);
        virtual ~MainDrive();

        virtual void setNumThreads(UInt numThreads);
        virtual UInt getNumThreads() const;

        virtual void setNumTasksPerThread(UInt numTasksPerThread);
        virtual UInt getNumTasksPerThread() const;

        virtual void setMinNumTasks(UInt minNumTasks);

        virtual void setHeightDependentInLoad(bool heightDependentInLoad);
        virtual bool isHeightDependentInLoad() const;

        virtual void setRemoveLayersInLoad(UInt removeLayersInLoad);
        virtual UInt getRemoveLayersInLoad() const;

#if USE_REMOVED_LAYERS_PROPORTIONS
        virtual void setRemovedLayersProportions(UInt i, Real proportion);
        virtual Real getRemovedLayersProportions(UInt i) const;

        virtual void setRemovedLayersProportions(const Data::RealVector &removedLayersProportions);
        virtual const Data::RealVector &getRemovedLayersProportions() const;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        virtual void setLoadThreshold(Real loadThreshold);
        virtual Real getLoadThreshold() const;

        virtual void setPartitionerType(enum PMJMesh2::PartitionerType partitionerType);
        virtual enum PMJMesh2::PartitionerType getPartitionerType();

#if USE_FILE_BASED_BSP
        virtual void setPartitionBSPFileName(const std::string &partitionBSPFileName);
        virtual std::string getPartitionBSPFileName() const;
#endif //#if USE_FILE_BASED_BSP

        virtual void setInterfaceMeshGenerationType(enum PMJMesh2::InterfaceMeshGenerationType interfaceMeshGenerationType);
        virtual enum PMJMesh2::InterfaceMeshGenerationType getInterfaceMeshGenerationType();

        virtual void setCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition);
        virtual bool isCreatingInnerFrontInDecomposition() const;

        virtual void setRangeProportion(Real rangeProportion);
        virtual Real getRangeProportion() const;

        virtual void setImprovementInSlavesEnabled(bool improvementInSlavesEnabled);
        virtual bool isImprovementInSlavesEnabled() const;

        virtual void setImprovementLayers(UInt improvementLayers);
        virtual UInt getImprovementLayers() const;

        virtual void setUpdateType(enum PMJMesh2::MeshUpdateType updateType);
        virtual enum PMJMesh2::MeshUpdateType getUpdateType();

        virtual void setFinalMeshState(enum FinalMeshState finalMeshState);
        virtual enum FinalMeshState getFinalMeshState() const;

        virtual void setAPrioriInterfaceElementSizeFactor(Real aPrioriInterfaceElementSizeFactor);
        virtual Real getAPrioriInterfaceElementSizeFactor() const;

        virtual void setAPrioriInterfaceBadAngleCos(Real aPrioriInterfaceBadAngleCos);
        virtual Real getAPrioriInterfaceBadAngleCos() const;

		virtual void setAPrioriInterfaceShiftDelta(Real aPrioriInterfaceShiftDelta);
		virtual Real getAPrioriInterfaceShiftDelta() const;

        virtual const PartitionBSPTree *getPartitionBSPTree() const;
        virtual const Parallel::TaskVector &getTasks() const;

#if DEBUG_CELL_COUNTER
        virtual void setOnlyCount(bool onlyCount);
#endif //#if DEBUG_CELL_COUNTER

        virtual bool loadFile();
		virtual bool saveFile() const;

		virtual void execute();

    public:

        enum
		{
#if USE_NUMERICAL
			LOAD_CALCULATED = HOTELLING_BACK_TRANSFORM_APPLIED + 1,
#else
			LOAD_CALCULATED = MESH_IMPROVED + 1,
#endif //#if USE_NUMERICAL
			PARTITION_TREE_GENERATED,
			MESH_GENERATED,

			ERROR_PJMESH2 = ERROR_MESH_NOT_GENERATED_TOPOLOGY + 1,
		};

		static const Performer::Status errorPMJMesh2;

		static const Performer::Status loadCalculated;
		static const Performer::Status partitionTreeGenerated;
		static const Performer::Status meshGenerated;

    protected:

        virtual void clear();

#if (!USE_WORKER_FILE_LOADER)
		virtual InputData *makeInputData() const;
		virtual InputData *makeAndFillInputData() const;
		virtual void fillWithInputData(const InputData *input);

		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
#endif //#if (!USE_WORKER_FILE_LOADER)

        virtual Parallel::Communicator *makeCommunicator() const;
        virtual Parallel::LoadBalancer *makeLoadBalancer() const;

#if USE_NUMERICAL
        virtual MJMesh::HotellingTransformer *makeHotellingTransformer() const;
#endif //#if USE_NUMERICAL
        virtual Performer::TreeGenerator *makeTreeGenerator() const;
		virtual MJMesh::MinTreeRefiner *makeTreeMinRefiner() const;
		virtual MJMesh::NeighborTreeRefiner *makeTreeNeighborRefiner() const;
        virtual PMJMesh::LoadCalculator *makeLoadCalculator() const;
        virtual PartitionBSPTreeGenerator *makePartitionTreeGenerator() const;
        virtual ParallelMeshGenerator *makeMeshGenerator() const;

        virtual PMJMesh::MJMeshTask *makeTask(PMJMesh::Partition *p, PartitionBSPTreeCell *cell) const;

        virtual bool executeInit();
        virtual bool executeBoundaryBuilder(bool del);
		virtual bool executeTreeGenerator(bool del);
		virtual bool executeLoadCalculator();
		virtual bool executePartitionTreeGenerator();
		virtual bool executeParallelMeshGenerator();
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeFinalize();

    protected:

        mutable UInt numThreads;

        UInt numTasksPerThread;

        bool heightDependentInLoad;

        UInt removeLayersInLoad;
#if USE_REMOVED_LAYERS_PROPORTIONS
        Data::RealVector removedLayersProportions;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        Real loadThreshold;

        enum PartitionerType partitionerType;
#if USE_FILE_BASED_BSP
        std::string partitionBSPFileName;
#endif //#if USE_FILE_BASED_BSP

        enum InterfaceMeshGenerationType interfaceMeshGenerationType;

        bool creatingInnerFrontInDecomposition;

        Real rangeProportion;

		bool improvementInSlavesEnabled;
		UInt improvementLayers;

		enum MeshUpdateType updateType;

		enum FinalMeshState finalMeshState;

		//a priori only
		Real aPrioriInterfaceElementSizeFactor;
		Real aPrioriInterfaceBadAngleCos;
		Real aPrioriInterfaceShiftDelta;

    protected:

        PartitionBSPTree *partitionTree;

        PMJMesh::LoadCalculator *loadCalculator;
        PartitionBSPTreeGenerator *partitionTreeGenerator;
        ParallelMeshGenerator *meshGenerator;

        Parallel::TaskVector tasks;

    protected:

#if USE_C__11
        PMJMesh::FEListHashMap *elementsMap;
#else
        PMJMesh::FEListMap *elementsMap;
#endif //#if USE_C__11

    protected:

#if DEBUG_CELL_COUNTER
        bool onlyCount;
#endif //#if DEBUG_CELL_COUNTER

    };
}

#endif //#ifndef _PMJMESH2_MAIN_DRIVE_H_

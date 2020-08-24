#ifndef _PMJMESH2_INPUT_DATA_H_
#define _PMJMESH2_INPUT_DATA_H_

#include "PMJMesh2/Definitions.h"
#include "Parallel/Transferable.h"

namespace PMJMesh2
{
    class InputData : public Parallel::Transferable
    {
    public:

        InputData();
        virtual ~InputData();

        virtual void setInputFileName(const std::string &inputFileName);
        virtual const std::string &getInputFileName() const;

        virtual void setDimension(UInt dimension);
        virtual UInt getDimension() const;

        virtual void setIdVertex(ULInt idVertex);
        virtual ULInt getIdVertex() const;

        virtual void setIdFrontElement(ULInt idFrontElement);
        virtual ULInt getIdFrontElement() const;

        virtual void setIdElement(ULInt idElement);
        virtual ULInt getIdElement() const;

        virtual void setBoundarySorted(bool boundarySorted);
		virtual bool isBoundarySorted() const;

#if USE_NUMERICAL
		virtual void setHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool isHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

		virtual void setTreeFactor(Real treeFactor);
		virtual Real getTreeFactor() const;

		virtual void setUsingSearchTrees(bool usingSearchTrees);
		virtual bool isUsingSearchTrees() const;

		virtual void setNumImproves(UInt numImproves);
		virtual UInt getNumImproves() const;

		virtual void setSmoothingFactor(Real smoothingFactor);
		virtual Real getSmoothingFactor() const;

		virtual void setMeshSorted(bool meshSorted);
		virtual bool isMeshSorted() const;

		virtual void setLocalBackTrackingEnabled(bool localBackTrackingEnabled);
		virtual bool isLocalBackTrackingEnabled() const;

		virtual void setMaxBackTracks(UInt maxBackTracks);
		virtual UInt getMaxBackTracks() const;

        virtual void setHeightDependentInLoad(bool heightDependentInLoad);
        virtual bool isHeightDependentInLoad() const;

        virtual void setRemoveLayersInLoad(UInt removeLayersInLoad);
        virtual UInt getRemoveLayersInLoad() const;

#if USE_REMOVED_LAYERS_PROPORTIONS
        virtual void setRemovedLayersProportions(const Data::RealVector &removedLayersProportions);
        virtual const Data::RealVector &getRemovedLayersProportions() const;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        virtual void setLoadThreshold(Real loadThreshold);
        virtual Real getLoadThreshold() const;

        virtual void setInterfaceBadAngleCos(Real interfaceBadAngleCos);
        virtual Real getInterfaceBadAngleCos() const;

		virtual void setInterfaceShiftDelta(Real interfaceShiftDelta);
		virtual Real getInterfaceShiftDelta() const;

        virtual void setPartitionerType(enum PartitionerType partitionerType);
        virtual enum PartitionerType getPartitionerType() const;

#if USE_FILE_BASED_BSP
        virtual void setPartitionBSPFileName(const std::string &partitionBSPFileName);
        virtual std::string getPartitionBSPFileName() const;
#endif //#if USE_FILE_BASED_BSP

        virtual void setInterfaceMeshGenerationType(enum InterfaceMeshGenerationType interfaceMeshGenerationType);
        virtual enum InterfaceMeshGenerationType getInterfaceMeshGenerationType() const;

        virtual void setInterfaceElementSizeFactor(Real interfaceElementSizeFactor);
        virtual Real getInterfaceElementSizeFactor() const;

        virtual void setSharedParallelismEnabled(bool sharedParallelismEnabled);
        virtual bool isSharedParallelismEnabled() const;

        virtual void setNumThreads(UInt numThreads);
        virtual UInt getNumThreads() const;

        virtual void setNumTasksPerThread(UInt numTasksPerThread);
        virtual UInt getNumTasksPerThread() const;

        virtual void setCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition);
        virtual bool isCreatingInnerFrontInDecomposition() const;

        virtual void setRangeProportion(Real rangeProportion);
		virtual Real getRangeProportion() const;

		virtual void setImprovementInSlavesEnabled(bool improvementInSlavesEnabled);
		virtual bool isImprovementInSlavesEnabled() const;

		virtual void setImprovementLayers(UInt improvementLayers);
		virtual UInt getImprovementLayers() const;

		virtual void setUpdateType(enum MeshUpdateType updateType);
		virtual enum MeshUpdateType getUpdateType() const;

        virtual void setFinalMeshState(enum FinalMeshState finalMeshState);
        virtual enum FinalMeshState getFinalMeshState() const;

		virtual void setTolerance(Real tolerance);
		virtual Real getTolerance() const;

#if USE_GUI
		virtual void setStepByStep(bool stepByStep);
		virtual bool isStepByStep() const;
#endif //#if USE_GUI

        virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

    protected:

        std::string inputFileName;

        UInt dimension;

        ULInt idVertex;
		ULInt idFrontElement;
		ULInt idElement;

		bool boundarySorted;
#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		Real treeFactor;
		bool usingSearchTrees;
		UInt numImproves;
		Real smoothingFactor;
		bool meshSorted;
		bool localBackTrackingEnabled;
		UInt maxBackTracks;

        bool heightDependentInLoad;

        UInt removeLayersInLoad;
#if USE_REMOVED_LAYERS_PROPORTIONS
        Data::RealVector removedLayersProportions;
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

        Real loadThreshold;
        Real interfaceBadAngleCos;
		Real interfaceShiftDelta;

        enum PartitionerType partitionerType;
#if USE_FILE_BASED_BSP
        std::string partitionBSPFileName;
#endif //#if USE_FILE_BASED_BSP

        enum InterfaceMeshGenerationType interfaceMeshGenerationType;
        Real interfaceElementSizeFactor;

        bool sharedParallelismEnabled;

        UInt numThreads;

        UInt numTasksPerThread;

        bool creatingInnerFrontInDecomposition;

        Real rangeProportion;

		bool improvementInSlavesEnabled;
		UInt improvementLayers;

		enum MeshUpdateType updateType;

		enum FinalMeshState finalMeshState;

        Real tolerance;

#if USE_GUI
		bool stepByStep;
#endif //#if USE_GUI

    };
}

#endif //#ifndef _PMJMESH2_INPUT_DATA_H_

#ifndef _PMJMESH_INPUT_DATA_H_
#define _PMJMESH_INPUT_DATA_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/MainDrive.h"
#include "Parallel/Transferable.h"

namespace PMJMesh
{
	class InputData : public Parallel::Transferable
	{
	public:

		InputData();
		virtual ~InputData();

		virtual void setIdVertex(ULInt idVertex);
		virtual ULInt getIdVertex() const;

		virtual void setIdFrontElement(ULInt idFrontElement);
		virtual ULInt getIdFrontElement() const;

		virtual void setIdElement(ULInt idElement);
		virtual ULInt getIdElement() const;

		virtual void setDimension(UInt dimension);
		virtual UInt getDimension() const;

		virtual void setAlgorithm(enum AvailableAlgorithms algorithm);
		virtual enum AvailableAlgorithms getAlgorithm() const;

		virtual void setBoundarySorted(bool boundarySorted);
		virtual bool isBoundarySorted() const;

#if USE_NUMERICAL
		virtual void setHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool isHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

		virtual void setTreeFactor(Real treeFactor);
		virtual Real getTreeFactor() const;

		virtual void setSmoothInSlavesEnabled(bool smoothInSlavesEnabled);
		virtual bool isSmoothInSlavesEnabled() const;

		virtual void setUsingSearchTrees(bool usingSearchTrees);
		virtual bool isUsingSearchTrees() const;

		virtual void setRangeProportion(Real rangeProportion);
		virtual Real getRangeProportion() const;

		virtual void setNumImproves(UInt numImproves);
		virtual UInt getNumImproves() const;

		virtual void setSmoothingFactor(Real smoothingFactor);
		virtual Real getSmoothingFactor() const;

#if (!USE_DELETE_FELS)
		virtual void setUnsmoothingLayer(UInt unsmoothingLayer);
		virtual UInt getUnsmoothingLayer() const;
#endif //#if (!USE_DELETE_FELS)

		virtual void setSmoothingLayer(UInt smoothingLayer);
		virtual UInt getSmoothingLayer() const;

		virtual void setMeshSorted(bool meshSorted);
		virtual bool isMeshSorted() const;

		virtual void setLocalBackTrackingEnabled(bool localBackTrackingEnabled);
		virtual bool isLocalBackTrackingEnabled() const;

		virtual void setMaxBackTracks(UInt maxBackTracks);
		virtual UInt getMaxBackTracks() const;

		virtual void setUpdateType(enum MeshUpdateType updateType);
		virtual enum MeshUpdateType getUpdateType() const;

		virtual void setParallel(bool parallel);
		virtual bool isParallel() const;

		virtual void setMinRefinementPostponed(bool minRefinementPostponed);
		virtual bool isMinRefinementPostponed() const;

		virtual void setUsePartitioningThreshold(bool usePartitioningThreshold);
		virtual bool isUsePartitioningThreshold() const;

		virtual void setPartitioningThreshold(Real partitioningThreshold);
		virtual Real getPartitioningThreshold() const;

		virtual void setLoadBalancer(enum Parallel::AvailableLoadBalancers loadBalancer);
		virtual enum Parallel::AvailableLoadBalancers getLoadBalancer() const;

		virtual void setSkippingInnerInDecomposition(bool skippingInnerInDecomposition);
		virtual bool isSkippingInnerInDecomposition() const;

		virtual void setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition);
		virtual bool isCreateInnerFrontInDecomposition() const;

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

		ULInt idVertex;
		ULInt idFrontElement;
		ULInt idElement;

		UInt dimension;

		enum AvailableAlgorithms algorithm;

		bool boundarySorted;
#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		Real treeFactor;
		bool usingSearchTrees;
		Real rangeProportion;

		bool smoothInSlavesEnabled;
		UInt numImproves;
		Real smoothingFactor;
#if (!USE_DELETE_FELS)
		UInt unsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)
		UInt smoothingLayer;
		bool meshSorted;
		bool localBackTrackingEnabled;
		UInt maxBackTracks;

		enum MeshUpdateType updateType;

		bool parallel;
		bool minRefinementPostponed;
		bool usePartitioningThreshold;
		Real partitioningThreshold;
		enum Parallel::AvailableLoadBalancers loadBalancer;

		bool skippingInnerInDecomposition;
		bool createInnerFrontInDecomposition;

		Real tolerance;
#if USE_GUI
		bool stepByStep;
#endif //#if USE_GUI
	};
}

#endif //#ifndef _PMJMESH_INPUT_DATA_H_

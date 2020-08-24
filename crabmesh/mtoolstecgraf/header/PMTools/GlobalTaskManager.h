#ifndef _PMTOOLS_GLOBAL_TASK_MANAGER_H_
#define _PMTOOLS_GLOBAL_TASK_MANAGER_H_

#include "PMTools/Definitions.h"

#if (USE_MTOOLS)

#include "PMJMesh/GlobalTaskManager.h"
#include "OctTree.hpp"

namespace PMTools
{
	class GlobalTaskManager : public PMJMesh::GlobalTaskManager
	{
	public:

		GlobalTaskManager(const FTools::OctTree *ftree, bool sharedParallel, UInt dimension,
			MJMesh::Boundary *boundary, Data::BoxTree *tree, Real percentageNonWorkingTask,
			bool heightDependentInLoad, UInt removeLayersInLoad, bool smoothInSlavesEnabled, UInt numImproves,
			Real smoothingFactor,
#if (!USE_DELETE_FELS)
			UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			UInt smoothingLayer,
			bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks,
			enum PMJMesh::PartitionerType partitionerType,
			enum PMJMesh::MeshUpdateType updateType, enum PMJMesh::ShiftType shiftType,
			bool usePartitioningThreshold, Real maxAllowedLoad,
			bool sorted, bool usingSearchTrees, Real rangeProportion,
			UInt minRefinementLevel, bool skippingInnerInDecomposition,
			bool createInnerFrontInDecomposition,
			enum Parallel::AvailableLoadBalancers balancerType,
			Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm);
		virtual ~GlobalTaskManager();

		virtual Parallel::Task *makeTask() const;

		virtual void finalizeMesh();

	protected:

		virtual bool checkInnerFront(const Data::VertexList &vertices, const Data::FrontElementList &fels,
			const PMJMesh::Partition *partition, Real size) const;

#if USE_NEW_FRONT_UPDATER
		virtual PMJMesh::ActualFrontUpdater *makeFrontUpdater() const;
		virtual PMJMesh::MeshUpdater *makeMeshUpdater() const;
#endif //#if USE_NEW_FRONT_UPDATER

		virtual bool setTaskParameters(Parallel::Task *t,
			MJMesh::Boundary *boundary, Data::BoxTree *tree,
			Performer::IdManager *idManager) const;

		virtual void balanceLoad(bool shared);
		virtual void executeGeometryGeneration();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		virtual std::string timing(const Parallel::Task *task, Real timeTotal) const;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	protected:

		const FTools::OctTree *ftree;

		bool disableBackTrackingInInterTasks;

	};
}

#endif //#if (USE_MTOOLS)

#endif //#ifndef _PMTOOLS_GLOBAL_TASK_MANAGER_H_

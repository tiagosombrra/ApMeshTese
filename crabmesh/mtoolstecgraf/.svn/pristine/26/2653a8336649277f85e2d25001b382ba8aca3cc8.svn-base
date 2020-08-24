#ifndef _PMJMESH_WORKER_MESH_IMPROVER_H_
#define _PMJMESH_WORKER_MESH_IMPROVER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/MeshImprover.h"

namespace PMJMesh
{
	class WorkerMeshImprover : public MJMesh::MeshImprover
	{
	public:

#if USE_DELETE_FELS
	WorkerMeshImprover(UInt dimension, MJMesh::Mesh *mesh,
			MJMesh::Boundary *boundary, Data::BoxTree *tree, bool deleteBoundary,
			UInt numImproves, Real smoothingFactor, bool localBackTrackingEnabled,
			bool meshSorted, UInt maxBackTracks);
#else
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	WorkerMeshImprover(UInt dimension, MJMesh::Mesh *mesh,
			UInt unsmoothingLayer, MJMesh::Boundary *boundary, Data::BoxTree *tree,
			bool removeBoundaryFelsForSmoothing, bool deleteBoundary,
			UInt numImproves, Real smoothingFactor, bool localBackTrackingEnabled,
			bool meshSorted, UInt maxBackTracks);
#else
		WorkerMeshImprover(UInt dimension, MJMesh::Mesh *mesh,
			UInt unsmoothingLayer, const Data::FrontElementList &frontElements,
			Data::BoxTree *tree, bool removeBoundaryFelsForSmoothing, UInt numImproves,
			Real smoothingFactor, bool localBackTrackingEnabled,
			bool meshSorted, UInt maxBackTracks);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if USE_DELETE_FELS
		virtual ~WorkerMeshImprover();

#if (!USE_DELETE_FELS)
		virtual void setUnsmoothingLayer(UInt unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
		virtual void setDeleteBoundary(bool deleteBoundary);
#else
		virtual void setFrontElements(const Data::FrontElementList &frontElements);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

#if (!USE_DELETE_FELS)
		virtual void setRemoveBoundaryFelsForSmoothing(bool removeBoundaryFelsForSmoothing);
#endif //#if (!USE_DELETE_FELS)

		virtual void execute();

	protected:

#if (!USE_DELETE_FELS)
		UInt unsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
		bool deleteBoundary;
#else
		Data::FrontElementList frontElements;
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

#if (!USE_DELETE_FELS)
		//this will be true only for the shared memory version, not the
		// distributed (or hybrid) memory version.
		bool removeBoundaryFelsForSmoothing;
#endif //#if (!USE_DELETE_FELS)
	};
}

#endif //#ifndef _PMJMESH_WORKER_LAPLACIAN_SMOOTHER_H_

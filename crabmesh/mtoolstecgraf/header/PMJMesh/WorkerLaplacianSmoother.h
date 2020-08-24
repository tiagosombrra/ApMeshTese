#ifndef _PMJMESH_WORKER_LAPLACIAN_SMOOTHER_H_
#define _PMJMESH_WORKER_LAPLACIAN_SMOOTHER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/LaplacianSmoother.h"

namespace PMJMesh
{
	class WorkerLaplacianSmoother : public MJMesh::LaplacianSmoother
	{
	public:

#if USE_DELETE_FELS
    WorkerLaplacianSmoother(UInt dimension, Data::Mesh *mesh = NULL, bool testAdjacency = true,
			//Real factor = 0.5);
			Real factor = 1.0);
#else
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	WorkerLaplacianSmoother(UInt dimension, const MJMesh::Boundary *boundary,
			bool removeForSmoothing, UInt unsmoothingLayer = 0,
			Data::Mesh *mesh = NULL, bool testAdjacency = true,
			//Real factor = 0.5);
			Real factor = 1.0);
#else
		WorkerLaplacianSmoother(UInt dimension, const Data::VertexList &front,
			const Data::FrontElementList &boundary, UInt unsmoothingLayer = 0,
			Data::Mesh *mesh = NULL, bool testAdjacency = true,
			//Real factor = 0.5);
			Real factor = 1.0);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if USE_DELETE_FELS
		virtual ~WorkerLaplacianSmoother();

#if (!USE_DELETE_FELS)
		virtual void setUnsmoothingLayer(UInt unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)

#if (!USE_DELETE_FELS)
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
		virtual void setBoundary(const MJMesh::Boundary *boundary);
		virtual void setRemoveForSmoothing(bool removeForSmoothing);
#else
		virtual void setFront(const Data::VertexList &front);
		virtual void setBoundary(const Data::FrontElementList &boundary);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if (!USE_DELETE_FELS)

#if (!USE_DELETE_FELS)
		virtual void findLayers();
#endif //#if (!USE_DELETE_FELS)

		virtual void execute();

	protected:

#if (!USE_DELETE_FELS)
#if USE_ATTRIBS
		virtual void clearAttribs() const;
#endif //#if USE_ATTRIBS

		//virtual Data::VertexList findNewInnerVertices() const;
		virtual void removeLayers() const;
		virtual void addLayers() const;
#endif //#if (!USE_DELETE_FELS)

	protected:

#if (!USE_DELETE_FELS)
		UInt unsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)

#if (!USE_DELETE_FELS)
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
		const MJMesh::Boundary *boundary;
		bool removeForSmoothing;
#else
		Data::VertexList front;
		Data::FrontElementList boundary;
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if (!USE_DELETE_FELS)

#if (!USE_DELETE_FELS)
		//Data::VertexList oldInnerVertices;
#if USE_C__11
		Data::VertexHash removed;
#else
		Data::VertexSet removed;
#endif //#if USE_C__11
#endif //#if (!USE_DELETE_FELS)
	};
}

#endif //#ifndef _PMJMESH_WORKER_LAPLACIAN_SMOOTHER_H_

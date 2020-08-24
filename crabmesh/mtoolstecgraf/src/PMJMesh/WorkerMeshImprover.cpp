#include "PMJMesh/WorkerMeshImprover.h"

#include "PMJMesh/WorkerLaplacianSmoother.h"
#include "PMJMesh/MeshBackTracker.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "Data/FrontElement.h"

#if USE_DELETE_FELS
PMJMesh::WorkerMeshImprover::WorkerMeshImprover(UInt dimension, MJMesh::Mesh *mesh,
	MJMesh::Boundary *boundary, Data::BoxTree *tree,
	bool deleteBoundary, UInt numImproves, Real smoothingFactor,
	bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks) :
	MJMesh::MeshImprover(dimension, mesh, NULL, tree, boundary, numImproves,
		smoothingFactor, localBackTrackingEnabled, meshSorted, maxBackTracks)
#else
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
PMJMesh::WorkerMeshImprover::WorkerMeshImprover(UInt dimension, MJMesh::Mesh *mesh,
	UInt unsmoothingLayer, MJMesh::Boundary *boundary, Data::BoxTree *tree,
	bool removeBoundaryFelsForSmoothing, bool deleteBoundary,
	UInt numImproves, Real smoothingFactor,
	bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks) :
	MJMesh::MeshImprover(dimension, mesh, NULL, tree, boundary, numImproves,
		smoothingFactor, localBackTrackingEnabled, meshSorted, maxBackTracks)
#else
PMJMesh::WorkerMeshImprover::WorkerMeshImprover(UInt dimension, MJMesh::Mesh *mesh,
	UInt unsmoothingLayer, const Data::FrontElementList &frontElements, Data::BoxTree *tree,
	bool removeBoundaryFelsForSmoothing, UInt numImproves, Real smoothingFactor,
	bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks) :
	MJMesh::MeshImprover(dimension, mesh, NULL, tree, NULL, numImproves,
		smoothingFactor, localBackTrackingEnabled, meshSorted, maxBackTracks)
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif // USE_DELETE_FELS
{
#if (!USE_DELETE_FELS)
	this->setUnsmoothingLayer(unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	this->setDeleteBoundary(deleteBoundary);
#else
	this->setFrontElements(frontElements);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

#if (!USE_DELETE_FELS)
	this->setRemoveBoundaryFelsForSmoothing(removeBoundaryFelsForSmoothing);
#endif //#if (!USE_DELETE_FELS)
}

PMJMesh::WorkerMeshImprover::~WorkerMeshImprover()
{

}

#if (!USE_DELETE_FELS)
void PMJMesh::WorkerMeshImprover::setUnsmoothingLayer(UInt unsmoothingLayer)
{
	this->unsmoothingLayer = unsmoothingLayer;
}
#endif //#if (!USE_DELETE_FELS)

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
void PMJMesh::WorkerMeshImprover::setDeleteBoundary(bool deleteBoundary)
{
	this->deleteBoundary = deleteBoundary;
}
#else
void PMJMesh::WorkerMeshImprover::setFrontElements(const Data::FrontElementList &frontElements)
{
	this->frontElements = frontElements;
}
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

#if (!USE_DELETE_FELS)
void PMJMesh::WorkerMeshImprover::setRemoveBoundaryFelsForSmoothing(bool removeBoundaryFelsForSmoothing)
{
	this->removeBoundaryFelsForSmoothing = removeBoundaryFelsForSmoothing;
}
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::WorkerMeshImprover::execute()
{
	//std::stringstream str;
	//Real debugstart = Data::time();

	if (this->status.getType() == this->notInit.getType())
	{
		if (!this->mesh)
		{
			this->status = Performer::MeshImprover::ended;

			//str << "Worker, improver, time for not executing = " << Data::time() - debugstart << std::endl;
			//std::cout << str.str();

			return;
		}

#if ((!USE_NEW_FRONT_UPDATER) /*|| true*/)
		Data::VertexSet verticesSet;

		for (Data::FrontElementList::iterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			for (UInt i = 0; i < (*iter)->numVertices(); i++)
			{
				verticesSet.insert((*iter)->getVertex(i));
			}
		}

		//str << "Worker, improver, time for creating vertices set = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		Data::VertexList vertices(verticesSet.begin(), verticesSet.end());

		//str << "Worker, improver, time for creating vertices list = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();
#endif //#if ((!USE_NEW_FRONT_UPDATER) /*|| true*/)

#if USE_DELETE_FELS
		PMJMesh::WorkerLaplacianSmoother *smoother = new PMJMesh::WorkerLaplacianSmoother(
			this->dimension, this->mesh, true, this->smoothingFactor);
#else
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
		PMJMesh::WorkerLaplacianSmoother *smoother = new PMJMesh::WorkerLaplacianSmoother(
			this->dimension, this->boundary, this->removeBoundaryFelsForSmoothing,
			this->unsmoothingLayer, this->mesh, true, this->smoothingFactor);
#else
		PMJMesh::WorkerLaplacianSmoother *smoother = new PMJMesh::WorkerLaplacianSmoother(
			this->dimension, vertices,
			this->removeBoundaryFelsForSmoothing ? this->frontElements : Data::FrontElementList(),
			this->unsmoothingLayer, this->mesh, true, this->smoothingFactor);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if USE_DELETE_FELS

		//str << "Worker, improver, time for creating smoother = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if (!USE_DELETE_FELS)
		if ((this->removeBoundaryFelsForSmoothing) && (this->unsmoothingLayer == 0))
		{
			smoother->findLayers();

			//str << "Worker, improver, time for smoother to find layers = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();
		}
#endif //#if (!USE_DELETE_FELS)

		this->add(smoother);
			//this->unsmoothingLayer, this->mesh, (this->dimension != 2), this->smoothingFactor));

		//str << "Worker, improver, time for adding smoother = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		if (this->localBackTrackingEnabled)
		{
#if ((!USE_NEW_FRONT_UPDATER) /*|| true*/)
			this->boundary = new MJMesh::Boundary();

			//Data::VertexVector vs(vertices.begin(), vertices.end());
			//Data::FrontElementVector fes(this->frontElements.begin(), this->frontElements.end());

			//this->boundary->setVertices(vs);
			this->boundary->setVertices(vertices);
			//this->boundary->setElements(fes);
			this->boundary->setElements(this->frontElements);

			//str << "Worker, improver, time for creating new boundary = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();
#endif //#if ((!USE_NEW_FRONT_UPDATER) /*|| true*/)

			PMJMesh::MeshBackTracker *opt = new PMJMesh::MeshBackTracker(this->dimension,
				this->mesh, this->boundary, this->tree,
				this->front, this->meshSorted, this->maxBackTracks);

			opt->setIdManager(this->idManager);

			//str << "Worker, improver, time for creating back tracker = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			this->add(opt);

			//str << "Worker, improver, time for adding back tracker = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();
		}

		this->status = this->init;
	}

	MJMesh::MeshImprover::execute();

	//str << "Worker, improver, time for executing = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	if (this->status.getType() == this->ended.getType())
	{
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
		if (this->deleteBoundary)
		{
			this->boundary->clear();

			//str << "Worker, improver, time for clearing boundary = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			delete this->boundary;

			this->boundary = NULL;

			//str << "Worker, improver, time for deleting boundary = " << Data::time() - debugstart << std::endl;
		}
#else
		if (this->boundary)
		{
			//this->boundary->setVertices(Data::VertexVector());
			this->boundary->setVertices(Data::VertexList());
			//this->boundary->setElements(Data::FrontElementVector());
			this->boundary->setElements(Data::FrontElementList());

			//str << "Worker, improver, time for reseting boundary vertices and front elements = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			delete this->boundary;

			this->boundary = NULL;

			//str << "Worker, improver, time for deleting boundary = " << Data::time() - debugstart << std::endl;
		}
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	}

	//std::cout << str.str();
}

#include "MJMesh/MeshImprover.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/MeshBackTracker.h"
#include "MJMesh/LaplacianSmoother.h"

MJMesh::MeshImprover::MeshImprover(UInt dimension, MJMesh::Mesh *mesh,
	MJMesh::Front *front, Data::BoxTree *tree, MJMesh::Boundary *boundary,
	UInt numImproves, Real smoothingFactor, bool localBackTrackingEnabled,
	bool meshSorted, UInt maxBackTracks) :
	Performer::MeshImprover(mesh, numImproves)
{
	this->setDimension(dimension);
	this->setFront(front);
	this->setTree(tree);
	this->setBoundary(boundary);
	this->setSmoothingFactor(smoothingFactor);
	this->setLocalBackTrackingEnabled(localBackTrackingEnabled);
	this->setMeshSorted(meshSorted);
	this->setMaxBackTracks(maxBackTracks);

	this->idManager = NULL;
}

MJMesh::MeshImprover::~MeshImprover()
{

}

void MJMesh::MeshImprover::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void MJMesh::MeshImprover::setFront(MJMesh::Front *front)
{
	this->front = front;
}

void MJMesh::MeshImprover::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}

void MJMesh::MeshImprover::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void MJMesh::MeshImprover::setSmoothingFactor(Real smoothingFactor)
{
	this->smoothingFactor = smoothingFactor;
}

void MJMesh::MeshImprover::setMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;
}

void MJMesh::MeshImprover::setLocalBackTrackingEnabled(bool localBackTrackingEnabled)
{
	this->localBackTrackingEnabled = localBackTrackingEnabled;
}

void MJMesh::MeshImprover::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

void MJMesh::MeshImprover::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

void MJMesh::MeshImprover::execute()
{
	switch (this->status.getType())
	{
	case Performer::MeshImprover::NOT_INIT:
		if (!this->mesh)
		{
			this->status = Performer::MeshImprover::ended;

			return;
		}

		//markos debug add smoother only in 2D
#if USE_JMSH_SMOOTHING
		this->add(new MJMesh::LaplacianSmoother(this->dimension, this->mesh, true, this->smoothingFactor));
#else
		if (this->dimension == 2)
		{
			this->add(new MJMesh::LaplacianSmoother(this->mesh, true, this->smoothingFactor));
		}
#endif //#if USE_JMSH_SMOOTHING
		//end markos debug

		if (this->localBackTrackingEnabled)
		{
			MJMesh::MeshBackTracker *opt = new MJMesh::MeshBackTracker(this->dimension,
				this->mesh, this->boundary, this->tree,
				this->front, this->meshSorted, this->maxBackTracks);

			opt->setIdManager(this->idManager);

			this->add(opt);
		}

		this->status = Performer::MeshImprover::init;

	case Performer::MeshImprover::INIT:
		Performer::MeshImprover::execute();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case Performer::MeshImprover::ENDED:
		break;
	}
}

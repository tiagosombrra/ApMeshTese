#include "PMJMesh/FrontUpdater.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/Front.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "Data/Boundary.h"

//debug
#include "MJMesh/Mesh.h"
//endebug

PMJMesh::FrontUpdater::FrontUpdater(UInt dimension, bool shared,
	Data::Boundary *boundary, PMJMesh::Front *front, MJMesh::Mesh *mesh,
	const PMJMesh::TaskManager *taskManager, Parallel::Communicator *comm) :
	Parallel::Builder(comm)
{
	this->setDimension(dimension);

	this->setShared(shared);

	this->setBoundary(boundary);

	this->setFront(front);

	this->setMesh(mesh);

	this->setTask(NULL);
	this->setPartition(NULL);

	this->setTaskManager(taskManager);

	this->updated = false;
}

PMJMesh::FrontUpdater::~FrontUpdater()
{

}

void PMJMesh::FrontUpdater::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::FrontUpdater::setShared(bool shared)
{
	this->shared = shared;
}

void PMJMesh::FrontUpdater::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void PMJMesh::FrontUpdater::setFront(PMJMesh::Front *front)
{
	this->front = front;
}

void PMJMesh::FrontUpdater::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

void PMJMesh::FrontUpdater::setTask(PMJMesh::MJMeshTask *task)
{
	this->task = task;
}

void PMJMesh::FrontUpdater::setPartition(PMJMesh::Partition *partition)
{
	this->partition = partition;
}

void PMJMesh::FrontUpdater::setTaskManager(const PMJMesh::TaskManager *taskManager)
{
	this->taskManager = taskManager;
}

bool PMJMesh::FrontUpdater::isUpdated() const
{
	return this->updated;
}

void PMJMesh::FrontUpdater::initialize()
{
	this->updated = false;

	if (!this->shared)
	{
		return;
	}

	Data::VertexList vertices = this->front->getVertices();
	Data::FrontElementList fels = this->front->getAllElements();

	this->frontVertices.insert(vertices.begin(), vertices.end());
	this->frontFels.insert(fels.begin(), fels.end());
}

void PMJMesh::FrontUpdater::execute()
{
	if ((!this->task) ||
		((!this->shared) && (!this->partition)))
	{
		return;
	}

	if (this->shared)
	{
		this->updateFrontShared();
	}
	else
	{
		if (this->updateFrontDistributed())
		{
			this->updated = true;
		}
	}
}

void PMJMesh::FrontUpdater::finalize()
{
	this->task = NULL;
	this->partition = NULL;

	if (!this->shared)
	{
		return;
	}

	this->frontVertices.clear();
	this->frontFels.clear();
}

#endif //#if (!USE_NEW_FRONT_UPDATER)

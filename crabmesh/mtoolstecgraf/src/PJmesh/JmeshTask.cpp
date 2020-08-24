#include "PJmesh/JmeshTask.h"

#include "PJmesh/WorkerMainDrive.h"

PJmesh::JmeshTask::JmeshTask() :
	PMJMesh::MJMeshTask()
{

}

PJmesh::JmeshTask::~JmeshTask()
{

}

void PJmesh::JmeshTask::makeBuilder()
{
	if (this->builder)
	{
		delete this->builder;
	}

	this->builder = new PJmesh::WorkerMainDrive(this->comm);
}

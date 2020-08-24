#include "PMTools/MToolsTask.h"

#if (USE_MTOOLS)

#include "PMTools/Communicator.h"
#include "PMTools/WorkerMainDrive.h"

using namespace PMTools;

PMTools::MToolsTask::MToolsTask() :
	PMJMesh::MJMeshTask()
{
	this->enableBackTracking = true;
}

PMTools::MToolsTask::MToolsTask(PMJMesh::Partition *partition,
    PMJMesh::WorkerMainDrive *builder,
    PMJMesh::Communicator *comm) :
        PMJMesh::MJMeshTask::MJMeshTask(partition, builder, NULL)
{
    this->setComm(comm);

    this->enableBackTracking = true;
}

PMTools::MToolsTask::~MToolsTask()
{

}

void PMTools::MToolsTask::setEnableBackTracking(bool enableBackTracking)
{
	this->enableBackTracking = enableBackTracking;
}

void PMTools::MToolsTask::makeBuilder()
{
	if (this->builder)
	{
		delete this->builder;
	}

	this->builder = new PMTools::WorkerMainDrive(this->comm);
}

void PMTools::MToolsTask::execute()
{
	if (this->builder)
	{
		dynamic_cast<PMTools::WorkerMainDrive *>(this->builder)->setEnableBackTracking(this->enableBackTracking);
	}

	PMJMesh::MJMeshTask::execute();
}

UInt PMTools::MToolsTask::packSize(const Parallel::Communicator *comm,
	ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
	ULInt numFrontVertices, ULInt numFrontFrontElements) const
{
	UInt size = PMJMesh::MJMeshTask::packSize(comm,
		numMeshVertices, numMeshFrontElements, numMeshElements,
		numFrontVertices, numFrontFrontElements);

	//bool - enableBackTracking
	size += comm->packSize(1, Communicator::BOOL);

	return size;
}

void PMTools::MToolsTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
	//Data::VertexList &meshVertices, Data::FrontElementList &meshFels, Data::ElementList &meshEls,
	const Data::Mesh *mesh,
	//Data::VertexList &frontVertices, Data::FrontElementList &frontBoundary, Data::FrontElementList &frontElements,
	//Data::FrontElementList &frontRejected, Data::FrontElementList &frontCrossing) const;
	const Data::Front *front) const
{
	PMJMesh::MJMeshTask::pack(comm, buffer, size, position,
		mesh, front);

	comm->pack(this->enableBackTracking, buffer, size, position);
}

void PMTools::MToolsTask::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	PMJMesh::MJMeshTask::unpack(comm, p, position);

	Int size = p.first;
	char *buffer = p.second;

	comm->unpack(this->enableBackTracking, buffer, size, position);
}

#endif //#if (USE_MTOOLS)

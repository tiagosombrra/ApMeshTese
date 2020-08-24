#include "PMJMesh2/MJMeshTask.h"

#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMJMesh2/Communicator.h"
#include "PMJMesh2/WorkerMainDrive.h"

using namespace PMJMesh2;

PMJMesh2::MJMeshTask::MJMeshTask() :
    PMJMesh2::Task::Task(),
    PMJMesh::MJMeshTask::MJMeshTask()
{

}

PMJMesh2::MJMeshTask::MJMeshTask(PMJMesh::Partition *partition,
    PMJMesh2::WorkerMainDrive *builder, PMJMesh2::Communicator *comm) :
        PMJMesh2::Task::Task(),
        PMJMesh::MJMeshTask::MJMeshTask(partition, builder, NULL)
{
    this->setComm(comm);
}

PMJMesh2::MJMeshTask::~MJMeshTask()
{

}

void PMJMesh2::MJMeshTask::makeBuilder()
{
    if (this->builder)
	{
		delete this->builder;
	}

	this->builder = new PMJMesh2::WorkerMainDrive(this->comm);
}

UInt PMJMesh2::MJMeshTask::packSize(const Parallel::Communicator *comm,
    ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
    ULInt numFrontVertices, ULInt numFrontFrontElements, ULInt numAdvanced) const
{
    UInt size = PMJMesh::MJMeshTask::packSize(comm, numMeshVertices, numMeshFrontElements, numMeshElements,
        numFrontVertices, numFrontFrontElements);

    size += PMJMesh2::Task::packSize(comm, numAdvanced);

    return size;
}

UInt PMJMesh2::MJMeshTask::packSize(const Parallel::Communicator *comm) const
{
    ULInt numMeshVertices, numMeshFrontElements, numMeshElements,
		numFrontVertices, numFrontFrontElements, numAdvanced;

	numMeshVertices = numMeshFrontElements = numMeshElements =
	numFrontVertices = numFrontFrontElements = numAdvanced = 0;

	MJMesh::Mesh *mesh = this->getMesh();

	if (mesh)
	{
		numMeshVertices = mesh->verticesSize();
		numMeshFrontElements = mesh->frontElementsSize();
		numMeshElements = mesh->size();
	}

	PMJMesh::Front *front = this->getFront();

	if (front)
	{
		numFrontVertices = front->verticesSize();
		numFrontFrontElements = front->size(); //numFrontFrontElements = front->size(0) + front->size(1) + front->size(2) + front->size(3);
	}

	if (this->sendAdvanced)
    {
        numAdvanced = static_cast<ULInt>(this->advancedIds.size());
    }

	return this->packSize(comm,
		numMeshVertices, numMeshFrontElements, numMeshElements,
		numFrontVertices, numFrontFrontElements, numAdvanced);
}

void PMJMesh2::MJMeshTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
    const Data::Mesh *mesh, const Data::Front *front,
#if USE_C__11
    const Data::ULIntHash &advancedIds
#else
    const Data::ULIntSet &advancedIds
#endif //#if USE_C__11
    ) const
{
    PMJMesh::MJMeshTask::pack(comm, buffer, size, position, mesh, front);

    PMJMesh2::Task::pack(comm, buffer, size, position, advancedIds);
}

void PMJMesh2::MJMeshTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
    this->pack(comm, buffer, size, position,
		this->getMesh(), this->getFront(),
        this->sendAdvanced ? this->advancedIds :
#if USE_C__11
            Data::ULIntHash()
#else
            Data::ULIntSet()
#endif //#if USE_C__11
        );
}

Parallel::Package PMJMesh2::MJMeshTask::pack(const Parallel::Communicator *comm) const
{
    MJMesh::Mesh *mesh = this->getMesh();
	PMJMesh::Front *front = this->getFront();

	Int size = this->packSize(comm,
		mesh ? mesh->verticesSize() : 0, mesh ? mesh->frontElementsSize() : 0,
		mesh ? mesh->size() : 0, front ? front->verticesSize() : 0,
		front ? front->size(0) + front->size(1) + front->size(2) + front->size(3) : 0,
        this->sendAdvanced ? static_cast<ULInt>(this->advancedIds.size()) : 0);

	char *buffer = NULL;
	Int position = 0;

	buffer = new char[size];

	this->pack(comm, buffer, size, position,
		mesh, front, this->sendAdvanced ? this->advancedIds :
#if USE_C__11
        Data::ULIntHash()
#else
        Data::ULIntSet()
#endif //#if USE_C__11
		);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh2::MJMeshTask::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
    PMJMesh::MJMeshTask::unpack(comm, p, position);
    PMJMesh2::Task::unpack(comm, p, position);
}

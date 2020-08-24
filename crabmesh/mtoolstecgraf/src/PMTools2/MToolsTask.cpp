#include "PMTools2/MToolsTask.h"

#if USE_MTOOLS

#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMTools2/Communicator.h"
#include "PMTools2/WorkerMainDrive.h"

using namespace PMTools2;

PMTools2::MToolsTask::MToolsTask() :
    PMJMesh2::Task::Task(),
    PMTools::MToolsTask::MToolsTask()
{

}

PMTools2::MToolsTask::MToolsTask(PMJMesh::Partition *partition,
    PMTools2::WorkerMainDrive *builder, PMTools2::Communicator *comm) :
        PMJMesh2::Task::Task(),
        PMTools::MToolsTask::MToolsTask(partition, builder, NULL)
{
    this->setComm(comm);
}

PMTools2::MToolsTask::~MToolsTask()
{

}

void PMTools2::MToolsTask::makeBuilder()
{
    if (this->builder)
	{
		delete this->builder;
	}

	this->builder = new PMTools2::WorkerMainDrive(this->comm);
}

UInt PMTools2::MToolsTask::packSize(const Parallel::Communicator *comm,
    ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
    ULInt numFrontVertices, ULInt numFrontFrontElements, ULInt numAdvanced) const
{
    UInt size = PMTools::MToolsTask::packSize(comm, numMeshVertices, numMeshFrontElements, numMeshElements,
        numFrontVertices, numFrontFrontElements);

    size += PMJMesh2::Task::packSize(comm, numAdvanced);

    return size;
}

UInt PMTools2::MToolsTask::packSize(const Parallel::Communicator *comm) const
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

void PMTools2::MToolsTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
    const Data::Mesh *mesh, const Data::Front *front,
#if USE_C__11
    const Data::ULIntHash &advancedIds
#else
    const Data::ULIntSet &advancedIds
#endif //#if USE_C__11
    ) const
{
    PMTools::MToolsTask::pack(comm, buffer, size, position, mesh, front);

    PMJMesh2::Task::pack(comm, buffer, size, position, advancedIds);
}

void PMTools2::MToolsTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
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

Parallel::Package PMTools2::MToolsTask::pack(const Parallel::Communicator *comm) const
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

void PMTools2::MToolsTask::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
    PMTools::MToolsTask::unpack(comm, p, position);
    PMJMesh2::Task::unpack(comm, p, position);
}

#endif //#if USE_MTOOLS

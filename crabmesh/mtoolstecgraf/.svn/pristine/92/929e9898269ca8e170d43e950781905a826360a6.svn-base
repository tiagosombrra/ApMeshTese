#include "PMJMesh2/Task.h"

#if USE_GUI
#include "Data/Point3D.h"
#endif //#if USE_GUI
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMJMesh2/Communicator.h"

using namespace PMJMesh2;

PMJMesh2::Task::Task()
{
    this->sendAdvanced = false;
    this->cell = NULL;
}

PMJMesh2::Task::~Task()
{
    while (!this->meshes.empty())
    {
        delete this->meshes.front();

        this->meshes.pop_front();
    }
}

void PMJMesh2::Task::setCell(PartitionBSPTreeCell *cell)
{
    this->cell = cell;
}

PMJMesh2::PartitionBSPTreeCell *PMJMesh2::Task::getCell() const
{
    return this->cell;
}

void PMJMesh2::Task::setMeshes(const Data::MeshList &meshes)
{
    this->meshes = meshes;
}

const Data::MeshList &PMJMesh2::Task::getMeshes() const
{
    return this->meshes;
}

void PMJMesh2::Task::clearMeshes()
{
    this->meshes.clear();
}

void PMJMesh2::Task::add(Data::Mesh *mesh)
{
    this->meshes.push_back(mesh);
}

void PMJMesh2::Task::remove(Data::Mesh *mesh)
{
    this->meshes.remove(mesh);
}

bool PMJMesh2::Task::have(const Data::Mesh *mesh)
{
    return (std::find(this->meshes.begin(), this->meshes.end(), mesh) != this->meshes.end());
}

#if USE_GUI
void PMJMesh2::Task::setMids(const Data::Point3DObjList &mids)
{
    this->mids = mids;
}

const Data::Point3DObjList &PMJMesh2::Task::getMids() const
{
    return this->mids;
}

void PMJMesh2::Task::clearMids()
{
    this->mids.clear();
}

void PMJMesh2::Task::add(const Data::Point3D &mid)
{
    this->mids.push_back(mid);
}
#endif //#if USE_GUI

void PMJMesh2::Task::setSendAdvanced(bool sendAdvanced)
{
	this->sendAdvanced = sendAdvanced;
}

void PMJMesh2::Task::setAdvancedFrom(PMJMesh2::Task *task)
{
    this->advancedIds.swap(task->advancedIds);
}

void PMJMesh2::Task::mergeAdvancedFrom(PMJMesh2::Task *task)
{
    this->advancedIds.insert(task->advancedIds.begin(), task->advancedIds.end());
}

void PMJMesh2::Task::addAdvanced(const Data::FrontElement *fel)
{
    this->advancedIds.insert(fel->getId());
}

void PMJMesh2::Task::removeAdvanced(const Data::FrontElement *fel)
{
    this->advancedIds.erase(fel->getId());
}

bool PMJMesh2::Task::isAdvanced(const Data::FrontElement *fel)
{
	return (this->advancedIds.find(fel->getId()) != this->advancedIds.end());
}

void PMJMesh2::Task::clearAdvanced()
{
	this->advancedIds.clear();
}

UInt PMJMesh2::Task::packSize(const Parallel::Communicator *comm, ULInt numAdvanced) const
{
    UInt size = 0;

    size += comm->packSize(1 + static_cast<UInt>(numAdvanced), Communicator::ULINT);

    return size;
}

UInt PMJMesh2::Task::packSize(const Parallel::Communicator *comm) const
{
    ULInt numAdvanced = 0;

	if (this->sendAdvanced)
    {
        numAdvanced = static_cast<ULInt>(this->advancedIds.size());
    }

	return this->packSize(comm, numAdvanced);
}

void PMJMesh2::Task::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
#if USE_C__11
    const Data::ULIntHash &advancedIds
#else
    const Data::ULIntSet &advancedIds
#endif //#if USE_C__11
    ) const
{
    comm->pack(static_cast<ULInt>(advancedIds.size()), buffer, size, position);

#if USE_C__11
    for (Data::ULIntHash::const_iterator iter = advancedIds.begin();
#else
    for (Data::ULIntSet::const_iterator iter = advancedIds.begin();
#endif //#if USE_C__11
         iter != advancedIds.end(); iter++)
    {
        comm->pack((*iter), buffer, size, position);
    }
}

void PMJMesh2::Task::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
    this->pack(comm, buffer, size, position,
		this->sendAdvanced ? this->advancedIds :
#if USE_C__11
            Data::ULIntHash()
#else
            Data::ULIntSet()
#endif //#if USE_C__11
        );
}

Parallel::Package PMJMesh2::Task::pack(const Parallel::Communicator *comm) const
{
    Int size = this->packSize(comm,
        this->sendAdvanced ? static_cast<ULInt>(this->advancedIds.size()) : 0);

	char *buffer = NULL;
	Int position = 0;

	buffer = new char[size];

	this->pack(comm, buffer, size, position,
		this->sendAdvanced ? this->advancedIds :
#if USE_C__11
        Data::ULIntHash()
#else
        Data::ULIntSet()
#endif //#if USE_C__11
		);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh2::Task::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
    Int size = p.first;
	char *buffer = p.second;

    ULInt numAdvanced = 0;

    comm->unpack(numAdvanced, buffer, size, position);

#if USE_C__11
    this->advancedIds.reserve(numAdvanced);
#endif //#if USE_C__11

    for (ULInt i = 0; i < numAdvanced; i++)
    {
        ULInt id = 0;

        comm->unpack(id, buffer, size, position);

        this->advancedIds.insert(id);
    }
}

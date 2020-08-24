#include "PMJMesh2/Communicator.h"

#include "Parallel/MPIMessage.h"
#include "PMJMesh/BoundaryTransferable.h"
#include "PMJMesh/TreeTransferable.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh2/InputData.h"
#include "PMJMesh2/MJMeshTask.h"

using namespace PMJMesh2;

PMJMesh2::Communicator::Communicator(bool sharedParallelismEnabled) :
	Parallel::TMCommunicator::TMCommunicator(sharedParallelismEnabled)
{

}

PMJMesh2::Communicator::~Communicator()
{

}

bool PMJMesh2::Communicator::isMaster() const
{
	return (this->rank() == this->root());
}

Parallel::Transferable *PMJMesh2::Communicator::unpack(Parallel::Package &p) const
{
	Parallel::Transferable *o = NULL;

	Int size = p.first;
	char *buffer = p.second;

	Int position = 0;
	UInt type = 0;

	this->unpack(type, buffer, size, position);

	switch (type)
	{
	case PMJMesh::TRANSFERABLE_MESSAGE:
#if USE_MPI
		o = new Parallel::MPIMessage();
#endif //#if USE_MPI
		break;

	case PMJMesh::TRANSFERABLE_INPUT_DATA:
		o = new PMJMesh2::InputData();
		break;

	case PMJMesh::TRANSFERABLE_BOUNDARY:
		o = new PMJMesh::BoundaryTransferable();
		break;

	case PMJMesh::TRANSFERABLE_TREE:
		o = new PMJMesh::TreeTransferable();
		break;

	case PMJMesh::TRANSFERABLE_MESH:
		o = new PMJMesh::MeshTransferable();
		break;

	case PMJMesh::TRANSFERABLE_PARTITION:
		o = new PMJMesh::Partition();
		break;

	case PMJMesh::TRANSFERABLE_MJMESH_TASK:
		o = new PMJMesh2::MJMeshTask();
		break;
	}

	if (o)
	{
		o->unpack(this, p, position);
	}

	return o;
}

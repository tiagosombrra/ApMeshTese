#include "PJmesh/Communicator.h"

#include "PJmesh/InputData.h"
#include "PJmesh/JmeshTask.h"

PJmesh::Communicator::Communicator() :
	PMJMesh::Communicator()
{

}

PJmesh::Communicator::~Communicator()
{

}

Parallel::Transferable *PJmesh::Communicator::unpack(Parallel::Package &p) const
{
	Parallel::Transferable *o = NULL;

	Int size = p.first;
	char *buffer = p.second;

	Int position = 0;
	UInt type = 0;

	this->unpack(type, buffer, size, position);

	switch (type)
	{
	case PMJMesh::TRANSFERABLE_INPUT_DATA:
		o = new PJmesh::InputData();
		break;

	case PMJMesh::TRANSFERABLE_MJMESH_TASK:
		o = new PJmesh::JmeshTask();
		break;
	}

	if (o)
	{
		o->unpack(this, p, position);
	}
	else
	{
		return PMJMesh::Communicator::unpack(p);
	}

	return o;
}

#include "PMTools/Communicator.h"

#if (USE_MTOOLS)

//#include "PMTools/InputData.h"
#include "PMTools/MToolsTask.h"

using namespace PMTools;

PMTools::Communicator::Communicator() :
	PMJMesh::Communicator()
{

}

PMTools::Communicator::~Communicator()
{

}

Parallel::Transferable *PMTools::Communicator::unpack(Parallel::Package &p) const
{
	Parallel::Transferable *o = NULL;

	Int size = p.first;
	char *buffer = p.second;

	Int position = 0;
	UInt type = 0;

	this->unpack(type, buffer, size, position);

	switch (type)
	{
	/*case PMJMesh::TRANSFERABLE_INPUT_DATA:
		o = new PMTools::InputData();
		break;*/

	case PMJMesh::TRANSFERABLE_MJMESH_TASK:
		o = new PMTools::MToolsTask();
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
#endif //#if (USE_MTOOLS)

#ifndef _PJMESH_COMMUNICATOR_H_
#define _PJMESH_COMMUNICATOR_H_

#include "PJmesh/Definitions.h"

#include "PMJMesh/Communicator.h"

namespace PJmesh
{
	class Communicator : public PMJMesh::Communicator
	{
	public:

		Communicator();
		virtual ~Communicator();

		using PMJMesh::Communicator::unpack;
		virtual Parallel::Transferable *unpack(Parallel::Package &p) const;
	};
}

#endif //#ifndef _PJMESH_COMMUNICATOR_H_

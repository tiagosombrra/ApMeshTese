#ifndef _PMTOOLS_COMMUNICATOR_H_
#define _PMTOOLS_COMMUNICATOR_H_

#include "PMTools/Definitions.h"

#if (USE_MTOOLS)

#include "PMJMesh/Communicator.h"

namespace PMTools
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

#endif //#if (USE_MTOOLS)

#endif //#ifndef _PMTOOLS_COMMUNICATOR_H_

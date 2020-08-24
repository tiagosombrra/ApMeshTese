#ifndef _PMJMESH_COMMUNICATOR_H_
#define _PMJMESH_COMMUNICATOR_H_

#include "PMJMesh/Definitions.h"
#include "Parallel/TMCommunicator.h"

namespace PMJMesh
{
	class Communicator : public Parallel::TMCommunicator
	{
	public:

		Communicator(bool sharedParallelismEnabled = true);
		virtual ~Communicator();

		virtual Int root() const;
		virtual Int rank() const;

		virtual Data::IntVector ranks(Int process = -1) const;

		virtual void initThreadCount();

		virtual Data::RealVector runtimes() const;

		virtual bool isMaster() const;

		virtual Int numWorkers() const;

		virtual void calcRuntimeInfo(Real runtime);

		using Parallel::TMCommunicator::unpack;
		virtual Parallel::Transferable *unpack(Parallel::Package &p) const;

	protected:

		Data::RealVector runtime;
	};
}

#endif //#ifndef _PMJMESH_COMMUNICATOR_H_

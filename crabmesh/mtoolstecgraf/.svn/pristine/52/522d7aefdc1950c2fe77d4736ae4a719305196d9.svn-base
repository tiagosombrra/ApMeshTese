#ifndef _PMJMESH2_COMMUNICATOR_H_
#define _PMJMESH2_COMMUNICATOR_H_

#include "PMJMesh2/Definitions.h"
#include "Parallel/TMCommunicator.h"

namespace PMJMesh2
{
    class Communicator : public Parallel::TMCommunicator
    {
    public:

        Communicator(bool sharedParallelismEnabled = true);
        virtual ~Communicator();

        virtual bool isMaster() const;

        using Parallel::TMCommunicator::unpack;
		virtual Parallel::Transferable *unpack(Parallel::Package &p) const;

    protected:

    };
}

#endif //#ifndef _PMJMESH2_COMMUNICATOR_H_

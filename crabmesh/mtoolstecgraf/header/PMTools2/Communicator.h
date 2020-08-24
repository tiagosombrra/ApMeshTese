#ifndef _PMTOOLS2_COMMUNICATOR_H_
#define _PMTOOLS2_COMMUNICATOR_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "Parallel/TMCommunicator.h"

namespace PMTools2
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

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_COMMUNICATOR_H_

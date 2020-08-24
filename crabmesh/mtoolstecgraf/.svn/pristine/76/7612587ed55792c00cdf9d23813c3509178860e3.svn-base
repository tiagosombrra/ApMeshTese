#ifndef _PARALLEL_SYNC_DYNAMIC_LOAD_BALANCER_H_
#define _PARALLEL_SYNC_DYNAMIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/Builder.h"
#include "Parallel/DynamicLoadBalancer.h"

namespace Parallel
{
	class SyncDynamicLoadBalancer : public Parallel::DynamicLoadBalancer
	{
		//This class is a dynamic load balancer which, when it needs to balance
		// the load, stops the concurrent execution of the program (all the
		// processes synchronize), applies a static load balancer, and then
		// resumes the execution.

	public:

		SyncDynamicLoadBalancer(StaticLoadBalancer *slb, Communicator *comm);
		SyncDynamicLoadBalancer(StaticLoadBalancer *slb,
			const TaskVector &tasks, Communicator *comm);
		virtual ~SyncDynamicLoadBalancer();

		virtual void setSlb(StaticLoadBalancer *slb);

	protected:

		StaticLoadBalancer *slb;
	};
}

#endif //#ifndef _PARALLEL_SYNC_DYNAMIC_LOAD_BALANCER_H_

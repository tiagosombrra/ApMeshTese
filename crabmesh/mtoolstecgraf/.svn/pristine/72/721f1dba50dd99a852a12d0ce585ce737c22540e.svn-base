#ifndef _PARALLEL_ASYNC_DYNAMIC_LOAD_BALANCER_H_
#define _PARALLEL_ASYNC_DYNAMIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/Builder.h"
#include "Parallel/DynamicLoadBalancer.h"

namespace Parallel
{
	class AsyncDynamicLoadBalancer : public Parallel::DynamicLoadBalancer
	{
		//This class is a dynamic load balance which does not need to stop and
		// resume the execution of the program, it runs concurrently with the
		// computation (possibly in another thread).

	public:

		AsyncDynamicLoadBalancer(Communicator *comm);
		AsyncDynamicLoadBalancer(const TaskVector &tasks, Communicator *comm);
		virtual ~AsyncDynamicLoadBalancer();
	};
}

#endif //#ifndef _PARALLEL_ASYNC_DYNAMIC_LOAD_BALANCER_H_

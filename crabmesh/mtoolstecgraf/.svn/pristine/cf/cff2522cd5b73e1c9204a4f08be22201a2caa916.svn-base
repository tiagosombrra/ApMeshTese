#ifndef _PARALLEL_DYNAMIC_LOAD_BALANCER_H_
#define _PARALLEL_DYNAMIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/LoadBalancer.h"

namespace Parallel
{
	class DynamicLoadBalancer : public Parallel::LoadBalancer
	{
	public:

		DynamicLoadBalancer(Communicator *comm);
		DynamicLoadBalancer(const TaskVector &tasks, Communicator *comm);
		virtual ~DynamicLoadBalancer();

		virtual bool isStatic() const;
	};
}

#endif //#ifndef _PARALLEL_DYNAMIC_LOAD_BALANCER_H_

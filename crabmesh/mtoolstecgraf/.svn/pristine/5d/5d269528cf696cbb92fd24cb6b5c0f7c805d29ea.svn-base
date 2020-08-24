#ifndef _PARALLEL_DEMAND_PARALLEL_DYNAMIC_LOAD_BALANCER_H_
#define _PARALLEL_DEMAND_PARALLEL_DYNAMIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/AsyncDynamicLoadBalancer.h"

namespace Parallel
{
	class DemandParallelDynamicLoadBalancer : public AsyncDynamicLoadBalancer
	{
	public:

		DemandParallelDynamicLoadBalancer(Communicator *comm);
		DemandParallelDynamicLoadBalancer(const TaskVector &tasks,
			Communicator *comm);
		virtual ~DemandParallelDynamicLoadBalancer();

		virtual bool needToBalance() const;

		virtual UInt numTasks() const;

		virtual Task *next();

		virtual void sendToSlaves();

		virtual void receiveFromMaster();

		virtual void execute();

	private:

		bool ended;
	};
}

#endif //#ifndef _PARALLEL_DEMAND_PARALLEL_DYNAMIC_LOAD_BALANCER_H_

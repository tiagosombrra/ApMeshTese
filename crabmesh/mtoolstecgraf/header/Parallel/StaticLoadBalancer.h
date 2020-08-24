#ifndef _PARALLEL_STATIC_LOAD_BALANCER_H_
#define _PARALLEL_STATIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/LoadBalancer.h"

namespace Parallel
{
	class StaticLoadBalancer : public Parallel::LoadBalancer
	{
	public:

		StaticLoadBalancer(Communicator *comm);
		StaticLoadBalancer(const TaskVector &tasks, Communicator *comm);
		virtual ~StaticLoadBalancer();

		virtual void setTaskVectorMap(const TaskVectorMap &taskVectorMap);

		virtual bool isStatic() const;

		virtual bool needToBalance() const;

		//This execute does nothing, it is here just to allow this class to
		//  be instantiated. The mapping of several tasks to each processor
		//  should be given using method taskVectorMap().
		virtual void execute();
	};
}

#endif //#ifndef _PARALLEL_STATIC_LOAD_BALANCER_H_

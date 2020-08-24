#ifndef _PARALLEL_SPRING_SERIAL_STATIC_LOAD_BALANCER_H_
#define _PARALLEL_SPRING_SERIAL_STATIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/StaticLoadBalancer.h"
#include "Performer/Builder.h"

namespace Parallel
{
	class SpringSerialStaticLoadBalancer : public Parallel::StaticLoadBalancer
	{
	public:

		SpringSerialStaticLoadBalancer(TMCommunicator *comm);
		SpringSerialStaticLoadBalancer(const TaskVector &tasks, TMCommunicator *comm);
		virtual ~SpringSerialStaticLoadBalancer();

		virtual void execute();
	};
}

#endif //#ifndef _PARALLEL_SPRING_SERIAL_STATIC_LOAD_BALANCER_H_

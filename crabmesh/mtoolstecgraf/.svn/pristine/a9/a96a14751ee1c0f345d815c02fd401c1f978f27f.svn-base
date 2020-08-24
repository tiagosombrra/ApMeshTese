#ifndef _PARALLEL_GREEDY_SERIAL_STATIC_LOAD_BALANCER_H_
#define _PARALLEL_GREEDY_SERIAL_STATIC_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/StaticLoadBalancer.h"
#include "Performer/Builder.h"

namespace Parallel
{
	class GreedySerialStaticLoadBalancer : public Parallel::StaticLoadBalancer
	{
	public:

		GreedySerialStaticLoadBalancer(Communicator *comm);
		GreedySerialStaticLoadBalancer(const TaskVector &tasks, Communicator *comm);
		virtual ~GreedySerialStaticLoadBalancer();

		virtual void execute();

	protected:

		virtual void updateLoad(Int processor, Real load);

	protected:

		Data::RealVector totalLoad;
	};
}

#endif //#ifndef _PARALLEL_GREEDY_SERIAL_STATIC_LOAD_BALANCER_H_

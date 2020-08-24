#ifndef _PARALLEL_WEIGHTED_GREEDY_SERIAL_STATIC_LOAD_BALANCER_H_
#define _PARALLEL_WEIGHTED_GREEDY_SERIAL_STATIC_LOAD_BALANCER_H_

#include "PMJMesh/Definitions.h"
#include "Parallel/GreedySerialStaticLoadBalancer.h"

namespace Parallel
{
	class WeightedGreedySerialStaticLoadBalancer : public GreedySerialStaticLoadBalancer
	{
	public:

		WeightedGreedySerialStaticLoadBalancer(const Data::RealVector &weights,
			Communicator *comm);
		WeightedGreedySerialStaticLoadBalancer(const Data::RealVector &weights,
			const TaskVector &tasks, Communicator *comm);
		virtual ~WeightedGreedySerialStaticLoadBalancer();

		virtual void setWeights(const Data::RealVector &weights);

	protected:

		virtual void updateLoad(Int processor, Real load);

	protected:

		Data::RealVector weights;
	};
}

#endif //#ifndef _PARALLEL_WEIGHTED_GREEDY_SERIAL_STATIC_LOAD_BALANCER_H_

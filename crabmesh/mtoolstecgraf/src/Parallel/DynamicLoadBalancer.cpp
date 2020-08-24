#include "Parallel/DynamicLoadBalancer.h"

Parallel::DynamicLoadBalancer::DynamicLoadBalancer(Communicator *comm) :
	LoadBalancer(comm)
{

}

Parallel::DynamicLoadBalancer::DynamicLoadBalancer(const TaskVector &tasks, Communicator *comm) :
	LoadBalancer(tasks, comm)
{

}

Parallel::DynamicLoadBalancer::~DynamicLoadBalancer()
{

}

bool Parallel::DynamicLoadBalancer::isStatic() const
{
	return false;
}

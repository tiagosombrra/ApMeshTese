#include "Parallel/AsyncDynamicLoadBalancer.h"

Parallel::AsyncDynamicLoadBalancer::AsyncDynamicLoadBalancer(Communicator  *comm) :
	DynamicLoadBalancer(comm)
{

}

Parallel::AsyncDynamicLoadBalancer::AsyncDynamicLoadBalancer(const TaskVector &tasks, Communicator  *comm) :
	DynamicLoadBalancer(tasks, comm)
{

}

Parallel::AsyncDynamicLoadBalancer::~AsyncDynamicLoadBalancer()
{

}

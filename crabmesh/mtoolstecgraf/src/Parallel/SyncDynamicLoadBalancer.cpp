#include "Parallel/SyncDynamicLoadBalancer.h"

Parallel::SyncDynamicLoadBalancer::SyncDynamicLoadBalancer(StaticLoadBalancer *slb,
	Communicator  *comm) :
	DynamicLoadBalancer(comm)
{
	this->setSlb(slb);
}

Parallel::SyncDynamicLoadBalancer::SyncDynamicLoadBalancer(StaticLoadBalancer *slb,
	const TaskVector &tasks, Communicator  *comm) :
	DynamicLoadBalancer(tasks, comm)
{
	this->setSlb(slb);
}

Parallel::SyncDynamicLoadBalancer::~SyncDynamicLoadBalancer()
{

}

void Parallel::SyncDynamicLoadBalancer::setSlb(StaticLoadBalancer *slb)
{
	this->slb = slb;
}

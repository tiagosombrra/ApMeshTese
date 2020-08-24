#include "Parallel/StaticLoadBalancer.h"

#include "Parallel/Communicator.h"

Parallel::StaticLoadBalancer::StaticLoadBalancer(Communicator *comm) :
	LoadBalancer(comm)
{

}

Parallel::StaticLoadBalancer::StaticLoadBalancer(const TaskVector &tasks, Communicator *comm) :
	LoadBalancer(tasks, comm)
{

}

Parallel::StaticLoadBalancer::~StaticLoadBalancer()
{

}

void Parallel::StaticLoadBalancer::setTaskVectorMap(const TaskVectorMap &taskVectorMap)
{
	this->taskVectorMap = taskVectorMap;
}

bool Parallel::StaticLoadBalancer::isStatic() const
{
	return true;
}

bool Parallel::StaticLoadBalancer::needToBalance() const
{
	return !taskVectorMap.empty();
}

void Parallel::StaticLoadBalancer::execute()
{

}

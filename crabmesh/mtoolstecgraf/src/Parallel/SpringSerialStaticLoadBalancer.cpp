#include "Parallel/SpringSerialStaticLoadBalancer.h"

#include "Parallel/Task.h"
#include "Parallel/TMCommunicator.h"

Parallel::SpringSerialStaticLoadBalancer::SpringSerialStaticLoadBalancer(TMCommunicator *comm) :
	StaticLoadBalancer(comm)
{

}

Parallel::SpringSerialStaticLoadBalancer::SpringSerialStaticLoadBalancer(
	const TaskVector &tasks, TMCommunicator *comm) :
	StaticLoadBalancer(tasks, comm)
{

}

Parallel::SpringSerialStaticLoadBalancer::~SpringSerialStaticLoadBalancer()
{

}

void Parallel::SpringSerialStaticLoadBalancer::execute()
{
	Int numWorkers = this->comm->numWorkers();

	if ((numWorkers <= 0) || (this->tasks.empty()))
	{
		return;
	}

	this->taskVectorMap.clear();

	Int numProcesses = this->comm->numProcesses();

	if ((!this->comm->isParallel()) ||
		((numProcesses == 2) && (numWorkers == static_cast<TMCommunicator *>(this->comm)->numThreads(1))))
	{
		this->sort();

		this->status = Parallel::LoadBalancer::success;

		return;
	}

	Parallel::TaskVector tasks = this->tasks;

	Int worker = 0;

	this->sort(true);

	while (!tasks.empty())
	{
		Task *t = tasks.back();
		tasks.pop_back();

		Real min = t->load();

		//if (!(also divide empty partitions))
		if (true)
		{
			if (min < Data::getTolerance())
			{
				continue;
			}
		}

		if (worker < numWorkers)
		{
			this->taskVectorMap[worker].push_back(t);
		}
		else
		{
			this->taskVectorMap[2*numWorkers - worker - 1].push_back(t);
		}

		worker++;

		if (worker == 2*numWorkers)
		{
			worker = 0;
		}
	}

	this->status = Parallel::LoadBalancer::success;
}

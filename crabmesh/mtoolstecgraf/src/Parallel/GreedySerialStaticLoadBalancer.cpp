#include "Parallel/GreedySerialStaticLoadBalancer.h"

#include "Parallel/Task.h"
#include "Parallel/TMCommunicator.h"

Parallel::GreedySerialStaticLoadBalancer::GreedySerialStaticLoadBalancer(Communicator *comm) :
	StaticLoadBalancer(comm)
{

}

Parallel::GreedySerialStaticLoadBalancer::GreedySerialStaticLoadBalancer(
	const TaskVector &tasks, Communicator *comm) :
	StaticLoadBalancer(tasks, comm)
{

}

Parallel::GreedySerialStaticLoadBalancer::~GreedySerialStaticLoadBalancer()
{

}

void Parallel::GreedySerialStaticLoadBalancer::updateLoad(Int processor, Real load)
{
	this->totalLoad[processor] += load;
}

void Parallel::GreedySerialStaticLoadBalancer::execute()
{
	Int numWorkers = this->comm->numWorkers();

	if ((numWorkers <= 0) || (this->tasks.empty()))
	{
		return;
	}

	this->taskVectorMap.clear();
	this->totalLoad.clear();

	Int numProcesses = this->comm->numProcesses();

	if ((!this->comm->isParallel()) ||
		((numProcesses == 2) && (numWorkers == static_cast<TMCommunicator *>(this->comm)->numThreads(1))))
	{
		this->sort();

		this->status = Parallel::LoadBalancer::success;

		return;
	}

	this->totalLoad.resize(numWorkers);

	for (Int i = 0; i < numWorkers; i++)
	{
		this->totalLoad[i] = 0.0;
	}

	Parallel::TaskVector emptyTasks;
	Parallel::TaskVector tasks = this->tasks;

	this->sort(false);

	while (!tasks.empty())
	{
		Task *t = tasks.back();

		Real max = t->load();

		if (max <= Data::getTolerance())
		{
			emptyTasks = tasks;

			tasks.clear();

			break;
		}

		tasks.pop_back();

		Int worker = 0;

		for (Int i = 1; i < numWorkers; i++)
		{
			if (this->totalLoad[i] < this->totalLoad[worker])
			{
				worker = i;
			}
		}

		this->taskVectorMap[worker].push_back(t);

		this->updateLoad(worker, max);
	}

	/*if (false)
	{
		while (!emptyTasks.empty())
		{
			Task *t = emptyTasks.back();
			emptyTasks.pop_back();

			UInt processor = (*this->taskVectorMap.begin()).first;
			UInt min = static_cast<UInt>((*this->taskVectorMap.begin()).second.size());

			for (TaskVectorMap::iterator iter = ++this->taskVectorMap.begin();
				 iter != this->taskVectorMap.end(); iter++)
			{
				UInt size = static_cast<UInt>((*iter).second.size());

				if (size < min)
				{
					min = size;
					processor = (*iter).first;
				}
			}

			this->taskVectorMap[processor].push_back(t);
		}
	}*/

	this->status = Parallel::LoadBalancer::success;
}

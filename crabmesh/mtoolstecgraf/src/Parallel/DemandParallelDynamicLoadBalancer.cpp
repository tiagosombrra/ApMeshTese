#include "Parallel/DemandParallelDynamicLoadBalancer.h"

#include "Parallel/Partition.h"
#include "Parallel/Communicator.h"
#include "Parallel/Message.h"
#include "Parallel/Task.h"

using namespace Parallel;

Parallel::DemandParallelDynamicLoadBalancer::DemandParallelDynamicLoadBalancer(Communicator *comm) :
	AsyncDynamicLoadBalancer(comm)
{
	this->ended = false;
}

Parallel::DemandParallelDynamicLoadBalancer::DemandParallelDynamicLoadBalancer(
	const TaskVector &tasks, Communicator *comm) :
	AsyncDynamicLoadBalancer(tasks, comm)
{
	this->ended = false;
}

Parallel::DemandParallelDynamicLoadBalancer::~DemandParallelDynamicLoadBalancer()
{

}

bool Parallel::DemandParallelDynamicLoadBalancer::needToBalance() const
{
	return !this->taskVectorMap.empty();
}

UInt Parallel::DemandParallelDynamicLoadBalancer::numTasks() const
{
	return (!this->comm->isParallel()) ? AsyncDynamicLoadBalancer::numTasks() : UINT_MAX;
}

Task *Parallel::DemandParallelDynamicLoadBalancer::next()
{
	if (!this->comm->isParallel())
	{
		return AsyncDynamicLoadBalancer::next();
	}

	if (this->ended)
	{
		return NULL;
	}

	Int rank = this->comm->rank();
	Int root = this->comm->root();

	this->comm->send(rank, root, 0);

	Package p = std::make_pair(0, static_cast<char *>(NULL));

	this->comm->receive(p.first, root, 0);

	if (p.first != 0)
	{
		p.second = new char[p.first];

		this->comm->receive(p.second, p.first, this->comm->PACKED, root, 0);

		//return static_cast<Task *>(this->comm->unpack(p));
		Task *t = static_cast<Task *>(this->comm->unpack(p, true));

		t->setComm(this->comm);

		return t;
	}

	this->comm->barrier();

	this->ended = true;

	return NULL;
}

void Parallel::DemandParallelDynamicLoadBalancer::sendToSlaves()
{
	if (!this->comm->isParallel())
	{
		AsyncDynamicLoadBalancer::sendToSlaves();

		return;
	}

	while (!this->taskVectorMap.empty())
	{
		TaskVector tasks = (*this->taskVectorMap.begin()).second;

		this->taskVectorMap.erase(this->taskVectorMap.begin());

		while (!tasks.empty())
		{
			Task *t = tasks.back();

			tasks.pop_back();

			Int process = 0;

			this->comm->receive(process, -1, 0);

			Package p = t->pack(this->comm);

			this->comm->removeRequest(this->comm->isend(p.first, process, 0));

			this->comm->removeRequest(this->comm->isend(p.second, p.first, this->comm->PACKED, process, 0), true);
		}
	}

	Int numProcesses = this->comm->numProcesses();

	for (Int i = 1; i < numProcesses; i++)
	{
		Int process = 0;

		this->comm->receive(process, -1, 0);

		this->comm->removeRequest(this->comm->isend(0, process, 0));
	}

	this->comm->barrier();

	this->comm->deleteRequest();
}

void Parallel::DemandParallelDynamicLoadBalancer::receiveFromMaster()
{
	if (!this->comm->isParallel())
	{
		AsyncDynamicLoadBalancer::receiveFromMaster();
	}

	this->ended = false;
}

void Parallel::DemandParallelDynamicLoadBalancer::execute()
{
	Int numWorkers = this->comm->numWorkers();

	if ((numWorkers <= 0) || (this->tasks.empty()))
	{
		return;
	}

	this->taskVectorMap.clear();

	this->sort();

	this->status = Parallel::LoadBalancer::success;
}

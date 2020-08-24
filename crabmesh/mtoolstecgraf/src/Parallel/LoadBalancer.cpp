#include "Parallel/LoadBalancer.h"

#include "Parallel/Communicator.h"
#include "Parallel/Partition.h"
#include "Parallel/Message.h"
#include "Parallel/Task.h"

using namespace Parallel;

const Performer::Status Parallel::LoadBalancer::notInit =
	Performer::Status(Parallel::LoadBalancer::NOT_INIT, "not init");
const Performer::Status Parallel::LoadBalancer::success =
	Performer::Status(Parallel::LoadBalancer::SUCCESS, "success");
const Performer::Status Parallel::LoadBalancer::error =
	Performer::Status(Parallel::LoadBalancer::ERROR, "error");

Parallel::LoadBalancer::LoadBalancer(Communicator *comm) :
	Parallel::Builder(comm)
{
	this->status = Parallel::LoadBalancer::notInit;
}

Parallel::LoadBalancer::LoadBalancer(const TaskVector &tasks, Communicator *comm) :
	Parallel::Builder(comm)
{
	this->status = Parallel::LoadBalancer::notInit;

	this->setTasks(tasks);
}

Parallel::LoadBalancer::~LoadBalancer()
{

}

void Parallel::LoadBalancer::deleteRemotePartitions()
{
	if (!this->comm)
	{
		return;
	}

	Int rank = this->comm->rank();

	while (!this->taskVectorMap.empty())
	{
		TaskVectorMap::iterator it = this->taskVectorMap.begin();

		if ((*it).first != rank)
		{
			TaskVector tasks = (*it).second;

			while (!tasks.empty())
			{
				Task *t = tasks.back();

				tasks.pop_back();

				TaskVector::iterator it = std::find(this->tasks.begin(), this->tasks.end(), t);

				if (it != this->tasks.end())
				{
					this->tasks.erase(it);
				}

				delete t;
			}
		}

		this->taskVectorMap.erase(it);
	}
}

const TaskVectorMap &Parallel::LoadBalancer::getTaskVectorMap() const
{
	return this->taskVectorMap;
}

void Parallel::LoadBalancer::setTasks(const TaskVector &tasks)
{
	this->tasks = tasks;
}

const TaskVector &Parallel::LoadBalancer::getTasks() const
{
	return this->tasks;
}

UInt Parallel::LoadBalancer::numTasks() const
{
	return static_cast<UInt>(this->tasks.size());
}

void Parallel::LoadBalancer::clearMap()
{
	this->taskVectorMap.clear();
}

//Int Parallel::LoadBalancer::getProcess(ULInt task) const
Int Parallel::LoadBalancer::getProcess(const Task *task) const
{
	//Data::IntMap::const_iterator it = this->taskProcessMap.find(task);
	TaskIntMap::const_iterator it = this->taskProcessMap.find(task);

	if (it == this->taskProcessMap.end())
	{
		//std::cout << "erro em LoadBalancer::getProcess()" << std::endl;

		return 0;
	}

	//std::cout << "process[" << task << "] = " << (*it).second << std::endl;

	return (*it).second;
}

bool Parallel::LoadBalancer::isDynamic() const
{
	return !this->isStatic();
}

Task *Parallel::LoadBalancer::next()
{
	if (this->tasks.empty())
	{
		return NULL;
	}

	Task *t = this->tasks.back();

	this->tasks.pop_back();

	return t;
}

void Parallel::LoadBalancer::sendToSlaves()
{
	//new - collective communication

	//std::cout << "master, sending tasks to slave" << std::endl;

	Int numProcesses = this->comm->numProcesses();

	Int counts[numProcesses];
	Int displacements[numProcesses];

	TaskVectorMap tasks;
	Data::IntList sizes;

	counts[0] = displacements[0] = 0;

	for (Int process = 1; process < numProcesses; process++)
	{
		counts[process] = 0;
		displacements[process] = displacements[process - 1] + counts[process - 1];

		Data::IntVector ranks = this->comm->ranks(process);

		for (UInt j = 0; j < ranks.size(); j++)
		{
			TaskVector ts = this->taskVectorMap[ranks[j]];

			tasks[process].insert(tasks[process].end(), ts.begin(), ts.end());

			for (UInt i = 0; i < ts.size(); i++)
			{
				//debug
				//std::cout << "process = " << process << ", task = " << ts[i]->getId() << std::endl;
				//endebug

				sizes.push_back(ts[i]->packSize(this->comm));

				counts[process] += sizes.back();
			}
		}

		counts[process] += this->comm->packSize(1 + static_cast<UInt>(tasks[process].size()), Communicator::INT);
	}

	this->comm->scatter(counts);

	Int totalSize = displacements[numProcesses - 1] + counts[numProcesses - 1];

	char *buffer = new char[totalSize];
	Int position = 0;

	for (Int process = 1; process < numProcesses; process++)
	{
		this->comm->pack(static_cast<Int>(tasks[process].size()), buffer, totalSize, position);

		for (UInt i = 0; i < tasks[process].size(); i++)
		{
			this->comm->pack(sizes.front(), buffer, totalSize, position);

			sizes.pop_front();

			Task *t = tasks[process][i];

			t->pack(this->comm, buffer, totalSize, position);
		}
	}

	char *recv = new char[0];

	this->comm->scatterv(buffer, counts, displacements, recv, 0, Communicator::PACKED, this->comm->root());

	delete [] recv;
	delete [] buffer;

	this->comm->barrier();

	//std::cout << "master, end sending tasks to slave" << std::endl;

	return;

	//end new

	/*//old - point to point communication

	Int numProcesses = this->comm->numProcesses();

	for (Int process = 1; process < numProcesses; process++)
	{
		Data::IntVector ranks = this->comm->ranks(process);

		TaskVector tasks;

		for (UInt j = 0; j < ranks.size(); j++)
		{
			TaskVector ts = this->taskVectorMap[ranks[j]];

			tasks.insert(tasks.end(), ts.begin(), ts.end());
		}

		this->comm->send((UInt)tasks.size(), process, 0);

		for (UInt j = 0; j < tasks.size(); j++)
		{
			this->comm->removeRequest(this->comm->isend(tasks[j], process, 0), true);
		}
	}

	this->comm->barrier();

	this->comm->deleteRequest();

	//end old*/
}

void Parallel::LoadBalancer::sendToMaster()
{
	//std::cout << "slave, sending tasks to master" << std::endl;

	Int root = this->comm->root();

	Int totalSize = 0;
	Data::IntList sizes;

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		sizes.push_back(this->tasks[i]->packSize(this->comm));

		totalSize += sizes.back();

		//std::cout << "size packed = " << sizes.back() << std::endl;
	}

	totalSize += this->comm->packSize(static_cast<UInt>(tasks.size()), Communicator::INT);

	//std::cout << "load balancer, send to master, gather, totalSize = " << totalSize << ", process " << this->comm->rank() << std::endl;

	this->comm->gather(totalSize, root);

	//std::cout << "load balancer, send to master, end gather, process " << this->comm->rank() << std::endl;

	char *buffer = new char[totalSize];

	Int position = 0;

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		Task *t = this->tasks[i];

		this->comm->pack(static_cast<Int>(sizes.front()), buffer, totalSize, position);

		sizes.pop_front();

		t->pack(this->comm, buffer, totalSize, position);

		delete t;
	}

	//std::cout << "load balancer, send to master, gatherv, process " << this->comm->rank() << std::endl;

	this->comm->gatherv(buffer, totalSize, NULL, NULL, NULL, Communicator::PACKED, root);

	//std::cout << "load balancer, send to master, end gatherv, process " << this->comm->rank() << std::endl;

	delete [] buffer;

	this->tasks.clear();

	this->comm->barrier();

	//std::cout << "slave, end sending tasks to master" << std::endl;

	/*//old - point to point communication

	Int rank = this->comm->rank();
	Int root = this->comm->root();

	this->comm->send(rank, root, 0);
	this->comm->send((UInt)this->tasks.size(), root, 0);

	while (!this->tasks.empty())
	{
		Task *task = this->tasks.back();

		this->tasks.pop_back();

		this->comm->removeRequest(this->comm->isend(task, root, 0), true);

		delete task;
	}

	this->comm->barrier();

	this->comm->deleteRequest();

	//end old*/
}

void Parallel::LoadBalancer::receiveFromMaster()
{
	//new - collective communication

	//std::cout << "slave, receiving tasks from master" << std::endl;

	Int root = this->comm->root();

	Int size = 0;

	this->comm->scatter(size, root);

	char *buffer =  new char[size];

	this->comm->scatterv(NULL, NULL, NULL, buffer, size, Communicator::PACKED, root);

	Int numTasks = 0;
	Int position = 0;

	this->comm->unpack(numTasks, buffer, size, position);

	for (Int i = 0; i < numTasks; i++)
	{
		Int taskSize = 0;

		this->comm->unpack(taskSize, buffer, size, position);

		char *taskBuffer = new char[taskSize];

		this->comm->unpack(taskBuffer, taskSize, buffer, size, position, Communicator::PACKED);

		Package p(taskSize, taskBuffer);

		Task *t = static_cast<Task *>(this->comm->unpack(p, true));

		t->setComm(this->comm);

		this->tasks.push_back(t);
	}

	delete [] buffer;

	this->comm->barrier();

	this->sort(false);

	//std::cout << "slave, end receiving tasks from master" << std::endl;

	return;

	//end new

	/*//old - point to point communication

	Int root = this->comm->root();

	UInt size = 0;

	this->comm->receive(size, root, 0);

	for (UInt i = 0; i < size; i++)
	{
		Task *t = (Task *)this->comm->receive(root, 0);

		this->tasks.push_back(t);
	}

	this->comm->barrier();

	this->sort(false);

	//end old - point to point communication*/
}

void Parallel::LoadBalancer::receiveFromSlaves()
{
	//new - collective communication

	//std::cout << "master, receiving tasks from slaves" << std::endl;

	this->taskProcessMap.clear();

	this->tasks.clear();

	Int numProcesses = this->comm->numProcesses();

	Int sizes[numProcesses];
	Int displacements[numProcesses];

	//std::cout << "load balancer, receiving from slaves, gather" << std::endl;

	this->comm->gather(0, sizes);

	//std::cout << "load balancer, received from slaves, end gather" << std::endl;

	Int totalSize = 0;
	displacements[0] = 0;

	for (Int i = 1; i < numProcesses; i++)
	{
		displacements[i] = totalSize;
		totalSize += sizes[i];
		//displacements[i] = displacements[i - 1] + sizes[i - 1];
	}

	/*std::cout << "totalSize = " << totalSize << std::endl;
	std::cout << "sizes = [";
	for (Int i = 0; i < numProcesses; i++)
		std::cout << sizes[i] << ((i < numProcesses - 1) ? ", " : "]");
	std::cout << std::endl;
	std::cout << "displacements = [";
	for (Int i = 0; i < numProcesses; i++)
		std::cout << displacements[i] << ((i < numProcesses - 1) ? ", " : "]");
	std::cout << std::endl;*/

	char *buffer = new char[totalSize];

	char ignore[sizes[0]];

	//std::cout << "load balancer, receiving from slaves, gatherv, sizeof(buffer) = " << sizeof(buffer) << std::endl;

	this->comm->gatherv(ignore, sizes[0], buffer, sizes, displacements, Communicator::PACKED, this->comm->root());

	//std::cout << "load balancer, received from slaves, end gatherv" << std::endl;

	Int position = sizes[0];
	Int process = 0;

	while (position != totalSize)
	{
		Int size = 0;

		this->comm->unpack(size, buffer, totalSize, position);

		//std::cout << "size unpacked = " << size << std::endl;

		char *taskBuffer = new char[size];

		this->comm->unpack(taskBuffer, size, buffer, totalSize, position, Communicator::PACKED);

		//std::cout << "position = " << position << std::endl;

		while (position - displacements[process] > sizes[process])
		{
			process++;
		}

		//std::cout << "process = " << process << std::endl;

		Package p(size, taskBuffer);

		Task *t = static_cast<Task *>(this->comm->unpack(p, true));

		t->setComm(this->comm);

		//this->taskProcessMap[t->getId()] = process;
		this->taskProcessMap[t] = process;

		//std::cout << "mapping task " << t << " to process " << process << std::endl;

		//debug
		//std::cout << "process = " << process << ", task = " << t->getId() << std::endl;
		//endebug

		this->tasks.push_back(t);
	}

	delete [] buffer;

	this->comm->barrier();

	//std::cout << "master, end receiving tasks from slaves" << std::endl;

	//end new

	/*//old - point to point communication

	this->tasks.clear();

	Int numProcesses = this->comm->numProcesses();

	for (Int i = 1; i < numProcesses; i++)
	{
		Int rank = 0;

		this->comm->receive(rank, -1, 0);

		UInt size = 0;

		this->comm->receive(size, rank, 0);

		for (UInt j = 0; j < size; j++)
		{
			Task *t = (Task *)this->comm->receive(rank, 0);

			this->tasks.push_back(t);
		}
	}

	this->comm->barrier();

	//end old*/
}

bool Parallel::LoadBalancer::greater(Task *t1, Task *t2)
{
	return ((t1->getPartition()->getLoad() > t2->getPartition()->getLoad()) ||
			((t1->getPartition()->getLoad() >= t2->getPartition()->getLoad()) && (t1->getId() > t2->getId())));
}

bool Parallel::LoadBalancer::less(Task *t1, Task *t2)
{
	return ((t1->getPartition()->getLoad() < t2->getPartition()->getLoad()) ||
			((t1->getPartition()->getLoad() <= t2->getPartition()->getLoad()) && (t1->getId() < t2->getId())));
}

void Parallel::LoadBalancer::sort(bool max)
{
	if (max)
	{
		std::sort(this->tasks.begin(), this->tasks.end(), greater);
	}
	else
	{
		std::sort(this->tasks.begin(), this->tasks.end(), less);
	}
}

void Parallel::LoadBalancer::sort()
{
	this->sort(false);

	//TODO add this attribute to the class LoadBalancer, and consider it in
	// all imlpemented load balancers
	//if (also divide empty partitions)
	if (false)
	{
		this->taskVectorMap[0] = this->tasks;
	}
	else
	{
		for (TaskVector::iterator iter = this->tasks.begin();
			 iter != this->tasks.end(); iter++)
		{
			if ((*iter)->load() <= Data::getTolerance())
			{
				continue;
			}

			this->taskVectorMap[0].push_back((*iter));
		}
	}
}

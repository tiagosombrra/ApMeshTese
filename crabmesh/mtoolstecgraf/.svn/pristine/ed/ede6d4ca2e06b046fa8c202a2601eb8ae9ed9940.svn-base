#include "Parallel/Task.h"

#include "Parallel/Partition.h"

using namespace Parallel;

const Performer::Status Parallel::Task::starting =
	Performer::Status(Parallel::Task::STARTING, "task starting");
const Performer::Status Parallel::Task::ready =
	Performer::Status(Parallel::Task::READY, "task ready");
const Performer::Status Parallel::Task::running =
	Performer::Status(Parallel::Task::RUNNING, "task running");
const Performer::Status Parallel::Task::finished =
	Performer::Status(Parallel::Task::FINISHED, "task finished");

Parallel::Task::Task(UInt type, Communicator *c, Performer::Builder *b, Partition *p) :
	Transferable(type),
	Parallel::Builder(c),
	Data::Identifiable()
{
	this->setBuilder(b);
	this->setPartition(p);

	this->status = Parallel::Task::starting;
}

Parallel::Task::~Task()
{
	if (this->builder)
	{
		delete this->builder;
	}

	if (this->partition)
	{
		delete this->partition;
	}
}

void Parallel::Task::setBuilder(Performer::Builder *b)
{
	this->builder = b;
}

Performer::Builder *Parallel::Task::getBuilder() const
{
	return this->builder;
}

void Parallel::Task::setPartition(Partition *p)
{
	this->partition = p;
}

Partition *Parallel::Task::getPartition() const
{
	return this->partition;
}

Real Parallel::Task::load() const
{
	return this->partition ? this->partition->getLoad() : -1.0;
}

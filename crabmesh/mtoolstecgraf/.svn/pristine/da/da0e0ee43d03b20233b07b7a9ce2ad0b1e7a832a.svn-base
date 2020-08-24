#include "Parallel/MainDrive.h"

#include "Parallel/Communicator.h"
#include "Parallel/LoadBalancer.h"

//#include <mpi.h>

using namespace Parallel;

Parallel::MainDrive::MainDrive(Communicator *comm, LoadBalancer *loadBalancer) :
	Performer::MainDrive()
{
	this->comm = NULL;
	this->loadBalancer = NULL;

	this->setComm(comm);
	this->setLoadBalancer(loadBalancer);
}

Parallel::MainDrive::~MainDrive()
{
	if (this->loadBalancer)
	{
		delete this->loadBalancer;
	}

	if (this->comm)
	{
		delete this->comm;
	}
}

void Parallel::MainDrive::setComm(Communicator *comm)
{
	this->comm = comm;
}

Communicator *Parallel::MainDrive::getComm() const
{
	return this->comm;
}

void Parallel::MainDrive::setLoadBalancer(LoadBalancer *loadBalancer)
{
	this->loadBalancer = loadBalancer;
}

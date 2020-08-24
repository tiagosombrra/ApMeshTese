#include "Parallel/Builder.h"

#include "Parallel/Communicator.h"

using namespace Parallel;

Parallel::Builder::Builder(Communicator *comm) :
	Performer::Builder()
{
	this->setComm(comm);
}

Parallel::Builder::~Builder()
{

}

void Parallel::Builder::setComm(Communicator *comm)
{
	this->comm = comm;
}

Communicator *Parallel::Builder::getComm() const
{
	return this->comm;
}

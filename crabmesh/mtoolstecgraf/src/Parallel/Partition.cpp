#include "Parallel/Partition.h"

using namespace Parallel;

Parallel::Partition::Partition(UInt type, Real load) :
	Transferable(type),
	Identifiable()
{
	this->setLoad(load);
}

Parallel::Partition::~Partition()
{

}

void Parallel::Partition::setLoad(Real load)
{
	this->load = load;
}

Real Parallel::Partition::getLoad() const
{
	return this->load;
}

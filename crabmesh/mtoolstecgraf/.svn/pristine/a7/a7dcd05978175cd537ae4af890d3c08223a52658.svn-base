#include "Parallel/Partitioner.h"

using namespace Parallel;

Parallel::Partitioner::Partitioner() :
	Parallel::Builder()
{

}

Parallel::Partitioner::~Partitioner()
{
	//this->partitions.clear();
}

const PartitionVector &Parallel::Partitioner::getPartitions() const
{
	return this->partitions;
}

UInt Parallel::Partitioner::numPartitions() const
{
	return static_cast<UInt>(this->partitions.size());
}

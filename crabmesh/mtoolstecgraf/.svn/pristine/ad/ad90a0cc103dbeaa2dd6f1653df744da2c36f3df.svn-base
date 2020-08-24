#include "Parallel/WeightedGreedySerialStaticLoadBalancer.h"

Parallel::WeightedGreedySerialStaticLoadBalancer::WeightedGreedySerialStaticLoadBalancer(
	const Data::RealVector &weights, Communicator *comm) :
	GreedySerialStaticLoadBalancer(comm)
{
	this->setWeights(weights);
}

Parallel::WeightedGreedySerialStaticLoadBalancer::WeightedGreedySerialStaticLoadBalancer(
	const Data::RealVector &weights, const TaskVector &tasks, Communicator *comm) :
	GreedySerialStaticLoadBalancer(tasks, comm)
{
	this->setWeights(weights);
}

Parallel::WeightedGreedySerialStaticLoadBalancer::~WeightedGreedySerialStaticLoadBalancer()
{

}

void Parallel::WeightedGreedySerialStaticLoadBalancer::setWeights(const Data::RealVector &weights)
{
	this->weights = weights;
}

void Parallel::WeightedGreedySerialStaticLoadBalancer::updateLoad(Int processor, Real load)
{
	//std::cout << "pjmesh greedyserialstaticloadbalancer totalload[" << processor << "] = " << totalLoad[processor] << std::endl;

	if (this->weights.empty())
	{
		Parallel::GreedySerialStaticLoadBalancer::updateLoad(processor, load);
	}
	else
	{
		//std::cout << "pjmesh greedyserialstaticloadbalancer runtime[" << processor << "] = " << this->runtime[processor] << std::endl;

		this->totalLoad[processor] += load*this->weights[processor];
	}
}

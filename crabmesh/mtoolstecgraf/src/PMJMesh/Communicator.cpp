#include "PMJMesh/Communicator.h"

#include "Parallel/MPIMessage.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/InputData.h"
#include "PMJMesh/BoundaryTransferable.h"
#include "PMJMesh/TreeTransferable.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/Partition.h"

using namespace PMJMesh;

PMJMesh::Communicator::Communicator(bool sharedParallelismEnabled) :
	Parallel::TMCommunicator::TMCommunicator(sharedParallelismEnabled)
{

}

PMJMesh::Communicator::~Communicator()
{

}

Int PMJMesh::Communicator::root() const
{
	return 0;
}

Int PMJMesh::Communicator::rank() const
{
	//return (this->numWorkers() == 1) ? 0 : Parallel::MPICommunicator::rank() - 1;
	return Parallel::TMCommunicator::rank();
}

Data::IntVector PMJMesh::Communicator::ranks(Int process) const
{
	Int rank = (process < 0) ? this->rank() : process;

	Int size = this->threadCount[rank];

	//global shift
	Data::IntVector ranks(size, rank - 1);
	//non global shift
	//Data::IntVector ranks(size, rank);

	Int numProcesses = this->numProcesses();

	for (Int j = 1; j < size; j++)
	{
		ranks[j] = ranks[j - 1] + 1;

		//global shift
		for (Int i = 1; i < rank; i++)
		//non global shift
		//for (Int i = 0; i < rank; i++)
		{
			if (this->threadCount[i] > j)
			{
				ranks[j]++;
			}
		}

		for (Int i = rank + 1; i < numProcesses; i++)
		{
			if (this->threadCount[i] >= j)
			{
				ranks[j]++;
			}
		}
	}

	return ranks;
}

void PMJMesh::Communicator::initThreadCount()
{
	Int numProcesses = this->numProcesses();

	Int numThreads = this->getMaxThreads();

	this->totalThreadCount = 0;

	if (numProcesses == 1)
	{
		this->totalThreadCount = numThreads;
	}
	else
	{
		this->threadCount.clear();

		this->threadCount.resize(numProcesses);

		Int *threads = this->allGather(numThreads);

		for (Int i = 0; i < numProcesses; i++)
		{
			this->threadCount[i] = threads[i];

			//global shift
			if (i > 0)
			{
				this->totalThreadCount += threads[i];
			}
		}

		delete [] threads;
	}

	/*std::stringstream str;
	str << "rank = " << this->rank()
		<< ", this->totalThreadCount = " << this->totalThreadCount
		<< ", this->getMaxThreads() = " << this->getMaxThreads()
		<< ", this->threadCount[] = " << (this->threadCount.empty() ? -1 : static_cast<int>(this->threadCount[this->rank()])) << std::endl;
	std::cout << str.str();*/
}

Data::RealVector PMJMesh::Communicator::runtimes() const
{
	Data::RealVector runtimes;

	Int numWorkers = this->numWorkers();

	if ((numWorkers <= 1) || (this->threadCount.empty()))
	{
		return runtimes;
	}

	runtimes.resize(numWorkers);

	Int numProcesses = this->numProcesses();

	for (Int i = 1; i < numProcesses; i++)
	{
		Data::IntVector ranks = this->ranks(i);

		for (UInt j = 0; j < ranks.size(); j++)
		{
			runtimes[ranks[j]] = this->runtime[i];
		}
	}

	return runtimes;
}

bool PMJMesh::Communicator::isMaster() const
{
	return (this->rank() == this->root());
}

Int PMJMesh::Communicator::numWorkers() const
{
	//global shift
	if (!this->isSharedParallelismEnabled())
	{
		return std::max(this->numProcesses() - 1, 1);
	}

	//non global shift
	if (this->totalThreadCount == 0)
	{
		Int num = this->numProcesses() - 1;

		//return (num == 0) ? 1 : num;
		return num + 1;
	}

	return this->totalThreadCount;
}

void PMJMesh::Communicator::calcRuntimeInfo(Real runtime)
{
	Int numProcesses = this->numProcesses();

	Real *runtimes = this->allGather(runtime);

	this->runtime.resize(numProcesses);

	for (Int i = 0; i < numProcesses; i++)
	{
		this->runtime[i] = runtimes[i];
	}

	delete [] runtimes;

	bool proportional = true;

	if (proportional)
	{
		//this way, the runtime of each processor is divided by the least of
		// them, so that the value kept in this->runtime is how much a
		// processor is slower than the fastest one

		Real minRuntime = this->runtime[0];

		for (Int i = 1; i < numProcesses; i++)
		{
			if (this->runtime[i] < minRuntime)
			{
				minRuntime = this->runtime[i];
			}
		}

		for (Int i = 0; i < numProcesses; i++)
		{
			this->runtime[i] /= minRuntime;
		}
	}
}

Parallel::Transferable *PMJMesh::Communicator::unpack(Parallel::Package &p) const
{
	Parallel::Transferable *o = NULL;

	Int size = p.first;
	char *buffer = p.second;

	Int position = 0;
	UInt type = 0;

	this->unpack(type, buffer, size, position);

	switch (type)
	{
	case TRANSFERABLE_MESSAGE:
#if USE_MPI
		o = new Parallel::MPIMessage();
#endif //#if USE_MPI
		break;

	case TRANSFERABLE_INPUT_DATA:
		o = new PMJMesh::InputData();
		break;

	case TRANSFERABLE_BOUNDARY:
		o = new PMJMesh::BoundaryTransferable();
		break;

	case TRANSFERABLE_TREE:
		o = new PMJMesh::TreeTransferable();
		break;

	case TRANSFERABLE_MESH:
		o = new PMJMesh::MeshTransferable();
		break;

	case TRANSFERABLE_PARTITION:
		o = new PMJMesh::Partition();
		break;

	case TRANSFERABLE_MJMESH_TASK:
		o = new PMJMesh::MJMeshTask();
		break;
	}

	if (o)
	{
		o->unpack(this, p, position);
	}

	return o;
}

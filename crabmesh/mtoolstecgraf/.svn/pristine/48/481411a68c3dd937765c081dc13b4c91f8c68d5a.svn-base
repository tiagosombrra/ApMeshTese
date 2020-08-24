#include "PMJMesh/TaskManager.h"

#include "Data/FrontElement.h"
#include "Performer/RangedIdManager.h"
#include "Parallel/Task.h"
#include "Parallel/Communicator.h"
#include "Parallel/LoadBalancer.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/MainDrive.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Communicator.h"

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#include "PMJMesh/WorkerMainDrive.h"
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

PMJMesh::TaskManager::TaskManager(bool sharedParallel,
	Parallel::TaskVector tasks, UInt dimension, MJMesh::Boundary *boundary,
	Data::BoxTree *tree, Parallel::LoadBalancer *loadBalancer,
	Parallel::Communicator *comm) :
	Parallel::Builder(comm)
{
	this->setSharedParallel(sharedParallel);

	this->setTasks(tasks);

	this->setLoadBalancer(loadBalancer);

	this->setDimension(dimension);
	this->setBoundary(boundary);
	this->setTree(tree);

	this->idManager = NULL;

	this->idoffset = 0;
	this->idrange = 1024;

	this->task = NULL;
}

PMJMesh::TaskManager::~TaskManager()
{
	//this->tasks.clear();
	//this->current.clear();
	//this->done.clear();

	if (this->loadBalancer)
	{
		delete this->loadBalancer;
	}

	this->clearBoundaries();
}

void PMJMesh::TaskManager::clearBoundaries()
{
#if USE_UNIQUE_BOUNDARY
	while (!this->boundaries.empty())
	{
		MJMesh::Boundary *b = this->boundaries.back();

		this->boundaries.pop_back();

		//b->clear();

		delete b;
	}
#endif //#if USE_UNIQUE_BOUNDARY

	for (UInt i = 0; i < this->idManagers.size(); i++)
	{
		if (this->idManagers[i])
		{
			delete this->idManagers[i];
		}
	}

	this->idManagers.clear();
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::string PMJMesh::TaskManager::timing(const Parallel::Task *task, Real timeTotal) const
{
	std::stringstream str;
	str << "Processor = " << this->comm->rank() << "\n";
	str << "Thread = " << static_cast<Parallel::TMCommunicator *>(this->comm)->threadId() << "\n";
	str << "Task = " << task->getId() << "\n";
	str << "Load = " << task->getPartition()->getLoad() << "\n";
	str << "Total Time = " << timeTotal << "\n";

	return str.str();
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

void PMJMesh::TaskManager::setSharedParallel(bool sharedParallel)
{
	this->parallel = sharedParallel;
}

bool PMJMesh::TaskManager::isSharedParallel() const
{
	return this->parallel;
}

void PMJMesh::TaskManager::setTasks(const Parallel::TaskVector &tasks)
{
	this->tasks = tasks;

	this->numTasks = static_cast<UInt>(this->tasks.size());
}

void PMJMesh::TaskManager::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::TaskManager::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void PMJMesh::TaskManager::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}

/*Parallel::Task *PMJMesh::TaskManager::makeTask() const
{
	return new PMJMesh::MJMeshTask();
}*/

Parallel::TaskVector PMJMesh::TaskManager::getTasks() const
{
	Parallel::TaskVector tasks = this->tasks;

	tasks.insert(tasks.begin(), this->done.begin(), this->done.end());

	for (Parallel::TaskVector::const_iterator iter = this->current.begin();
		 iter != this->current.end(); iter++)
	{
		if (!(*iter))
		{
			continue;
		}

		tasks.push_back((*iter));
	}

	return tasks;
}

void PMJMesh::TaskManager::setLoadBalancer(Parallel::LoadBalancer *loadBalancer)
{
	this->loadBalancer = loadBalancer;
}

void PMJMesh::TaskManager::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

void PMJMesh::TaskManager::execute()
{
	this->status = MJMesh::MainDrive::success;

	//Int numThreads = static_cast<PMJMesh::Communicator *>(this->comm)->numThreads();
	Int numThreads = static_cast<PMJMesh::Communicator *>(this->comm)->getMaxThreads();

#if USE_UNIQUE_BOUNDARY
	if (this->boundaries.empty())
	{
		this->boundaries.resize(numThreads);

		/*for (Int i = 0; i < numThreads; i++)
		{
			this->boundaries[i] = new MJMesh::Boundary(this->boundary);
		}*/
	}
#endif //#if USE_UNIQUE_BOUNDARY

	if (this->idManagers.empty())
	{
		this->idManagers.resize(numThreads, NULL);

		UInt numProcs = this->comm->numProcesses();
		UInt rank = this->comm->rank();

		if (numProcs > 1)
		{
			numProcs--;
			rank--;
		}

		ULInt procOffset = rank*this->idrange;

		this->idoffset = numProcs*this->idrange;
		ULInt tidrange = this->idrange/numThreads;

		for (Int i = 0; i < numThreads; i++)
		{
			this->idManagers[i] = new Performer::RangedIdManager(1, 1, 1, 1, 3);

			ULInt threadOffset = i*tidrange;

			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setRange(tidrange);
			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setOffset(this->idoffset);

			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(0, this->idManager->getId(0) + procOffset + threadOffset);
			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(1, this->idManager->getId(1) + procOffset + threadOffset);
			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(2, this->idManager->getId(2) + procOffset + threadOffset);

			/*str.clear();
			str << "process = " << this->comm->rank()
				<< " setting initial ids for idManagers[" << i << "], = " << this->idManagers[i]->getId(0) << ", "
				<< this->idManagers[i]->getId(1) << ", " << this->idManagers[i]->getId(2) << std::endl;
			std::cout << str.str();*/
		}
	}

	{
		/*UInt numProcs = this->comm->numProcesses();
		UInt rank = this->comm->rank();

		if (numProcs > 1)
		{
			numProcs--;
			rank--;
		}

		ULInt procOffset = rank*this->idrange;

		this->idoffset = numProcs*this->idrange;
		ULInt tidrange = this->idrange/numThreads;*/

		/*std::stringstream str;
		str << "process = " << this->comm->rank()
			<< " initial ids, " << this->idManager->getId(0) << ", "
			<< this->idManager->getId(1) << ", " << this->idManager->getId(2) << std::endl;
		std::cout << str.str();*/

		for (Int i = 0; i < numThreads; i++)
		{
			/*ULInt threadOffset = i*tidrange;

			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(0, this->idManager->getId(0) + procOffset + threadOffset);
			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(1, this->idManager->getId(1) + procOffset + threadOffset);
			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(2, this->idManager->getId(2) + procOffset + threadOffset);

			this->idManagers[i]->setId(0, static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getMin(0));
			this->idManagers[i]->setId(1, static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getMin(1));
			this->idManagers[i]->setId(2, static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getMin(2));

			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setOffset(this->idoffset);
			static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setRange(tidrange);*/

			this->idManagers[i]->setId(0, this->idManager->getId(0));
			this->idManagers[i]->setId(1, this->idManager->getId(1));
			this->idManagers[i]->setId(2, this->idManager->getId(2));

			/*str.clear();
			str << "process = " << this->comm->rank()
				<< " setting initial ids for idManagers[" << i << "], = " << this->idManagers[i]->getId(0) << ", "
				<< this->idManagers[i]->getId(1) << ", " << this->idManagers[i]->getId(2) << std::endl;
			std::cout << str.str();*/
		}
	}

	if (this->current.empty())
	{
		this->current.resize(numThreads, NULL);

		/*#pragma omp parallel for shared(numThreads) if(this->parallel)
		for (Int i = 0; i < numThreads; i++)
		{
			this->current[i] = NULL;
		}*/
	}

	UInt size = 0;

	//std::cout << "begin omp parallel" << std::endl;

	Data::BoxTree *tree = this->tree;

#if USE_OPENMP
	#pragma omp parallel shared(size) firstprivate(tree) if(this->parallel)
#endif //#if USE_OPENMP
	{
		UInt id = static_cast<PMJMesh::Communicator *>(this->comm)->threadId();

		this->current[id] = NULL;

#if USE_UNIQUE_BOUNDARY
		if (!this->boundaries[id])
		{
			this->boundaries[id] = new MJMesh::Boundary(this->boundary);
		}

		MJMesh::Boundary *boundary = this->boundaries[id];
#else
		MJMesh::Boundary *boundary = this->boundary;
#endif //#if USE_UNIQUE_BOUNDARY

		Parallel::Task *current = NULL;

		while (size < this->numTasks)
		{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
			Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

			//if (!this->current[id])
			if (!current)
			{
				bool breaking = false;

#if USE_OPENMP
				#pragma omp critical (pop_task)
#endif //#if USE_OPENMP
				{
					//std::cout << "begin critical before next" << std::endl;

					Parallel::Task *task = this->loadBalancer->next();

					//std::cout << "begin critical after next" << std::endl;

					if (task)
					{
						//this->current[id] = task;
						current = task;
						this->current[id] = current;

						//std::cout << "thread " << id << " has partition " << current->getId() << " with load " << current->load() << std::endl;
					}
					else
					{
						breaking = true;
					}
				}

				if (breaking)
				{
					break;
				}

				//if (!this->setTaskParameters(this->current[id],
				if (!this->setTaskParameters(current, boundary, tree, this->idManagers[id]))
				{
#if USE_OPENMP
					#pragma omp critical (push_done)
#endif //#if USE_OPENMP
					{
						//this->done.push_back(this->current[id]);
						this->done.push_back(current);
					}

					//this->current[id] = NULL;
					current = NULL;

					continue;
				}
			}

			/*std::stringstream str;
			str << "process = " << this->comm->rank() << ", thread = " << id  << ", task = " << current->getId() << std::endl;
			std::cout << str.str();*/

			/*std::stringstream str;
			str << "process = " << this->comm->rank() << ", thread = " << id
				<< ", task = " << current->getId() << ", initial ids = " << this->idManagers[id]->getId(0) << ", "
				<< this->idManagers[id]->getId(1) << ", " << this->idManagers[id]->getId(2) << std::endl;
			std::cout << str.str();*/

			//this->current[id]->execute();
			//#pragma omp critical (executing)
			current->execute();

			/*str.clear();
			str << "process = " << this->comm->rank() << ", thread = " << id
				<< ", task = " << current->getId() << ", final ids = " << this->idManagers[id]->getId(0) << ", "
				<< this->idManagers[id]->getId(1) << ", " << this->idManagers[id]->getId(2) << std::endl;
			std::cout << str.str();*/

			//str.clear();
			//str << "after execute, task = " << id;
			//std::cout << str.str() << std::endl;

#if USE_OPENMP
			#pragma omp critical (push_done)
#endif //#if USE_OPENMP
			{
				//if (this->current[id]->getStatus().getType() == this->current[id]->finished.getType())
				if (current->getStatus().getType() == current->finished.getType())
				{
					//this->isInError(this->current[id]);
					this->isInError(current);

					//this->done.push_back(this->current[id]);
					this->done.push_back(current);
				}

				this->getParameters(current);

				size = static_cast<UInt>(done.size());
			}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
			Real timeTotal = Data::time() - start;

			std::cout << this->timing(current, timeTotal) << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

			current = NULL;
			this->current[id] = NULL;

			//debug
			//break;
			//endebug
		}
	}

#if USE_POOL
	//PMJMesh::mergeThreadPools();
#endif //#if USE_POOL

	//std::cout << "end omp parallel" << std::endl;

	bool allDone = true;

	for (Int i = 0; i < numThreads; i++)
	{
		if (this->current[i])
		{
			allDone = false;

			break;
		}
	}

	if (allDone)
	{
		this->tasks.insert(this->tasks.end(), this->done.begin(), this->done.end());

		this->done.clear();

		this->current.clear();

		/*std::stringstream str;
		str << "process = " << this->comm->rank()
			<< " merging idManagers, 0 = " << this->idManagers[0]->getId(0) << ", "
			<< this->idManagers[0]->getId(1) << ", " << this->idManagers[0]->getId(2) << std::endl;
		std::cout << str.str();*/

		this->idManager->setId(0, this->idManagers[0]->getId(0));
		this->idManager->setId(1, this->idManagers[0]->getId(1));
		this->idManager->setId(2, this->idManagers[0]->getId(2));

		for (Int i = 1; i < numThreads; i++)
		{
			/*str.clear();
			str << "process = " << this->comm->rank()
				<< " merging idManagers, " << i << " = " << this->idManagers[i]->getId(0) << ", "
				<< this->idManagers[i]->getId(1) << ", " << this->idManagers[i]->getId(2) << std::endl;
			std::cout << str.str();*/

			this->idManager->setId(0, std::max(this->idManager->getId(0), this->idManagers[i]->getId(0)));
			this->idManager->setId(1, std::max(this->idManager->getId(1), this->idManagers[i]->getId(1)));
			this->idManager->setId(2, std::max(this->idManager->getId(2), this->idManagers[i]->getId(2)));
		}

		/*str << "process = " << this->comm->rank()
			<< " final ids = " << this->idManager->getId(0) << ", "
			<< this->idManager->getId(1) << ", " << this->idManager->getId(2) << std::endl;
		std::cout << str.str();*/
	}
}

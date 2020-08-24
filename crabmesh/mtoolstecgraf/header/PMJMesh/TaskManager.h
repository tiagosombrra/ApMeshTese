#ifndef _PMJMESH_TASK_MANAGER_H_
#define _PMJMESH_TASK_MANAGER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/MainDrive.h"
#include "Parallel/Builder.h"

namespace PMJMesh
{
	class TaskManager : public Parallel::Builder
	{
	public:

		TaskManager(bool sharedParallel, Parallel::TaskVector tasks,
			UInt dimension, MJMesh::Boundary *boundary, Data::BoxTree *tree,
			Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm);
		virtual ~TaskManager();

		virtual void setSharedParallel(bool sharedParallel);
		virtual bool isSharedParallel() const;

		virtual void setTasks(const Parallel::TaskVector &tasks);
		virtual Parallel::TaskVector getTasks() const;

		virtual void setLoadBalancer(Parallel::LoadBalancer *loadBalancer);

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual void setDimension(UInt dimension);

		virtual void setBoundary(MJMesh::Boundary *boundary);

		virtual void setTree(Data::BoxTree *tree);

		virtual Parallel::Task *makeTask() const = 0;

		virtual void execute();

	protected:

		virtual void clearBoundaries();
		virtual bool setTaskParameters(Parallel::Task *t,
			MJMesh::Boundary *boundary, Data::BoxTree *tree,
			Performer::IdManager *idManager) const = 0;
		virtual bool isInError(const Parallel::Task *t) const = 0;
		virtual void getParameters(const Parallel::Task *t) const = 0;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		virtual std::string timing(const Parallel::Task *task, Real timeTotal) const;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	protected:

		bool parallel;

		Parallel::LoadBalancer *loadBalancer;

		Performer::IdManager *idManager;

		UInt dimension;
		MJMesh::Boundary *boundary;
#if USE_UNIQUE_BOUNDARY
		//gambiarra... um boundary para cada thread
		std::vector<MJMesh::Boundary *> boundaries;
#endif //#if USE_UNIQUE_BOUNDARY
		Data::BoxTree *tree;

		Parallel::TaskVector tasks;

		//if parallel == true
		UInt numTasks;

		Parallel::TaskVector current;
		Parallel::TaskVector done;

		//if parallel != true
		Parallel::Task *task;

		Performer::IdManagerVector idManagers;

		ULInt idoffset;
		ULInt idrange;
	};
}

#endif //#ifndef _PMJMESH_TASK_MANAGER_H_

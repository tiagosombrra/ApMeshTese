#ifndef _PARALLEL_LOAD_BALANCER_H_
#define _PARALLEL_LOAD_BALANCER_H_

#include "Parallel/Definitions.h"
#include "Parallel/Builder.h"

namespace Parallel
{
	class LoadBalancer : public Parallel::Builder
	{
	public:

		LoadBalancer(Communicator *comm);
		LoadBalancer(const TaskVector &tasks, Communicator *comm);
		virtual ~LoadBalancer();

		virtual const TaskVectorMap &getTaskVectorMap() const;

		virtual void setTasks(const TaskVector &tasks);
		virtual const TaskVector &getTasks() const;

		virtual UInt numTasks() const;

		virtual void clearMap();

		//virtual Int getProcess(ULInt task) const;
		virtual Int getProcess(const Task *task) const;

		virtual bool isStatic() const = 0;
		virtual bool isDynamic() const;

		virtual bool needToBalance() const = 0;

		virtual Task *next();

		virtual void sendToSlaves();
		virtual void sendToMaster();

		virtual void receiveFromMaster();
		virtual void receiveFromSlaves();

		virtual void sort(bool max);
		virtual void sort();

	protected:

		virtual void deleteRemotePartitions();

		static bool greater(Task *t1, Task *t2);
		static bool less(Task *t1, Task *t2);

	public:

		enum
		{
			NOT_INIT = 1,
			SUCCESS,

			ERROR = 100
		};

	public:

		static const Performer::Status notInit;
		static const Performer::Status success;
		static const Performer::Status error;

	protected:

#if USE_C__11
		typedef std::unordered_map<const Task *, Int> TaskIntHashMap;
#endif //#if USE_C__11

		typedef std::map<const Task *, Int> TaskIntMap;

	protected:

		TaskVector tasks;

		TaskVectorMap taskVectorMap;

		//Data::IntMap taskProcessMap;
		TaskIntMap taskProcessMap;
	};
}

#endif //#ifndef _PARALLEL_LOAD_BALANCER_H_

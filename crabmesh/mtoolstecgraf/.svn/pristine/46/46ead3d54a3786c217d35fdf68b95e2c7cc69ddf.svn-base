#ifndef _PARALLEL_TASK_H_
#define _PARALLEL_TASK_H_

#include "Parallel/Definitions.h"
#include "Parallel/Transferable.h"
#include "Parallel/Builder.h"
#include "Data/Identifiable.h"

namespace Parallel
{
	class Task :
		public Parallel::Transferable,
		public Parallel::Builder,
		public Data::Identifiable
	{
	public:

		Task(UInt type, Communicator *c = NULL, Performer::Builder *b = NULL, Partition *p = NULL);
		virtual ~Task();

		virtual void setBuilder(Performer::Builder *b);
		virtual Performer::Builder *getBuilder() const;

		virtual void setPartition(Partition *p);
		virtual Partition *getPartition() const;

		virtual Real load() const;

		virtual void makeBuilder() = 0;

	public:

		enum
		{
			STARTING = 1,
			READY,
			RUNNING,
			FINISHED
		};

		static const Performer::Status starting;
		static const Performer::Status ready;
		static const Performer::Status running;
		static const Performer::Status finished;

	protected:

		Partition *partition;

		Performer::Builder *builder;
	};
}

#endif //#ifndef _PARALLEL_TASK_H_

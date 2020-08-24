#ifndef _PARALLEL_PARTITIONER_H_
#define _PARALLEL_PARTITIONER_H_

#include "Parallel/Definitions.h"
#include "Parallel/Builder.h"

namespace Parallel
{
	class Partitioner : public Parallel::Builder
	{
	public:

		Partitioner();
		virtual ~Partitioner();

		virtual const PartitionVector &getPartitions() const;

		virtual UInt numPartitions() const;

		virtual void execute() = 0;

	protected:

		PartitionVector partitions;
	};
}

#endif //#ifndef _PARALLEL_PARTITIONER_H_

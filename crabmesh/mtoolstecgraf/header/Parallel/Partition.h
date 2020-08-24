#ifndef _PARALLEL_PARTITION_H_
#define _PARALLEL_PARTITION_H_

#include "Parallel/Definitions.h"
#include "Parallel/Transferable.h"
#include "Data/Identifiable.h"

namespace Parallel
{
	class Partition :
		public Parallel::Transferable,
		public Data::Identifiable
	{
	public:

		Partition(UInt type, Real load = -1.0);
		virtual ~Partition();

		virtual void setLoad(Real load);
		virtual Real getLoad() const;

	protected:

		Real load;
	};
}

#endif //#ifndef _PARALLEL_PARTITION_H_

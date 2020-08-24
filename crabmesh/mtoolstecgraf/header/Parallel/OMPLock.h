#ifndef _PARALLEL_OMP_LOCK_H_
#define _PARALLEL_OMP_LOCK_H_

#include "Parallel/Definitions.h"

#if USE_OPENMP

#include <omp.h>

#include "Parallel/Lock.h"

namespace Parallel
{
	class OMPLock : public Parallel::Lock
	{
	public:

		OMPLock();
		virtual ~OMPLock();

		virtual void set();
		virtual void unset();

		virtual bool testAndSet();

	protected:

		omp_lock_t lock;
	};
}

#endif //#if USE_OPENMP

#endif //#ifndef _PARALLEL_OMP_LOCK_H_

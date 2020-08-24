#ifndef _PARALLEL_LOCK_H_
#define _PARALLEL_LOCK_H_

#include "Parallel/Definitions.h"

namespace Parallel
{
	class Lock
	{
	public:

		virtual ~Lock();

		virtual void set() = 0;
		virtual void unset() = 0;

		virtual bool testAndSet() = 0;

	};
}

#endif //#ifndef _PARALLEL_LOCK_H_

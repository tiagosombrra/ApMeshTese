#include "Parallel/OMPLock.h"

#if USE_OPENMP

Parallel::OMPLock::OMPLock()
{
	omp_init_lock(&this->lock);
}

Parallel::OMPLock::~OMPLock()
{
	omp_destroy_lock(&this->lock);
}

void Parallel::OMPLock::set()
{
	omp_set_lock(&this->lock);
}

void Parallel::OMPLock::unset()
{
	omp_unset_lock(&this->lock);
}

bool Parallel::OMPLock::testAndSet()
{
	return omp_test_lock(&this->lock);
}

#endif //#if USE_OPENMP

#ifndef _DATA_POOLABLE_H_
#define _DATA_POOLABLE_H_

#include "Data/Definitions.h"

//#if USE_POOL

#include "Data/Pool.h"

#if USE_OPENMP
#include <omp.h>
#endif //#if USE_OPENMP

#if USE_OPENMP
#define USING_POOLABLE(T)						\
	public:										\
		using Data::Poolable<T>::poolSet;		\
		using Data::Poolable<T>::poolGet;		\
		using Data::Poolable<T>::poolMake;		\
		using Data::Poolable<T>::poolUnmake;	\
		using Data::Poolable<T>::poolResize;	\
		using Data::Poolable<T>::poolMerge;		\
		using Data::Poolable<T>::operator new;	\
		using Data::Poolable<T>::operator delete;
#else
#define USING_POOLABLE(T)						\
	public:										\
		using Data::Poolable<T>::poolSet;		\
		using Data::Poolable<T>::poolGet;		\
		using Data::Poolable<T>::poolMake;		\
		using Data::Poolable<T>::poolUnmake;	\
		using Data::Poolable<T>::operator new;	\
		using Data::Poolable<T>::operator delete;
#endif //#if USE_OPENMP

namespace Data
{
	template <typename T> class Poolable
	{
	public:

		//Poolable();
		//virtual ~Poolable();

		static inline void poolSet(Pool<T> *pool);
		static inline Pool<T> *poolGet();
		static inline void poolMake();
		static inline void poolUnmake();
#if USE_OPENMP
		static inline void poolResize();
		static inline void poolMerge(UInt id = 0);
#endif //#if USE_OPENMP

		static inline void *operator new(std::size_t size);
		static inline void operator delete(void *p, std::size_t size);

	private:

#if ((!USE_OPENMP) || (!USE_GUI))
		static Pool<T> *pool;
#endif //#if ((!USE_OPENMP) || (!USE_GUI))

#if ((USE_OPENMP) && (!USE_GUI))
		#pragma omp threadprivate(pool)
#endif //#if ((USE_OPENMP) && (!USE_GUI))

#if USE_OPENMP
		static Pool<T> **pools;
		static UInt numPools;
#endif //#if USE_OPENMP
	};
}

#if ((!USE_OPENMP) || (!USE_GUI))
template <typename T> Data::Pool<T> *Data::Poolable<T>::pool = NULL;
#endif //#if ((!USE_OPENMP) || (!USE_GUI))
#if USE_OPENMP
template <typename T> Data::Pool<T> **Data::Poolable<T>::pools = NULL;
template <typename T> Data::UInt Data::Poolable<T>::numPools = 0;
#endif //#if USE_OPENMP

/*template <typename T> Data::Poolable<T>::Poolable()
{

}*/

/*template <typename T> Data::Poolable<T>::~Poolable()
{

}*/

template <typename T> void Data::Poolable<T>::poolSet(Pool<T> *pool)
{
#if USE_OPENMP
	if (!Data::Poolable<T>::pools)
	{
		Data::Poolable<T>::numPools = static_cast<UInt>(omp_get_max_threads());

		Data::Poolable<T>::pools = new Data::Pool<T>*[Data::Poolable<T>::numPools];

		for (UInt i = 0; i < Data::Poolable<T>::numPools; i++)
		{
			Data::Poolable<T>::pools[i] = NULL;
		}
	}
#endif //#if USE_OPENMP

	//std::stringstream str;
	//str << "setting pool of type " << typeid(T).name() << " for thread " << omp_get_thread_num() << " from " << static_cast<void *>(Data::Poolable<T>::pools[omp_get_thread_num()]) << " to " << static_cast<void *>(pool);
	//std::cout << str.str() << std::endl;

#if ((!USE_OPENMP) || (!USE_GUI))
	Data::Poolable<T>::pool =
#endif //#if ((!USE_OPENMP) || (!USE_GUI))
#if USE_OPENMP
	Data::Poolable<T>::pools[omp_get_thread_num()] =
#endif //#if USE_OPENMP
		pool;
}

template <typename T> Data::Pool<T> *Data::Poolable<T>::poolGet()
{
#if ((!USE_OPENMP) || (!USE_GUI))
	return Data::Poolable<T>::pool;
#else
	return Data::Poolable<T>::pools ? Data::Poolable<T>::pools[omp_get_thread_num()] : NULL;
#endif //#if ((!USE_OPENMP) || (!USE_GUI))
}

template <typename T> void Data::Poolable<T>::poolMake()
{
	assert(Data::Poolable<T>::poolGet() == NULL);

	Data::Poolable<T>::poolSet(new Data::Pool<T>());
}

template <typename T> void Data::Poolable<T>::poolUnmake()
{
	assert(Data::Poolable<T>::poolGet() != NULL);

	delete Data::Poolable<T>::poolGet();

	Data::Poolable<T>::poolSet(NULL);

#if USE_OPENMP
	UInt i = Data::Poolable<T>::numPools;

	for (i = 0; i < Data::Poolable<T>::numPools; i++)
	{
		if (Data::Poolable<T>::pools[i])
		{
			break;
		}
	}

	if (i == Data::Poolable<T>::numPools)
	{
		delete [] Data::Poolable<T>::pools;

		Data::Poolable<T>::pools = NULL;
	}
#endif //#if USE_OPENMP
}

#if USE_OPENMP
template <typename T> void Data::Poolable<T>::poolResize()
{
	if (!Data::Poolable<T>::pools)
	{
		return;
	}

	UInt nthreads = static_cast<UInt>(omp_get_max_threads());

	if (nthreads == Data::Poolable<T>::numPools)
	{
		return;
	}

	Data::Pool<T> **newPools = new Data::Pool<T>*[nthreads];

	if (nthreads > Data::Poolable<T>::numPools)
	{
		for (UInt i = 0; i < Data::Poolable<T>::numPools; i++)
		{
			newPools[i] = Data::Poolable<T>::pools[i];
		}

		for (UInt i = Data::Poolable<T>::numPools; i < nthreads; i++)
		{
			newPools[i] = NULL;
		}
	}
	else
	{
		Data::Pool<T> *tpool = NULL;

		for (UInt i = 0; i < nthreads; i++)
		{
			newPools[i] = Data::Poolable<T>::pools[i];

			if (Data::Poolable<T>::pools[i])
			{
				tpool = Data::Poolable<T>::pools[i];
			}
		}

		if (tpool)
		{
			for (UInt i = nthreads; i < Data::Poolable<T>::numPools; i++)
			{
				tpool->merge(Data::Poolable<T>::pools[i]);
			}
		}
		else
		{
			for (UInt i = nthreads; i < Data::Poolable<T>::numPools; i++)
			{
				if (tpool)
				{
					tpool->merge(Data::Poolable<T>::pools[i]);
				}
				else
				{
					tpool = newPools[0] = Data::Poolable<T>::pools[i];
				}
			}
		}
	}

	delete [] Data::Poolable<T>::pools;

	Data::Poolable<T>::pools = newPools;

	Data::Poolable<T>::numPools = nthreads;
}

template <typename T> void Data::Poolable<T>::poolMerge(UInt id)
{
	//std::stringstream str;
	//str << "merging pool of type " << typeid(T).name() << " to thread " << id;
	//std::cout << str.str() << std::endl;

	for (UInt i = 0; i < Data::Poolable<T>::numPools; i++)
	{
		if (i == id)
		{
			continue;
		}

		Data::Poolable<T>::pools[id]->merge(Data::Poolable<T>::pools[i]);
	}
}
#endif //#if USE_OPENMP

template <typename T> void *Data::Poolable<T>::operator new(std::size_t size)
{
	return Data::Poolable<T>::poolGet() ? Data::Poolable<T>::poolGet()->allocate(size) : ::operator new(size);
}

template <typename T> void Data::Poolable<T>::operator delete(void *p, std::size_t size)
{
	if (!Data::Poolable<T>::poolGet())
	{
		::operator delete(p);

		return;
	}

	if (Data::Poolable<T>::poolGet()->deallocate(p, size))
	{
		return;
	}

#if USE_OPENMP
	UInt id = static_cast<UInt>(omp_get_thread_num());

	for (UInt i = 1; i < Data::Poolable<T>::numPools; i++)
	{
		UInt j = (i + id)%Data::Poolable<T>::numPools;

		if ((j != id) &&
            (Data::Poolable<T>::pools[j]) &&
            (Data::Poolable<T>::pools[j]->deallocate(p, size)))
		{
			return;
		}
	}
#endif //#if USE_OPENMP

#if (!NDEBUG)
	std::cout << "ERROR: could not find pool for deleting object of type " << typeid(T).name() << std::endl;
#if USE_OPENMP
	std::cout << "thread " << id << std::endl;
#endif //#if USE_OPENMP
	std::cout << Data::trace() << std::endl;
#endif //#if (!NDEBUG)

	assert(false);

	::operator delete(p);
}

//#endif //#if USE_POOL

#endif //#ifndef _DATA_POOLABLE_H_

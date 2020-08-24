#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include "Data/Definitions.h"

//#if USE_POOL

#include "Data/Chunk.h"
#include <cassert>
#if USE_OPENMP
#include <omp.h>
#endif //#if USE_OPENMP

namespace Data
{
	template <typename T> class Pool
	{
	friend
		class Poolable<T>;

	public:

		//Pool(unsigned char blocks = UCHAR_MAX);
		Pool(typename Chunk<T>::Type blocks = Chunk<T>::CType::IDEAL);
		~Pool();

		bool merge(Pool<T> *pool);
		bool merge(Pool<T> &pool);

		UInt numChunks() const;
		UInt numObjects() const;

	protected:

		void *allocate(std::size_t blockSize);
		bool deallocate(void *p, std::size_t blockSize);

	protected:

		inline bool belong(const void *p, const Chunk<T> *chunk, std::size_t blockSize) const;
		inline bool available(const Chunk<T> *chunk) const;

#if DEBUG_POOL_CHECK_CHUNKS
		void checkChunks() const;
#endif //#if DEBUG_POOL_CHECK_CHUNKS

	protected:

		//unsigned char blocks;
		typename Chunk<T>::Type blocks;

		typename Chunk<T>::Vector chunks;

		Chunk<T> *alloc;
		Chunk<T> *dealloc;

#if ((USE_POOL_LOCK) && (USE_OPENMP))
		omp_lock_t lock;
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))
	};
}

//template <typename T> Data::Pool<T>::Pool(unsigned char blocks)
template <typename T> Data::Pool<T>::Pool(typename Chunk<T>::Type blocks)
{
	this->blocks = blocks;

	this->alloc = this->dealloc = NULL;

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_init_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))
}

template <typename T> Data::Pool<T>::~Pool()
{
	UInt leak = 0;
#if DEBUG_POOL_LEAK
	std::stringstream leakstr;
#endif //#if DEBUG_POOL_LEAK

	for (typename Chunk<T>::Vector::iterator iter = this->chunks.begin();
		 iter != this->chunks.end(); ++iter)
	{
		if (iter->available != this->blocks)
		{
			leak += static_cast<UInt>(this->blocks - iter->available);

#if DEBUG_POOL_LEAK
			for (UInt i = 0; i < blocks; i++)
			{
				if (!iter->traces[i].empty())
				{
					leakstr << iter->traces[i] << std::endl;
				}
			}
#endif //#if DEBUG_POOL_LEAK
		}

		delete [] iter->data;
	}

	if (leak > 0)
	{
		std::stringstream str;
		str << "ERROR: memory leak in pool, " << leak << " objects of type " << typeid(T).name() << " still reachable" << std::endl;
#if DEBUG_POOL_LEAK
		str << leakstr.str();
#endif //#if DEBUG_POOL_LEAK
		std::cout << str.str() << std::endl;
	}

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_destroy_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))
}

template <typename T> bool Data::Pool<T>::merge(Pool<T> *pool)
{
	if ((!pool) || (pool == this) || (pool->chunks.empty()))
	{
		return false;
	}

	assert(this->blocks == pool->blocks);

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_set_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

#if DEBUG_POOL_CHECK_CHUNKS
	this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS

	Chunk<T> *first = &this->chunks.front();
	long int dAlloc = this->alloc - first;
	long int dDealloc = this->dealloc - first;

	this->chunks.reserve(this->chunks.size() + pool->chunks.size());

	this->chunks.insert(this->chunks.end(), pool->chunks.begin(), pool->chunks.end());

	pool->chunks.clear();
	pool->alloc = pool->dealloc = NULL;

	Chunk<T> *nFirst = &this->chunks.front();

	if (first != nFirst)
	{
		this->alloc = nFirst + dAlloc;
		this->dealloc = nFirst + dDealloc;
	}

#if DEBUG_POOL_CHECK_CHUNKS
	this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_unset_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

	return true;
}

template <typename T> bool Data::Pool<T>::merge(Pool<T> &pool)
{
	return this->merge(&pool);
}

template <typename T> void *Data::Pool<T>::allocate(std::size_t blockSize)
{
	assert(blockSize == sizeof(T));

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_set_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

#if DEBUG_POOL_CHECK_CHUNKS
	this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS

	if ((this->alloc == NULL) || (this->alloc->available == 0))
	{
		// No available memory in this chunk
		// Try to find one
		typename Chunk<T>::Vector::iterator end = this->chunks.end();

		for (typename Chunk<T>::Vector::iterator iter = this->chunks.begin(); ; ++iter)
		{
			if (iter == end)
			{
				// All filled up-add a new chunk
				this->chunks.reserve(this->chunks.size() + 1);

				Chunk<T> chunk;
				chunk.initialize(blockSize, this->blocks);

				this->chunks.push_back(chunk);

				//this->alloc = &this->chunks.back();
				this->dealloc = this->alloc = &this->chunks.back();

#if DEBUG_POOL_CHECK_CHUNKS
				this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS

				break;
			}

			if (iter->available > 0)
			{
				// Found a chunk
				this->alloc = &(*iter);

				break;
			}
		}
	}

#if DEBUG_POOL_CHECK_CHUNKS
	this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS

	assert(this->alloc != 0);
	assert(this->alloc->available > 0);

	void *p = this->alloc->allocate(blockSize);

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_unset_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

	return p;
}

template <typename T> bool Data::Pool<T>::deallocate(void *p, std::size_t blockSize)
{
	assert(blockSize == sizeof(T));

	//bool hasEmptyChunk = (this->dealloc == this->alloc) ? false : (this->available(this->alloc));

#if USE_OPENMP
	if (!this->dealloc)
	{
		return false;
	}
#endif //#if USE_OPENMP

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_set_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

#if DEBUG_POOL_CHECK_CHUNKS
	this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS

	Chunk<T> *prev, *next, *first, *last, *empty;
	prev = next = this->dealloc;
	first = &this->chunks.front();
	last = &this->chunks.back();
	empty = ((this->dealloc == this->alloc) || !this->available(this->alloc)) ? NULL : this->alloc;
	bool belong = false;

	if (!this->belong(p, this->dealloc, blockSize))
	{
		if ((this->alloc != this->dealloc) && (this->belong(p, this->alloc, blockSize)))
		{
			this->dealloc = this->alloc;

			belong = true;
		}
		else if ((this->dealloc != first) && (this->belong(p, first, blockSize)))
		{
			this->dealloc = first;

			belong = true;
		}
		else if ((this->dealloc != last) && (this->belong(p, last, blockSize)))
		{
			this->dealloc = last;

			belong = true;
		}
		else
		{
			//while (true)
			while ((next != last) || (prev != first))
			{
				if (next != last)
				{
					if (this->belong(p, ++next, blockSize))
					{
						this->dealloc = next;

						belong = true;

						break;
					}

					if (this->available(next))
					{
						//hasEmptyChunk = true;
						empty = next;
					}
				}

				if (prev != first)
				{
					if (this->belong(p, --prev, blockSize))
					{
						this->dealloc = prev;

						belong = true;

						break;
					}

					if (this->available(prev))
					{
						//hasEmptyChunk = true;
						empty = prev;
					}
				}
			}
		}
	}

	if ((!belong) && (!this->belong(p, this->dealloc, blockSize)))
	{
#if ((USE_POOL_LOCK) && (USE_OPENMP))
		omp_unset_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

		return false;
	}

	//assert(this->belong(p, this->dealloc, blockSize));

	this->dealloc->deallocate(p, blockSize);

	if (this->dealloc->available == this->blocks)
	{
		//while ((!hasEmptyChunk) && ((next != last) || (prev != first)))
		while ((!empty) && ((next != last) || (prev != first)))
		{
			//hasEmptyChunk = (((next != last) && (this->available(++next))) || ((prev != first) && (this->available(--prev))));

			empty = ((next != last) && (this->available(++next))) ? next :
				(((prev != first) && (this->available(--prev))) ? prev : empty);
		}

		//if (hasEmptyChunk)
		if (empty)
		{
			delete [] this->dealloc->data;

#if DEBUG_POOL_LEAK
			this->dealloc->traces.clear();
#endif //#if DEBUG_POOL_LEAK
#if DEBUG_POOL_FREE
			this->dealloc->dirty.clear();
#endif //#if DEBUG_POOL_FREE

			if (this->alloc == last)
			{
				this->alloc = (this->dealloc != last) ? this->dealloc :
					((this->dealloc != empty) ? empty : first);
			}

			if (this->dealloc != last)
			{
				this->dealloc->data = last->data;
				this->dealloc->first = last->first;
				this->dealloc->available = last->available;
#if DEBUG_POOL_LEAK
				this->dealloc->traces.swap(last->traces);
#endif //#if DEBUG_POOL_LEAK
#if DEBUG_POOL_FREE
				this->dealloc->dirty.swap(last->dirty);
#endif //#if DEBUG_POOL_FREE
			}

			this->chunks.pop_back();

			this->dealloc = this->alloc;

#if DEBUG_POOL_CHECK_CHUNKS
			this->checkChunks();
#endif //#if DEBUG_POOL_CHECK_CHUNKS
		}
	}

#if ((USE_POOL_LOCK) && (USE_OPENMP))
	omp_unset_lock(&this->lock);
#endif //#if ((USE_POOL_LOCK) && (USE_OPENMP))

	return true;
}

template <typename T> Data::UInt Data::Pool<T>::numChunks() const
{
	return static_cast<UInt>(this->chunks.size());
	//return static_cast<UInt>(&this->chunks.back() - &this->chunks.front() + 1);
}

template <typename T> Data::UInt Data::Pool<T>::numObjects() const
{
	UInt total = static_cast<UInt>(this->chunks.size())*this->blocks;

	for (typename Chunk<T>::Vector::const_iterator iter = this->chunks.begin();
		 iter != this->chunks.end(); ++iter)
	{
		total -= static_cast<UInt>((*iter).available);
	}

	return total;
}

template <typename T> bool Data::Pool<T>::belong(const void *p, const Chunk<T> *chunk, std::size_t blockSize) const
{
	return ((p >= chunk->data) && (p < (chunk->data + blockSize*this->blocks)));
}

template <typename T> bool Data::Pool<T>::available(const Chunk<T> *chunk) const
{
	return (chunk->available > 0);
}

#if DEBUG_POOL_CHECK_CHUNKS
template <typename T> void Data::Pool<T>::checkChunks() const
{
	size_t i = 0, cont = 0;

	for (typename Chunk<T>::Vector::const_iterator iter = this->chunks.begin();
		 iter != this->chunks.end(); ++iter, ++i)
	{
		if ((*iter).data == NULL)
		{
			std::stringstream str;
			str << "ERROR: empty chunk (" << i << " of " << this->chunks.size() << ") in pool for type " << typeid(T).name() << std::endl;

			std::cout << str.str();

			cont++;
		}
	}

	if (cont > 0)
	{
		std::stringstream str;
		str << "ERROR: " << cont << " of " << i << " chunks in pool for type " << typeid(T).name() << " are empty" << std::endl;

		std::cout << str.str();

		assert(false);
	}
}
#endif //#if DEBUG_POOL_CHECK_CHUNKS

//#endif //#if USE_POOL

#endif //#ifndef _DATA_POOL_H_

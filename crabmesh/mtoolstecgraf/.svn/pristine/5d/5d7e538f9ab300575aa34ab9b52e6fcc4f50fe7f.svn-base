#ifndef _DATA_CHUNK_H_
#define _DATA_CHUNK_H_

#include "Data/Definitions.h"

//#if USE_POOL

namespace Data
{
	template <std::size_t size> struct ChunkType
	{
		//typedef unsigned short Type;
		typedef unsigned char Type;
		static const Type MIN = static_cast<Type>(1);
		//static const Type MAX = static_cast<Type>(USHRT_MAX);
		static const Type MAX = static_cast<Type>(UCHAR_MAX);
		static const Type IDEAL = static_cast<Type>(255); ///magic number 256 512 1024
	};

	/*template <> struct ChunkType<1>
	{
		typedef unsigned char Type;
		static const Type MIN = static_cast<Type>(1);
		static const Type MAX = static_casT<Type>(UCHAR_MAX);
		static const Type IDEAL = MAX;
	};*/

	template <typename T> class Chunk
	{
	public:
		typedef ChunkType<sizeof(T)> CType;
		typedef typename CType::Type Type;
		typedef std::vector<Chunk<T> > Vector;

	public:
		Chunk();
		~Chunk();

		void initialize(std::size_t blockSize, Type blocks);
		void *allocate(std::size_t blockSize);
		void deallocate(void *p, std::size_t blockSize);

#if DEBUG_POOL_LEAK
	protected:

		std::string trace() const;
#endif //#if DEBUG_POOL_LEAK

	public:

		Type *data;
		Type first;
		Type available;

#if DEBUG_POOL_LEAK
		std::vector<std::string> traces;
#endif //#if DEBUG_POOL_LEAK

#if DEBUG_POOL_FREE
		std::vector<bool> dirty;
#endif //#if DEBUG_POOL_FREE
	};
}

#include <cassert>

#if ((DEBUG_POOL_LEAK) && (USE_OPENMP))
#include <omp.h>
#endif //#if ((DEBUG_POOL_LEAK) && (USE_OPENMP))

template <typename T> Data::Chunk<T>::Chunk()
{
	this->data = NULL;
	//this->first = this->available = 0;
}

template <typename T> Data::Chunk<T>::~Chunk()
{

}

template <typename T> void Data::Chunk<T>::initialize(std::size_t blockSize, Type blocks)
{
	this->data = new Type[blockSize*blocks];
	this->first = 0;
	this->available = blocks;

	for (Type i = 0, *p = this->data; i != blocks; p += blockSize)
	{
		*p = ++i;
	}

#if DEBUG_POOL_LEAK
	this->traces.assign(static_cast<size_t>(blocks), "");
#endif //#if DEBUG_POOL_LEAK

#if DEBUG_POOL_FREE
	this->dirty.assign(static_cast<size_t>(blocks), false);
#endif //#if DEBUG_POOL_FREE
}

template <typename T> void *Data::Chunk<T>::allocate(std::size_t blockSize)
{
	if (!this->available) return NULL;

	assert(this->data != NULL);

	Type *ptr = this->data + (this->first*blockSize);

#if DEBUG_POOL_LEAK
	std::string str = this->trace();
#endif //#if DEBUG_POOL_LEAK

#if DEBUG_POOL_FREE
	if (this->dirty[this->first])
	{
		std::cout << "ERROR: double allocation in memory pool for type " << typeid(T).name() << " in position " << static_cast<void *>(ptr) << std::endl;
#if DEBUG_POOL_LEAK
		std::cout << "previous allocation: " << this->traces[this->first] << std::endl;
		std::cout << "current allocation: " << str << std::endl;
#endif //#if DEBUG_POOL_LEAK
	}

	this->dirty[this->first] = true;
#endif //#if DEBUG_POOL_FREE

#if DEBUG_POOL_LEAK
	this->traces[this->first] = str;
#endif //#if DEBUG_POOL_LEAK

	// Update this->first to point to the next block
	this->first = *ptr;

	--this->available;

	return ptr;
}

template <typename T> void Data::Chunk<T>::deallocate(void *p, std::size_t blockSize)
{
	assert(p >= this->data);

	Type *ptr = static_cast<Type *>(p);

	// Alignment check
	assert(((ptr - this->data) % blockSize) == 0);

	*ptr = this->first;

	this->first = static_cast<Type>((ptr - this->data)/blockSize);

	// Truncation check
	assert(this->first == ((ptr - this->data) / blockSize));

#if DEBUG_POOL_FREE
	if (!this->dirty[this->first])
	{
		std::cout << "ERROR: double free in memory pool for type " << typeid(T).name() << " in position " << static_cast<void *>(ptr) << std::endl;
#if DEBUG_POOL_LEAK
		std::cout << "current free: " << this->trace() << std::endl;
#endif //#if DEBUG_POOL_LEAK
	}

	this->dirty[this->first] = false;
#endif //#if DEBUG_POOL_FREE

#if DEBUG_POOL_LEAK
	this->traces[this->first].clear();
#endif //#if DEBUG_POOL_LEAK

	++this->available;
}

#if DEBUG_POOL_LEAK
template <typename T> std::string Data::Chunk<T>::trace() const
{
	std::stringstream str;

#if USE_OPENMP
	str << "on thread " << omp_get_thread_num() << std::endl;
#endif //#if USE_OPENMP
	str << Data::trace();

	return str.str();
}
#endif //#if DEBUG_POOL_LEAK

//#endif //#if USE_POOL

#endif //#ifndef _DATA_CHUNK_H_

#include "Data/Chunk.h"

//#if USE_POOL

/*#include <cassert>

#if ((DEBUG_LEAK_POOL) && (USE_OPENMP))
#include <omp.h>
#endif //#if ((DEBUG_LEAK_POOL) && (USE_OPENMP))

Data::Chunk::Chunk()
{
	this->data = NULL;
	//this->first = this->available = 0;
}

Data::Chunk::~Chunk()
{

}

void Data::Chunk::initialize(std::size_t blockSize, unsigned char blocks)
{
	this->data = new unsigned char[blockSize*blocks];
	this->first = 0;
	this->available = blocks;

	for (unsigned char i = 0, *p = this->data;
		 i != blocks; p += blockSize)
	{
		*p = ++i;
	}

#if DEBUG_LEAK_POOL
	this->trace = new std::string[blocks];
#endif //#if DEBUG_LEAK_POOL
}

void *Data::Chunk::allocate(std::size_t blockSize)
{
	if (!this->available) return NULL;

	assert(this->data != NULL);

	unsigned char *ptr = this->data + (this->first*blockSize);

#if DEBUG_LEAK_POOL
	std::stringstream str;

#if USE_OPENMP
	str << "on thread " << omp_get_thread_num() << std::endl;
#endif //#if USE_OPENMP
	str << Data::trace();

	this->trace[this->first] = str.str();
#endif //#if DEBUG_LEAK_POOL

	// Update this->first to point to the next block
	this->first = *ptr;

	--this->available;

	return ptr;
}

void Data::Chunk::deallocate(void *p, std::size_t blockSize)
{
	assert(p >= this->data);

	unsigned char *ptr = static_cast<unsigned char*>(p);

	// Alignment check
	assert(((ptr - this->data) % blockSize) == 0);

	*ptr = this->first;

	this->first = static_cast<unsigned char>((ptr - this->data)/blockSize);

	// Truncation check
	assert(this->first == ((ptr - this->data) / blockSize));

#if DEBUG_LEAK_POOL
	this->trace[this->first].clear();
#endif //#if DEBUG_LEAK_POOL

	++this->available;
}*/

//#endif //#if USE_POOL

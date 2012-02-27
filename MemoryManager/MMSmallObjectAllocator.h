#ifndef MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD

#include "MMSmallObjectAlloc.h"

#include "MMSingletonHolder.h"

/*
#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE		4096
#endif
*/ 

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE	256
#endif

namespace MM
{
	class SmallObjectAllocator
	{
	public:
		static void* Allocate(size_t size)
		{
			return SingletonHolder<SmallObjectAllocatorType>::Instance().Allocate(size);
		}

		static void Deallocate(void * p)
		{
			return SingletonHolder<SmallObjectAllocatorType>::Instance().Deallocate(p);
		}

		static size_t GetChunkSize()
		{
			return DEFAULT_CHUNK_SIZE;
		}

		static size_t GetMaxSmallObjectSize()
		{
			return MAX_SMALL_OBJECT_SIZE;
		}

	private:

		struct SmallObjectAllocatorType : public SmallObjectAlloc
		{
			SmallObjectAllocatorType()
				: SmallObjectAlloc(DEFAULT_CHUNK_SIZE, MAX_SMALL_OBJECT_SIZE) { }
		};
	}; 
}

#endif // MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD
#ifndef MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

#include "MMSmallObjectAlloc.h"
#include "MMSingletonHolder.h"

// DEFAULT_CHUNK_SIZE is alredy defined (and used) in MMFixedAlloc.h

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
	class SmallObjectAllocator : public AllocatorInterface
	{
	public:
		static void* operator new(size_t size)
		{
			return SingletonHolder<SmallObjectAllocatorType>::Instance().Allocate(size);
		}

		static void operator delete(void * p)
		{
			return SingletonHolder<SmallObjectAllocatorType>::Instance().Deallocate(p);
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
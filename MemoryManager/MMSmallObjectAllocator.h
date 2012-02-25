#ifndef MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

#include "MMSmallObjectAlloc.h"
#include "MMSingletonHolder.h"

#define DEFAULT_CHUNK_SIZE		4096
#define MAX_SMALL_OBJECT_SIZE	256

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
			return SingletonHolder<SmallObjectAllocatorType>::Instance().Deallocate(p,size);
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
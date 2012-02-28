#ifndef MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "MMSingleObjectAlloc.h"
#include "MMSingletonHolder.h"

#include "MMLockPoliclies.h"

namespace MM
{
	class SingleObjectAllocator
	{
	public:

		static void* Allocate(size_t size)
		{
			return SingletonHolder<SingleChunkAllocType>::Instance().Allocate(size);
		}

		static void Deallocate(void * p)
		{
			return SingletonHolder<SingleChunkAllocType>::Instance().Deallocate(p);
		}

	private:

		struct SingleChunkAllocType : public SingleChunkAlloc<NoLock> { };
	}; 
}

#endif // MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD
#ifndef MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "MMSingleObjectAlloc.h"
#include "MMSingletonHolder.h"

#include "MMPreprocDirectives.h"
#include "MMLockPoliclies.h"

namespace MM
{
	class SingleObjectAllocator
	{
	private:

#ifndef MM_ENABLE_MULTITHREAD
		typedef NoLock LockPolicy;
#else
		struct SingleObjectAllocLockTypeTag { };

		typedef MutexLock<SingleObjectAllocLockTypeTag> LockPolicy;
#endif

	public:

		static void* Allocate(size_t size, const char* category, const char* file, size_t line, const char* func)
		{
			void* p = SingletonHolder<SingleObjectAllocType, LockPolicy>::Instance().Allocate(size);
#if MM_ALLOCATION_TRACKER
			AllocationTracker::GetInstance().RecordAllocation(p, size, category, file, line, func);
#endif 
			return p;
		}

		static void Deallocate(void * p)
		{
#if MM_ALLOCATION_TRACKER
			AllocationTracker::GetInstance().RecordDellocation(p);
#endif 
			SingletonHolder<SingleObjectAllocType, LockPolicy>::Instance().Deallocate(p);
		}

	private:

		// Private constructor
		SingleObjectAllocator() { }

		struct SingleObjectAllocType 
			: public SingleObjectAlloc<LockPolicy> { };
	}; 
}

#endif // MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD
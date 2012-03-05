#ifndef MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "MMSingleObjectAlloc.h"

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

		struct SingleObjectAllocType 
			: public SingleObjectAlloc<LockPolicy> { } mAlloc;

	public:

		static SingleObjectAllocator& GetInstance()
		{
			// TODO: You need a lock?

			static SingleObjectAllocator alloc;
			return alloc;
		}

		void* Allocate(size_t size, const char* category, const char* file, size_t line, const char* func)
		{
			void* p = mAlloc.Allocate(size);
			AllocationTracker::GetInstance().RecordAllocation(p, size, category, file, line, func);
			return p;
		}

		void Deallocate(void * p)
		{
			AllocationTracker::GetInstance().RecordDellocation(p);
			mAlloc.Deallocate(p);
		}

	private:

		// Private constructor
		SingleObjectAllocator() { }
	}; 
}

#endif // MMSINGLEOBJECTALLOCATOR_H_INCLUDE_GUARD
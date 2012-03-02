#ifndef MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD

#include "MMSmallObjectAlloc.h"
#include "MMSingletonHolder.h"

#include "MMPreprocDirectives.h"
#include "MMLockPoliclies.h"

#include "MMAllocationTracker.h"

namespace MM
{
	class SmallObjectAllocator
	{
	private:

#ifndef MM_ENABLE_MULTITHREAD
		typedef NoLock LockPolicy;
#else
		struct SmallObjectAllocLockTypeTag { };

		typedef MutexLock<SmallObjectAllocLockTypeTag> LockPolicy;
#endif

	public:

		static void* Allocate(size_t size, const char* category, const char* file, size_t line, const char* func)
		{
			void* p = SingletonHolder<SmallObjectAllocatorType, LockPolicy>::Instance().Allocate(size);
			AllocationTracker::GetInstance().RecordAllocation(p, size, category, file, line, func);
			return p;
		}

		static void Deallocate(void * p)
		{
			AllocationTracker::GetInstance().RecordDellocation(p);
			return SingletonHolder<SmallObjectAllocatorType, LockPolicy>::Instance().Deallocate(p);
		}

		static size_t GetChunkSize()
		{
			return FSA_DEFAULT_CHUNK_SIZE;
		}

		static size_t GetMaxSmallObjectSize()
		{
			return SOA_MAX_SMALL_OBJECT_SIZE;
		}

	private:

		// Private constructor
		SmallObjectAllocator() { }

		struct SmallObjectAllocatorType : public SmallObjectAlloc<LockPolicy>
		{
			SmallObjectAllocatorType()
				: SmallObjectAlloc<LockPolicy>(SOA_MAX_SMALL_OBJECT_SIZE) { }
		};
	}; 
}

#endif // MMSMALLOBJECTALLCATOR_H_INCLUDE_GUARD
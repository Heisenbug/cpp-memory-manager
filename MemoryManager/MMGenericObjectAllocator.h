#ifndef MMGENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
#define MMGENERICOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "MMGenericObjectAlloc.h"
#include "MMSingletonHolder.h"

#include "MMPreprocDirectives.h"
#include "MMLockPoliclies.h"

namespace MM
{	
	class GenericObjectAllocator
	{
	private:

#ifndef MM_ENABLE_MULTITHREAD
		typedef NoLock LockPolicy;
#else
		struct GenericObjectAllocLockTypeTag { };

		typedef MutexLock<GenericObjectAllocLockTypeTag> LockPolicy;
#endif

	public:

		static void* Allocate(size_t size_t size, const char* category, const char* file, size_t line, const char* func)
		{
			// TODO: Record this allocation
			return SingletonHolder<GenericObjectAllocType, LockPolicy>::Instance().Allocate(size);
		}

		static void Deallocate(void * p)
		{
			// TODO: Record this deallocation
			return SingletonHolder<GenericObjectAllocType, LockPolicy>::Instance().Deallocate(p);
		}

	private:

		// Private constructor
		GenericObjectAllocator() { }

		struct GenericObjectAllocType 
			: public GenericObjectAlloc<LockPolicy> { };
	};
}

#endif // MMGENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
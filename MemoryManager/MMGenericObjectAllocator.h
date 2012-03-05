#ifndef MMGENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
#define MMGENERICOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "MMGenericObjectAlloc.h"

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

		struct GenericObjectAllocType 
			: public GenericObjectAlloc<LockPolicy> { } mAlloc;

	public:

		static GenericObjectAllocator& GetInstance()
		{
			// TODO: You need a lock?

			static GenericObjectAllocator alloc;
			return alloc;
		}

		void* Allocate(size_t size, const char* category, const char* file, size_t line, const char* func)
		{
			// TODO: Record this allocation
			return mAlloc.Allocate(size);
		}

		void Deallocate(void * p)
		{
			// TODO: Record this deallocation
			mAlloc.Deallocate(p);
		}

	private:

		// Private constructor
		GenericObjectAllocator() { }
	};
}

#endif // MMGENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
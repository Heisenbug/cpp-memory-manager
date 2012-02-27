#ifndef MEMORY_MANAGER_H_INCLUDE_GUARD
#define MEMORY_MANAGER_H_INCLUDE_GUARD

#include "MMSmallObjectAllocator.h"
#include "MMGenericObjectAllocator.h"
#include "MMAllocationTable.h"
#include "MMAllocatorInterface.h"

template<typename MemoryCategory>
void* MM_MALLOC(size_t size)
{
	return ::MM::AllocationPolicy<MemoryCategory>::AllocateBytes(size);
}

template<typename MemoryCategory>
void MM_FREE(void* p)
{
	::MM::AllocationPolicy<MemoryCategory>::DeallocateBytes(p);
}

namespace MM
{
	// Memory allocation categories
	typedef struct MEMCATEGORY_GENERAL { };

	// General AllocationPolicy interface
	template <typename MemoryCategory> class AllocationPolicy 
	{
	public:

		static inline void* AllocateBytes(size_t) { }
		static inline void DeallocateBytes(void*) { }
	};
	
	// AllocationPolicy for MEMCATEGORY_GENERAL
	template<> class AllocationPolicy<MEMCATEGORY_GENERAL>
	{
	public:

		static inline void* AllocateBytes(size_t size)//, const char* file = 0, int line = 0, const char* func = 0) 
		{
			// Default behavior
			// Check the size; if it's equal or lower than MAX_SMALL_OBJECT_SIZE, SmallObjectAllocator is called
			size_t maxSmallObjectSize = SmallObjectAllocator::GetMaxSmallObjectSize();

			if (size <= maxSmallObjectSize)
			{
				return SmallObjectAllocator::Allocate(size);
			}

			return GenericObjectAllocator::Allocate(size);
		}

		static inline void DeallocateBytes(void* p) 
		{
			AllocatorInterface* a = AllocationTable::FindAllocatorFor(p);
			a->Deallocate(p);
		}
	};
}

#endif // MEMORY_MANAGER_H_INCLUDE_GUARD
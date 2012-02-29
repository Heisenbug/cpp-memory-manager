#ifndef MEMORY_MANAGER_H_INCLUDE_GUARD
#define MEMORY_MANAGER_H_INCLUDE_GUARD

#include "MMSmallObjectAllocator.h"
#include "MMSingleObjectAllocator.h"
#include "MMAllocationTable.h"
#include "MMAllocatorInterface.h"

template<typename MemoryCategory>
void* MM_MALLOC(size_t size)
{
	//::MM::AllocationTable::Dump();
	return ::MM::AllocationPolicy<MemoryCategory>::AllocateBytes(size);
}

void MM_FREE(void* p)
{
	//::MM::AllocationTable::Dump();
	::MM::AllocatorInterface* a = ::MM::AllocationTable::FindAllocatorFor(p);
	a->Deallocate(p);
}

namespace MM
{
	// Memory allocation categories
	typedef struct MEMCATEGORY_GENERAL { };

	// General AllocationPolicy interface
	template <typename MemoryCategory> class AllocationPolicy 
	{
	public:

		// TODO: We can add a memory tracking function...JUST IN CASE :)
		static inline void* AllocateBytes(size_t) { }
	};
	
	// AllocationPolicy for MEMCATEGORY_GENERAL
	template<> class AllocationPolicy<MEMCATEGORY_GENERAL>
	{
	public:

		static inline void* AllocateBytes(size_t size)
		{
			// Default behavior
			// Check the size; if it's equal or lower than MAX_SMALL_OBJECT_SIZE, SmallObjectAllocator is called
			size_t maxSmallObjectSize = SmallObjectAllocator::GetMaxSmallObjectSize();

			if (size <= maxSmallObjectSize)
			{
				return SmallObjectAllocator::Allocate(size);
			}

			return SingleObjectAllocator::Allocate(size);
		}
	};
}

#endif // MEMORY_MANAGER_H_INCLUDE_GUARD
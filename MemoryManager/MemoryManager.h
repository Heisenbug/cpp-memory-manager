#ifndef MEMORY_MANAGER_H_INCLUDE_GUARD
#define MEMORY_MANAGER_H_INCLUDE_GUARD

template<typename MemoryCategory>
void* MM_NEW(size_t size)
{
	::MM::AllocationPolicy<MemoryCategory>::allocateBytes(size);
}

//template<typename MemoryCategory>
//void* MM_NEW_A(size_t size)
//{
//
//}
//
//template<typename MemoryCategory>
//void* MM_DELETE(void* p)
//{
//
//}
//
//template<typename MemoryCategory>
//void* MM_DELETE_A(void* p)
//{
//
//}

namespace MM
{
	// Memory allocation categories
	typedef struct MEMCATEGORY_GENERAL { };

	// General AllocationPolicy interface
	template <typename MemoryCategory> class AllocationPolicy 
	{
		static inline void* allocateBytes(size_t) { }
		static inline void deallocateBytes(void*) { }
	};
	
	// AllocationPolicy for MEMCATEGORY_GENERAL
	template<> class AllocationPolicy<MEMCATEGORY_GENERAL>
	{
		static inline void* allocateBytes(size_t count)//, const char* file = 0, int line = 0, const char* func = 0) 
		{
			// Default behavior
			// Check the size; if it's equal or lower than MAX_SMALL_OBJECT_SIZE, SmallObjectAllocator is called
			::MM::SmallObjectAlloc::getMax
		}

		static inline void deallocateBytes(void* ptr, size_t size = 0) 
		{

		}
	};
}

#endif // MEMORY_MANAGER_H_INCLUDE_GUARD
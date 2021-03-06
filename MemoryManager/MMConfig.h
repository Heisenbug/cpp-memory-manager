#ifndef MMCONFIG_H_INCLUDE_GUARD
#define MMCONFIG_H_INCLUDE_GUARD

#include "MMAllocationTable.h"
#include "MMAllocatorInterface.h"

#include "MMAllocatedObject.h"

#include "MMAllocationTracker.h"

#pragma region AllocPolicies includes

#include "MMGeneralAllocPolicy.h"
#include "MMGeneralAlignedAllocPolicy.h"

// EXTEND IT! Add your own custom policies include files here 

#pragma endregion

namespace MM
{
	#pragma region Categories

	// Memory allocation categories
	struct MEMCATEGORY_GENERAL { };

	// EXTEND IT! Add more categories here

	#pragma endregion

	#pragma region Allocation policies

	// AllocationPolicy interface
	template <typename MemoryCategory> class AllocationPolicy 
	{
		static inline void* AllocateBytes(size_t size, const char* category = 0, const char* file = 0, 
			size_t line = 0, const char* func = 0)	{ return 0; }

		static inline void DeallocateBytes(void* p) { }
	};

	// AlignedAllocationPolicy interface
	template <typename MemoryCategory, size_t Alignment = 0> class AlignedAllocationPolicy 
	{
		static inline void* AllocateAlignedBytes(size_t size,size_t align, const char* category = 0, const char* file = 0, 
			size_t line = 0, const char* func = 0) { }

		static inline void DeallocateAlignedBytes(void* p) { }
	};

	template<> class AllocationPolicy<MEMCATEGORY_GENERAL> 
		: public GeneralAllocPolicy { };
	
	template<size_t Alignment> class AlignedAllocationPolicy<MEMCATEGORY_GENERAL, Alignment> 
		: public GeneralAlignedAllocPolicy<Alignment> { };

	// EXTEND IT! Here you can add other specializations

	// Shortcuts
	typedef AllocationPolicy<MEMCATEGORY_GENERAL> GeneralAllocationPolicy;

	#pragma endregion

	#pragma region AllocatedObject types

	// Here we can define all the base classes for each allocation
	typedef AllocatedObject<GeneralAllocationPolicy> GeneralAllocatedObject;

	// EXTEND IT! Define all the AllocatedObject you want, and derive your classes from them

	#pragma endregion

	#pragma region Utilities

	void GenericDeallocation(void* p)
	{
		::MM::AllocatorInterface* a = ::MM::AllocationTable::FindAllocatorFor(p);
		assert(a);
		AllocationTracker::GetInstance().RecordDellocation(p);
		a->Deallocate(p);
	}

	#pragma endregion
}

#endif // MMCONFIG_H_INCLUDE_GUARD
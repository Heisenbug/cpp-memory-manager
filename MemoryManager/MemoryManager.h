#ifndef MEMORY_MANAGER_H_INCLUDE_GUARD
#define MEMORY_MANAGER_H_INCLUDE_GUARD

#include "MMConfig.h"

#include "MMSmartPointers.h"

#include <typeinfo>

namespace MM
{
	// Utility function for constructing an array of objects with placement new,
	template<typename T>
	T* ConstructArray(T* p, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			new ((void*)(p + i)) T();
		}
		return p;
	}
}

void InitMemoryManager()
{
	::MM::AllocationTable::Init();
}

#if MM_DEBUG_MODE

// Allocate a block of raw memory, and indicate the category of usage
#	define MM_MALLOC(size, category) ::MM::AllocationPolicy<category>::AllocateBytes(size, typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)
// Allocate a block of memory for a primitive type, and indicate the category of usage
#	define MM_ALLOC_T(T, count, category) static_cast<T*>(::MM::AllocationPolicy<category>::AllocateBytes((sizeof(T)*(count)), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__))
// Free the memory allocated with MM_MALLOC or MM_ALLOC_T
#	define MM_FREE(p) ::MM::GenericDeallocation(p);
#	define MM_FREE_CAT(p, category) ::MM::AllocationPolicy<category>::DeallocateBytes((void*)p)
// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#	define MM_NEW_T(T, category) new (::MM::AllocationPolicy<category>::AllocateBytes(sizeof(T), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)) T
// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define MM_NEW_ARRAY_T(T, count, category) ::MM::ConstructArray(static_cast<T*>(::MM::AllocationPolicy<category>::AllocateBytes((sizeof(T)*(count)), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)), count) 
// Free the memory allocated with MM_NEW_T
#	define MM_DELETE_T(p, T) if (p) { (p)(p)->~T(); ::MM::GenericDeallocation(p); } 
// Free the memory allocated with MM_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_T_CAT(p, T, category) if (p) { (p)->~T(); ::MM::AllocationPolicy<category>::DeallocateBytes((void*)ptr); }
// Free the memory allocated with MM_NEW_ARRAY_T
#	define MM_DELETE_ARRAY_T(p, T, count) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::GenericDeallocation(p); }
// Free the memory allocated with MM_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_ARRAY_T_CAT(p, T, count, category) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::AllocationPolicy<category>::DeallocateBytes((void*)p); }

// Aligned allocation
/// Allocate a block of raw memory aligned to SIMD boundaries, and indicate the category of usage
#	define MM_MALLOC_SIMD(size, category) ::MM::AlignedAllocationPolicy<category>::AllocateBytes(size, typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)
/// Allocate a block of raw memory aligned to user defined boundaries, and indicate the category of usage
#	define MM_MALLOC_ALIGN(size, category, align) ::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(size, typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)
/// Allocate a block of memory for a primitive type aligned to SIMD boundaries, and indicate the category of usage
#	define MM_ALLOC_T_SIMD(T, count, category) static_cast<T*>(::MM::AlignedAllocationPolicy<category>::AllocateBytes(sizeof(T)*(count), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__))
/// Allocate a block of memory for a primitive type aligned to user defined boundaries, and indicate the category of usage
#	define MM_ALLOC_T_ALIGN(T, count, category, align) static_cast<T*>(::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(sizeof(T)*(count), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__))
/// Free the memory allocated with either MM_MALLOC_SIMD or MM_ALLOC_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define MM_FREE_SIMD(p, category) ::MM::AlignedAllocationPolicy<category>::DeallocateBytes(p)
/// Free the memory allocated with either MM_MALLOC_ALIGN or OGRE_MM_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define MM_FREE_ALIGN(p, category, align) ::MM::AlignedAllocationPolicy<category, align>::DeallocateBytes(p)

/// Allocate space for one primitive type, external type or non-virtual type aligned to SIMD boundaries
#	define MM_NEW_T_SIMD(T, category) new (::MM::AlignedAllocationPolicy<category>::AllocateBytes(sizeof(T), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)) T
/// Allocate a block of memory for 'count' primitive types aligned to SIMD boundaries - do not use for classes that inherit from AllocatedObject
#	define MM_NEW_ARRAY_T_SIMD(T, count, category) ::MM::ConstructArray(static_cast<T*>(::MM::AlignedAllocationPolicy<category>::AllocateBytes(sizeof(T)*(count), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)), count) 
/// Free the memory allocated with MM_NEW_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_T_SIMD(p, T, category) if (p) { (p)->~T(); ::MM::AlignedAllocationPolicy<category>::DeallocateBytes(p); }
/// Free the memory allocated with MM_NEW_ARRAY_T_SIMD. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define MM_DELETE_ARRAY_T_SIMD(p, T, count, category) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::AlignedAllocationPolicy<category>::DeallocateBytes(p); }
/// Allocate space for one primitive type, external type or non-virtual type aligned to user defined boundaries
#	define MM_NEW_T_ALIGN(T, category, align) new (::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(sizeof(T), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)) T
/// Allocate a block of memory for 'count' primitive types aligned to user defined boundaries - do not use for classes that inherit from AllocatedObject
#	define MM_NEW_ARRAY_T_ALIGN(T, count, category, align) ::MM::ConstructArray(static_cast<T*>(::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(sizeof(T)*(count), typeid(category).name(), __FILE__, __LINE__, __FUNCTION__)), count) 
/// Free the memory allocated with MM_NEW_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_T_ALIGN(p, T, category, align) if (p) { (p)->~T(); ::MM::AlignedAllocationPolicy<category, align>::DeallocateBytes(p); }
/// Free the memory allocated with MM_NEW_ARRAY_T_ALIGN. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define MM_DELETE_ARRAY_T_ALIGN(p, T, count, category, align) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::AlignedAllocationPolicy<category, align>::DeallocateBytes(p); }

// new / delete for classes deriving from AllocatedObject
// Also hooks up the file/line/function params
// Can only be used with classes that derive from AllocatedObject since customised new/delete needed
#	define MM_NEW new (__FILE__, __LINE__, __FUNCTION__)
#	define MM_DELETE delete

#else

// Allocate a block of raw memory, and indicate the category of usage
#	define MM_MALLOC(size, category) ::MM::AllocationPolicy<category>::AllocateBytes(size)
// Allocate a block of memory for a primitive type, and indicate the category of usage
#	define MM_ALLOC_T(T, count, category) static_cast<T*>(::MM::AllocationPolicy<category>::AllocateBytes(sizeof(T)*(count)))
// Free the memory allocated with MM_MALLOC or MM_ALLOC_T
#	define MM_FREE(p) ::MM::GenericDeallocation(p);
#	define MM_FREE_CAT(p, category) ::MM::AllocationPolicy<category>::DeallocateBytes((void*)p)
// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#	define MM_NEW_T(T, category) new (::MM::AllocationPolicy<category>::AllocateBytes(sizeof(T)) T
// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define MM_NEW_ARRAY_T(T, count, category) ::MM::ConstructArray(static_cast<T*>(::MM::AllocationPolicy<category>::AllocateBytes(sizeof(T)*(count))), count) 
// Free the memory allocated with MM_NEW_T
#	define MM_DELETE_T(p, T) if (p) { (p)(p)->~T(); ::MM::GenericDeallocation(p); } 
// Free the memory allocated with MM_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_T_CAT(p, T, category) if (p) { (p)->~T(); ::MM::AllocationPolicy<category>::DeallocateBytes((void*)ptr); }
// Free the memory allocated with MM_NEW_ARRAY_T
#	define MM_DELETE_ARRAY_T(p, T, count) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::GenericDeallocation(p); }
// Free the memory allocated with MM_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_ARRAY_T_CAT(p, T, count, category) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::AllocationPolicy<category>::DeallocateBytes((void*)p); }

// Aligned allocation
/// Allocate a block of raw memory aligned to SIMD boundaries, and indicate the category of usage
#	define MM_MALLOC_SIMD(size, category) ::MM::AlignedAllocationPolicy<category>::AllocateBytes(size)
/// Allocate a block of raw memory aligned to user defined boundaries, and indicate the category of usage
#	define MM_MALLOC_ALIGN(size, category, align) ::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(size)
/// Allocate a block of memory for a primitive type aligned to SIMD boundaries, and indicate the category of usage
#	define MM_ALLOC_T_SIMD(T, count, category) static_cast<T*>(::MM::AlignedAllocationPolicy<category>::AllocateBytes(sizeof(T)*(count)))
/// Allocate a block of memory for a primitive type aligned to user defined boundaries, and indicate the category of usage
#	define MM_ALLOC_T_ALIGN(T, count, category, align) static_cast<T*>(::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(sizeof(T)*(count)))
/// Free the memory allocated with either MM_MALLOC_SIMD or MM_ALLOC_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define MM_FREE_SIMD(p, category) ::MM::AlignedAllocationPolicy<category>::DeallocateBytes(p)
/// Free the memory allocated with either MM_MALLOC_ALIGN or OGRE_MM_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define MM_FREE_ALIGN(p, category, align) ::MM::AlignedAllocationPolicy<category, align>::DeallocateBytes(p)

/// Allocate space for one primitive type, external type or non-virtual type aligned to SIMD boundaries
#	define MM_NEW_T_SIMD(T, category) new (::MM::AlignedAllocationPolicy<category>::AllocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types aligned to SIMD boundaries - do not use for classes that inherit from AllocatedObject
#	define MM_NEW_ARRAY_T_SIMD(T, count, category) ::MM::ConstructArray(static_cast<T*>(::MM::AlignedAllocationPolicy<category>::AllocateBytes(sizeof(T)*(count))), count) 
/// Free the memory allocated with MM_NEW_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_T_SIMD(p, T, category) if (p) { (p)->~T(); ::MM::AlignedAllocationPolicy<category>::DeallocateBytes(p); }
/// Free the memory allocated with MM_NEW_ARRAY_T_SIMD. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define MM_DELETE_ARRAY_T_SIMD(p, T, count, category) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::AlignedAllocationPolicy<category>::DeallocateBytes(p); }
/// Allocate space for one primitive type, external type or non-virtual type aligned to user defined boundaries
#	define MM_NEW_T_ALIGN(T, category, align) new (::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types aligned to user defined boundaries - do not use for classes that inherit from AllocatedObject
#	define MM_NEW_ARRAY_T_ALIGN(T, count, category, align) ::MM::ConstructArray(static_cast<T*>(::MM::AlignedAllocationPolicy<category, align>::AllocateBytes(sizeof(T)*(count))), count) 
/// Free the memory allocated with MM_NEW_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define MM_DELETE_T_ALIGN(p, T, category, align) if (p) { (p)->~T(); ::MM::AlignedAllocationPolicy<category, align>::DeallocateBytes(p); }
/// Free the memory allocated with MM_NEW_ARRAY_T_ALIGN. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define MM_DELETE_ARRAY_T_ALIGN(p, T, count, category, align) if (p) { for (size_t i = 0; i < count; ++i) { (p)[i].~T(); } ::MM::AlignedAllocationPolicy<category, align>::DeallocateBytes(p); }

// new / delete for classes deriving from AllocatedObject
#	define MM_NEW new 
#	define MM_DELETE delete

#endif // MM_DEBUG_MODE

#endif // MEMORY_MANAGER_H_INCLUDE_GUARD
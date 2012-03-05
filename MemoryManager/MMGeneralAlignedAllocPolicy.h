#pragma once

#include "MMSmallObjectAllocator.h"
#include "MMSingleObjectAllocator.h"
//#include "MMGenericObjectAllocator.h"

namespace MM
{
	// Forward declaration
	void GenericDeallocation(void* p);

	class GeneralAlignedAllocPolicy
	{
	public:

		static inline void* AllocateBytes(size_t size, size_t align, const char* category = 0, const char* file = 0, 
			size_t line = 0, const char* func = 0)
		{
			assert(align >=0);
			
			void *ptr;
			void *realMemoryPtr;
			size_t realSize = size + align - 1 + sizeof(void*);
			
			// Default behavior
			// Check the size; if it's equal or lower than MAX_SMALL_OBJECT_SIZE, SmallObjectAllocator is called
			size_t maxSmallObjectSize = SmallObjectAllocator::GetMaxSmallObjectSize();

			if (realSize <= maxSmallObjectSize)
			{
				realMemoryPtr = SmallObjectAllocator::Allocate(realSize, category, file, line, func);
			}else{

				realMemoryPtr =  SingleObjectAllocator::Allocate(size, category, file, line, func);
			}
			ptr = (void*) (((unsigned int)realMemoryPtr + sizeof(void*) + align -1) & ~(align-1));

			return ptr;
		}

		static inline void DeallocateBytes(void* p)
		{
			void *ptr = *((void**)((unsigned int)p - sizeof(void*)));

			GenericDeallocation(ptr);
		}

	private:

		// Private constructor
		GeneralAlignedAllocPolicy() { }
	};
}
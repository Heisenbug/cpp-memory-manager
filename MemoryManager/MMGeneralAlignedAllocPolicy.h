#ifndef MMGENERALALIGNEDALLOCPOLICY_H_INCLUDE_GUARD
#define MMGENERALALIGNEDALLOCPOLICY_H_INCLUDE_GUARD

#include "MMSmallObjectAllocator.h"
#include "MMSingleObjectAllocator.h"
//#include "MMGenericObjectAllocator.h"

#include "MMPreprocDirectives.h"

namespace MM
{
	// Forward declaration
	void GenericDeallocation(void* p);

	template<size_t Alignment = 0>
	class GeneralAlignedAllocPolicy
	{
	public:

		// Compile-time check for the Alignment value
		typedef int IsValidAlignment
			[Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];

		static inline void* AllocateAlignedBytes(size_t size, const char* category = 0, const char* file = 0, 
			size_t line = 0, const char* func = 0)
		{	
			size_t blockSize = Alignment ? size + Alignment - 1 + sizeof(void*) 
				: size + MM_SIMD_ALIGNMENT - 1 + sizeof(void*);

			void* ptr = AllocateBytes(size, category, file, line, func);

			void* p = (void*)(((unsigned int)ptr + sizeof(void*) + Alignment - 1) &~ (Alignment - 1));

			*((void**)((unsigned int)p - sizeof(void*))) = ptr;

			return p;
		}

		static inline void DeallocateAlignedBytes(void* p)
		{
			void* ptr = *((void**)((unsigned int)p - sizeof(void*)));

			DeallocateBytes(ptr);
		}

	private:

		static inline void* AllocateBytes(size_t size, const char* category = 0, const char* file = 0, 
			size_t line = 0, const char* func = 0)
		{
			// Default behavior
			// Check the size; if it's equal or lower than MAX_SMALL_OBJECT_SIZE, SmallObjectAllocator is called
			size_t maxSmallObjectSize = SmallObjectAllocator::GetMaxSmallObjectSize();

			if (size <= maxSmallObjectSize)
			{
				return SmallObjectAllocator::Allocate(size, category, file, line, func);
			}
			
			return SingleObjectAllocator::Allocate(size, category, file, line, func);
		}

		static inline void DeallocateBytes(void* p)
		{
			GenericDeallocation(p);
		}

		// Private constructor
		GeneralAlignedAllocPolicy() { }
	};
}

#endif // MMGENERALALIGNEDALLOCPOLICY_H_INCLUDE_GUARD

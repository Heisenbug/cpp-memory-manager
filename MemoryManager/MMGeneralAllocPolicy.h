#ifndef MMGENERALALLOCPOLICY_H_INCLUDE_GUARD
#define MMGENERALALLOCPOLICY_H_INCLUDE_GUARD

#include "MMSmallObjectAllocator.h"
#include "MMSingleObjectAllocator.h"
//#include "MMGenericObjectAllocator.h"

namespace MM
{
	// Forward declaration
	void GenericDeallocation(void* p);

	class GeneralAllocPolicy
	{
	public:

		static inline void* AllocateBytes(size_t size, const char* category = 0, const char* file = 0, 
			size_t line = 0, const char* func = 0)
		{
			// Default behavior
			// Check the size; if it's equal or lower than MAX_SMALL_OBJECT_SIZE, SmallObjectAllocator is called
			size_t maxSmallObjectSize = SmallObjectAllocator::GetMaxSmallObjectSize();

			if (size <= maxSmallObjectSize)
			{
				return SmallObjectAllocator::GetInstance().Allocate(size, category, file, line, func);
			}

			return SingleObjectAllocator::GetInstance().Allocate(size, category, file, line, func);
		}

		static inline void DeallocateBytes(void* p)
		{
			GenericDeallocation(p);
		}

	private:

		// Private constructor
		GeneralAllocPolicy() { }
	};
}

#endif // MMGENERALALLOCPOLICY_H_INCLUDE_GUARD
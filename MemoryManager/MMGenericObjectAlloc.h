#ifndef MMGENERICOBJECT_ALLOC_H_INCLUDE_GUARD
#define MMGENERICOBJECT_ALLOC_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

// TODO: Use this red-black tree for implement a new, efficient allocator
// This allocator must handle objects of any size. It can be used in tandem 
// with the SmallObjectAllocator
#include "MMRBTree.h"

namespace MM
{
	template<typename LockPolicy>
	class GenericObjectAlloc : public AllocatorInterface
	{
	public:

		GenericObjectAlloc() 
		{
			// ...
		}

		virtual ~GenericObjectAlloc()
		{
			// ...
		}

		// ...

		void* Allocate(size_t size) 
		{
			// ...
		}

		void Deallocate(void* p) 
		{
			// ...
		}

		// ...

	private:

		// ...
	};
}

#endif // MMGENERICOBJECT_ALLOC_H_INCLUDE_GUARD
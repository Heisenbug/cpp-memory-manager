#ifndef MMSMALLOBJECTALLOC_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLOC_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"
#include "MMFixedSizeAlloc.h"

#include <vector>

namespace MM
{
	template<typename LockPolicy>
	class SmallObjectAlloc : public AllocatorInterface
	{
	public:

		SmallObjectAlloc(size_t maxObjectSize);

		void* Allocate(size_t size);
		void Deallocate(void* p);

		virtual ~SmallObjectAlloc();

	private:

		FixedSizeAlloc*	FindFixedSizeAlloc(void* p);

		SmallObjectAlloc(const SmallObjectAlloc&);
		SmallObjectAlloc& operator=(const SmallObjectAlloc&);

		std::vector<FixedSizeAlloc>		mPool;
		FixedSizeAlloc*					mLastAlloc;
		FixedSizeAlloc*					mLastDealloc;

		size_t							mMaxObjectSize;
	};
}

#include "MMSmallObjectAlloc.templates.h"

#endif // MMSMALLOBJECTALLOCATOR_H_INCLUDE_GUARD
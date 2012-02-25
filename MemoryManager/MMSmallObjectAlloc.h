#ifndef MMSMALLOBJECTALLOC_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLOC_H_INCLUDE_GUARD

#include "MMFixedSizeAlloc.h"

#include <vector>

namespace MM
{
	class SmallObjectAlloc
	{
	public:
		SmallObjectAlloc(size_t chunkSize, size_t maxObjectSize);

		void* Allocate(size_t size);
		void Deallocate(void* p);

	private:
		FixedSizeAlloc*	FindFixedSizeAlloc(void* p);

		SmallObjectAlloc(const SmallObjectAlloc&);
		SmallObjectAlloc& operator=(const SmallObjectAlloc&);

		std::vector<FixedSizeAlloc>		mPool;
		FixedSizeAlloc*					mLastAlloc;
		FixedSizeAlloc*					mLastDealloc;

		size_t							mChunkSize;
		size_t							mMaxObjectSize;
	};
}

#endif // MMSMALLOBJECTALLOCATOR_H_INCLUDE_GUARD
#ifndef MMSMALLOBJECTALLOC_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLOC_H_INCLUDE_GUARD

#include "MMFixedSizeAlloc.h"

namespace MM
{
	class SmallObjectAlloc
	{
	public:
		SmallObjectAlloc(size_t chunkSize, size_t maxObjectSize);

		void* Allocate(size_t numBytes);
		void Deallocate(void* p, size_t size);

	private:
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
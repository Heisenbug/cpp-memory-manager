#ifndef SMALLOBJECTALLOCATOR_H_INCLUDE_GUARD
#define SMALLOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "FixedSizeAllocator.h"

namespace MM
{
	class SmallObjectAllocator
	{
	public:
		SmallObjectAllocator(size_t chunkSize, size_t maxObjectSize);

		void* Allocate(size_t numBytes);
		void Deallocate(void* p, size_t size);

	private:
		SmallObjectAllocator(const SmallObjectAllocator&);
		SmallObjectAllocator& operator=(const SmallObjectAllocator&);

		std::vector<FixedSizeAllocator>		mPool;
		FixedSizeAllocator*					mLastAlloc;
		FixedSizeAllocator*					mLastDealloc;

		size_t								mChunkSize;
		size_t								mMaxObjectSize;
	};
}

#endif // SMALLOBJECTALLOCATOR_H_INCLUDE_GUARD
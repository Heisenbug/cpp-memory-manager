#include "SmallObjectAllocator.h"

#include <algorithm>
#include <cassert>

namespace MM
{
	SmallObjectAllocator::SmallObjectAllocator(size_t chunkSize, size_t maxObjectSize)
		: mLastAlloc(0), mLastDealloc(0), mChunkSize(chunkSize), mMaxObjectSize(maxObjectSize) { }

	void* SmallObjectAllocator::Allocate(size_t numBytes)
	{
		// if (numBytes > mMaxObjectSize) return operator new(numBytes);

		if (mLastAlloc && mLastAlloc->BlockSize() == numBytes)
		{
			return mLastAlloc->Allocate();
		}

		std::vector<FixedSizeAllocator>::iterator i = 
			std::lower_bound(mPool.begin(), mPool.end(), numBytes);
		
		if (i == mPool.end() || i->BlockSize() != numBytes)
		{
			i = mPool.insert(i, FixedSizeAllocator(numBytes));
			mLastDealloc = &*mPool.begin();
		}
		mLastAlloc = &*i;
		return mLastAlloc->Allocate();
	}

	void SmallObjectAllocator::Deallocate(void* p, std::size_t numBytes)
	{
		// if (numBytes > mMaxObjectSize) return operator delete(p);

		if (mLastDealloc && mLastDealloc->BlockSize() == numBytes)
		{
			mLastDealloc->Deallocate(p);
			return;
		}

		std::vector<FixedSizeAllocator>::iterator i = 
			std::lower_bound(mPool.begin(), mPool.end(), numBytes);

		assert(i != mPool.end());
		assert(i->BlockSize() == numBytes);
		mLastDealloc = &*i;
		mLastDealloc->Deallocate(p);
	}
}
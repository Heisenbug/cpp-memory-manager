#include "MMSmallObjectAlloc.h"

#include <algorithm>
#include <cassert>

namespace MM
{
	SmallObjectAlloc::SmallObjectAlloc(size_t chunkSize, size_t maxObjectSize)
		: mLastAlloc(0), mLastDealloc(0), mChunkSize(chunkSize), mMaxObjectSize(maxObjectSize) { }

	void* SmallObjectAlloc::Allocate(size_t numBytes)
	{
		// The following line could be replaced by an assert:
		// if (numBytes > mMaxObjectSize) return operator new(numBytes);

		if (mLastAlloc && mLastAlloc->BlockSize() == numBytes)
		{
			return mLastAlloc->Allocate();
		}

		std::vector<FixedSizeAlloc>::iterator i = 
			std::lower_bound(mPool.begin(), mPool.end(), numBytes);
		
		if (i == mPool.end() || i->BlockSize() != numBytes)
		{
			i = mPool.insert(i, FixedSizeAlloc(numBytes));
			mLastDealloc = &*mPool.begin();
		}
		mLastAlloc = &*i;
		return mLastAlloc->Allocate();
	}

	void SmallObjectAlloc::Deallocate(void* p, std::size_t numBytes)
	{
		// The following line could be replaced by an assert:
		// if (numBytes > mMaxObjectSize) return operator delete(p);

		if (mLastDealloc && mLastDealloc->BlockSize() == numBytes)
		{
			mLastDealloc->Deallocate(p);
			return;
		}

		std::vector<FixedSizeAlloc>::iterator i = 
			std::lower_bound(mPool.begin(), mPool.end(), numBytes);

		assert(i != mPool.end());
		assert(i->BlockSize() == numBytes);
		mLastDealloc = &*i;
		mLastDealloc->Deallocate(p);
	}
}
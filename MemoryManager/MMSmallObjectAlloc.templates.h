#ifndef MMSMALLOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD
#define MMSMALLOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD

#include <algorithm>
#include <cassert>

namespace MM
{
	template<typename LockPolicy>
	SmallObjectAlloc<LockPolicy>::SmallObjectAlloc(size_t maxObjectSize)
		: mLastAlloc(0), mLastDealloc(0), mMaxObjectSize(maxObjectSize) { }

	template<typename LockPolicy>
	SmallObjectAlloc<LockPolicy>::~SmallObjectAlloc() {

		std::cout << "SmallObjectAlloc destroyed" << std::endl;
	}

	template<typename LockPolicy>
	void* 
	SmallObjectAlloc<LockPolicy>::Allocate(size_t size)
	{
		LockPolicy lock;

		assert(!(size > mMaxObjectSize));

		if (mLastAlloc && mLastAlloc->BlockSize() == size)
		{
			return mLastAlloc->Allocate();
		}

		std::vector<FixedSizeAlloc>::iterator i = 
			std::lower_bound(mPool.begin(), mPool.end(), size);
		
		if (i == mPool.end() || i->BlockSize() != size)
		{
			FixedSizeAlloc tmp = FixedSizeAlloc(size, this);
			i = mPool.insert(i, tmp);
			mLastDealloc = &*mPool.begin();
		}
		mLastAlloc = &*i;
		return mLastAlloc->Allocate();
	}

	template<typename LockPolicy>
	void 
	SmallObjectAlloc<LockPolicy>::Deallocate(void* p)
	{
		LockPolicy lock;

		// Old version...bleah! XD

		/*
		if (mLastDealloc && mLastDealloc->BlockSize() == size)
		{
			mLastDealloc->Deallocate(p);
			return;
		}

		std::vector<FixedSizeAlloc>::iterator i = 
			std::lower_bound(mPool.begin(), mPool.end(), size);

		assert(i != mPool.end());
		assert(i->BlockSize() == size);
		mLastDealloc = &*i;
		mLastDealloc->Deallocate(p);
		*/

		// New version, without the spec. of the size as paramter.
		// We'll use a search technique similar to FixedAlloc::FindChunk().
		// "Not try. Do or not do, there is no try". [Yoda, The Empire Strikes Back]
		
		// Test if p has been allocated by the FixedSizeAlloc pointed by mLastDealloc
		if (mLastDealloc && mLastDealloc->FindChunk(p))
		{
			// Yeah! *mLastDealloc, deallocate it!
			mLastDealloc->Deallocate(p);
			return;
		}

		// Ouch! Okay, no matter. Search p's owner started from mLastDealloc
		mLastDealloc = FindFixedSizeAlloc(p);

		assert(mLastDealloc);
		// TODO: Some other asserts? Come on, think...
		mLastDealloc->Deallocate(p);
	}

	template<typename LockPolicy>
	FixedSizeAlloc* 
	SmallObjectAlloc<LockPolicy>::FindFixedSizeAlloc(void* p)
	{
		FixedSizeAlloc* lo		= mLastDealloc;
		FixedSizeAlloc* hi		= mLastDealloc + 1;
		FixedSizeAlloc* loBound = &mPool.front();
		FixedSizeAlloc* hiBound	= &mPool.back() + 1;

		if (hi == hiBound) hi = 0;

		for (;;)
		{
			if (lo)
			{
				if (lo->FindChunk(p))
				{
					return lo;
				}

				if (lo == loBound) lo = 0;
				else --lo;
			}

			if (hi)
			{
				if (hi->FindChunk(p))
				{
					return hi;
				}

				if (++hi == hiBound) hi = 0;
			}
		}

		assert(false);
		return 0;
	}
}

#endif // MMSMALLOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD
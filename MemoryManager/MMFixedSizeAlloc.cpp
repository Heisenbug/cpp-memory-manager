#include "MMFixedSizeAlloc.h"

#include "MMAllocationTable.h"

#include <utility>
#include <cassert>

namespace MM
{
	void FixedSizeAlloc::SetOwner(AllocatorInterface * o)
	{
		this->owner = o;
	}
	#pragma region Chunk

	void FixedSizeAlloc::Chunk::Init(size_t blockSize, unsigned char blocks)
	{
		mData					= new unsigned char[blockSize * blocks];
		mFirstAvailableBlock	= 0;
		mAvailableBlocks		= blocks;

		unsigned char i = 0;
		unsigned char* p = mData;
		for (; i != blocks; p += blockSize)
		{
			*p = ++i;
		}
	}

	void* FixedSizeAlloc::Chunk::Allocate(size_t blockSize)
	{
		if (!mAvailableBlocks) return 0;
		unsigned char* pRestult =
			mData + (mFirstAvailableBlock * blockSize);
		// Update mFirstAvailableBlock to point to the next block
		mFirstAvailableBlock = *pRestult;
		--mAvailableBlocks;
		return pRestult;
	}

	void FixedSizeAlloc::Chunk::Deallocate(void* p, size_t blockSize)
	{
		assert(p >= mData);
		unsigned char* toRelease = static_cast<unsigned char*>(p);
		// Alignment check
		assert((toRelease - mData) % blockSize == 0);
		*toRelease = mFirstAvailableBlock;
		mFirstAvailableBlock = static_cast<unsigned char>((toRelease - mData) / blockSize);
		// Truncation check
		assert(mFirstAvailableBlock == (toRelease - mData) / blockSize);
		++mAvailableBlocks;
	}

	void FixedSizeAlloc::Chunk::Release()
	{
		delete[] mData;
	}

	#pragma endregion

	#pragma region FixedSizeAlloc

	FixedSizeAlloc::FixedSizeAlloc(size_t blockSize)
		: mBlockSize(blockSize), mAllocChunk(0), mDeallocChunk(0)
	{
		assert(mBlockSize > 0);

		mPrev = mNext = this;

		size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;
		if (numBlocks > UCHAR_MAX) numBlocks = UCHAR_MAX;
		else if (numBlocks == 0) numBlocks = 8 * blockSize;

		mNumBlocks = static_cast<unsigned char>(numBlocks);
		assert(mNumBlocks == numBlocks);
	}

	FixedSizeAlloc::FixedSizeAlloc(const FixedSizeAlloc& a)
		: mBlockSize(a.mBlockSize), mNumBlocks(a.mNumBlocks), mChunks(a.mChunks)
	{
		mPrev			= &a;
		mNext			= a.mNext;
		a.mNext->mPrev	= this;
		a.mNext			= this;

		mAllocChunk = a.mAllocChunk
			? &mChunks.front() + (a.mAllocChunk - &a.mChunks.front())
			: 0;

		mDeallocChunk = a.mDeallocChunk
			? &mChunks.front() + (a.mDeallocChunk - &a.mChunks.front())
			: 0;
	}

	FixedSizeAlloc& FixedSizeAlloc::operator=(const FixedSizeAlloc& a)
	{
		FixedSizeAlloc copy(a);
		copy.Swap(*this);
		return *this;
	}

	FixedSizeAlloc::~FixedSizeAlloc()
	{
		if (mPrev != this)
		{
			mPrev->mNext = mNext;
			mNext->mPrev = mPrev;
			return;
		}

		assert(mPrev == mNext);
		Chunks::iterator i = mChunks.begin();
		for (; i != mChunks.end(); ++i)
		{
			assert(i->mAvailableBlocks == mNumBlocks);
			i->Release();
		}
	}

	void FixedSizeAlloc::Swap(FixedSizeAlloc& a)
	{
		std::swap(mBlockSize, a.mBlockSize);
		std::swap(mNumBlocks, a.mNumBlocks);
		mChunks.swap(a.mChunks);
		std::swap(mAllocChunk, a.mAllocChunk);
		std::swap(mDeallocChunk, a.mDeallocChunk);
	}

	FixedSizeAlloc::Chunk* FixedSizeAlloc::FindChunk(void* p)
	{
		assert(!mChunks.empty());
		assert(mDeallocChunk);

		// mChuncks is searched starting from mDeallocateChunk and going up and down with two iterators
		const std::size_t chunkLength = mNumBlocks * mBlockSize;

		Chunk* lo		= mDeallocChunk;
		Chunk* hi		= mDeallocChunk + 1;
		Chunk* loBound	= &mChunks.front();
		Chunk* hiBound	= &mChunks.back() + 1;

		for (;;)
		{
			if (lo)
			{
				if (p >= lo->mData && p < lo->mData + chunkLength)
				{
					return lo;
				}
				if (lo == loBound) lo = 0;
				else --lo;
			}

			if (hi)
			{
				if (p >= hi->mData && p < hi->mData + chunkLength)
				{
					return hi;
				}

				if (++hi == hiBound) hi = 0;
			}
		}

		//assert(false);
		return 0;
	}

	void* FixedSizeAlloc::Allocate()
	{
		if (mAllocChunk == 0 || mAllocChunk->mAvailableBlocks == 0)
		{
			// No available memory in this chunk
			// Try to find one
			Chunks::iterator i = mChunks.begin();
			for (;; ++i)
			{
				if (i == mChunks.end())
				{
					bool hasChangedAddress = false;
					// All filled up - add a new chunk
					if (mChunks.capacity() == mChunks.size()){
						hasChangedAddress = true;
						for (MM::FixedSizeAlloc::Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it ){
							AllocationTable::InvalidateChunk(&*it, DEFAULT_CHUNK_SIZE, this->owner);
						}
					}
					mChunks.reserve(mChunks.size() + 1);

					if (hasChangedAddress == true){
						for (MM::FixedSizeAlloc::Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it ){
							AllocationTable::RegisterChunk(&*it, DEFAULT_CHUNK_SIZE, this->owner);
						}
					}
					Chunk newChunk;
					newChunk.Init(mBlockSize, mNumBlocks);
					mChunks.push_back(newChunk);
					mAllocChunk		= &mChunks.back();
					mDeallocChunk	= &mChunks.front();

					//NEW CHUNK..should be registered
					AllocationTable::RegisterChunk(&mChunks.back(), DEFAULT_CHUNK_SIZE, this->owner);
					break;
				}

				if (i->mAvailableBlocks > 0)
				{
					// Found a chunk
					mAllocChunk = &*i;
					break;
				}
			}
		}

		assert(mAllocChunk != 0);
		assert(mAllocChunk->mAvailableBlocks > 0);

		return mAllocChunk->Allocate(mBlockSize);
	}

	void FixedSizeAlloc::Deallocate(void* p)
	{
		assert(!mChunks.empty());
		assert(&mChunks.front() <= mDeallocChunk);
		assert(&mChunks.back() >= mDeallocChunk);

		mDeallocChunk = FindChunk(p);
		assert(mDeallocChunk);

		// Deallocate
		assert(mDeallocChunk->mData <= p);
		assert(mDeallocChunk->mData + mNumBlocks * mBlockSize > p);

		// Call into the chunk, will adjust the inner list but won't release memory
		mDeallocChunk->Deallocate(p, mBlockSize);

		if (mDeallocChunk->mAvailableBlocks == mNumBlocks)
		{
			// mDeallocChunk is completely free, should we release it? 
			Chunk& lastChunk = mChunks.back();

			if (&lastChunk == mDeallocChunk)
			{
				// Check if we have two last chunks empty
				if (mChunks.size() > 1 && 
					mDeallocChunk[-1].mAvailableBlocks == mNumBlocks)
				{
					// Two free chunks, discard the last one
					lastChunk.Release();
					mChunks.pop_back();
					mAllocChunk = mDeallocChunk = &mChunks.front();
				}
				return;
			}

			if (lastChunk.mAvailableBlocks == mNumBlocks)
			{
				// Two free blocks, discard one
				lastChunk.Release();
				mChunks.pop_back();
				mAllocChunk = mDeallocChunk;
			}
			else
			{
				// Move the empty chunk to the end
				std::swap(*mDeallocChunk, lastChunk);
				mAllocChunk = &mChunks.back();
			}
		}
	}

	size_t FixedSizeAlloc::BlockSize() const
	{ 
		return mBlockSize; 
	}

	bool FixedSizeAlloc::operator<(size_t s) const
	{ 
		return BlockSize() < s; 
	}

	#pragma endregion

	
}
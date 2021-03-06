#ifndef MMFIXEDSIZEALLOC_H_INCLUDE_GUARD
#define MMFIXEDSIZEALLOC_H_INCLUDE_GUARD

#include "MMTrackableChunkHolderInterface.h"
#include "MMChunkInterface.h"
#include "MMAllocatorInterface.h"

#include <vector>

namespace MM
{
	class FixedSizeAlloc : public TrackableChunkHolderInterface
	{
	private:

		#pragma region Chunk structure

		struct FSAChunk : public ChunkInterface
		{
			void	Init(size_t blockSize, unsigned char blocks, AllocatorInterface* owner = 0);
			void*	Allocate(size_t blockSize);
			void	Deallocate(void* p, size_t blockSize);
			void	Release();

			// DataPointer	mData;
			unsigned char	mFirstAvailableBlock;
			unsigned char	mAvailableBlocks;
		};

		#pragma endregion

	public:

		#pragma region Constructors and destructor

		explicit FixedSizeAlloc(size_t blockSize = 0, AllocatorInterface* owner = 0);
		FixedSizeAlloc(const FixedSizeAlloc&);
		FixedSizeAlloc& operator=(const FixedSizeAlloc&);
		~FixedSizeAlloc();

		void Swap(FixedSizeAlloc& a);

		#pragma endregion

		#pragma region Allocation and deallocation 

		// Allocate a memory block
		void* Allocate();
		// Deallocate a memory block previously allocated with Allocate()
		void Deallocate(void* p);

		#pragma endregion

		#pragma region Utilities
		
		// Returns the block size with which the FixedSizeAlloc was initialized
		size_t BlockSize() const;
		// Comparison operator for sorting 
		bool operator<(size_t s) const;

		FSAChunk* FindChunk(void* p);

		#pragma endregion

	private:

		#pragma region Typedefs

		typedef std::vector<FSAChunk> Chunks;

		#pragma endregion

		#pragma region Data

		size_t			mBlockSize;
		unsigned char	mNumBlocks;
		Chunks			mChunks;

		// Last chunk used for an allocation
		FSAChunk*			mAllocChunk;
		// Last chuck accessed by a deallocation
		FSAChunk*			mDeallocChunk;

		// For ensuring proper copy semantics
		mutable const FixedSizeAlloc* mPrev;
		mutable const FixedSizeAlloc* mNext;

		#pragma endregion
	};
}

#endif // MMFIXEDSIZEALLOC_H_INCLUDE_GUARD
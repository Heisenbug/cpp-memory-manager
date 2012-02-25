#ifndef MMFIXEDSIZEALLOC_H_INCLUDE_GUARD
#define MMFIXEDSIZEALLOC_H_INCLUDE_GUARD

#include "MMChunkInterface.h"

#include <vector>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

namespace MM
{
	class FixedSizeAlloc
	{
	private:

		#pragma region Chunk structure

		struct Chunk : public ChunkInterface
		{
			void	Init(size_t blockSize, unsigned char blocks);
			void*	Allocate(size_t blockSize);
			void	Deallocate(void* p, size_t blockSize);
			void	Release();

			unsigned char*	mData;
			unsigned char	mFirstAvailableBlock;
			unsigned char	mAvailableBlocks;
		};

		#pragma endregion

	public:

		explicit FixedSizeAlloc(size_t blockSize = 0);
		FixedSizeAlloc(const FixedSizeAlloc&);
		FixedSizeAlloc& operator=(const FixedSizeAlloc&);
		~FixedSizeAlloc();

		void Swap(FixedSizeAlloc& a);

		// Allocate a memory block
		void* Allocate();
		// Deallocate a memory block previously allocated with Allocate()
		void Deallocate(void* p);

		// Returns the block size with which the FixedSizeAlloc was initialized
		size_t BlockSize() const;
		// Comparison operator for sorting 
		bool operator<(size_t s) const;

		#pragma region Utilities

		Chunk* FindChunk(void* p);

		#pragma endregion

	private:

		#pragma region Typedefs

		typedef std::vector<Chunk> Chunks;

		#pragma endregion

		#pragma region Data

		size_t			mBlockSize;
		unsigned char	mNumBlocks;
		Chunks			mChunks;

		// Last chunk used for an allocation
		Chunk*			mAllocChunk;
		// Last chuck accessed by a deallocation
		Chunk*			mDeallocChunk;

		// For ensuring proper copy semantics
		mutable const FixedSizeAlloc* mPrev;
		mutable const FixedSizeAlloc* mNext;

		#pragma endregion
	};
}

#endif // MMFIXEDSIZEALLOC_H_INCLUDE_GUARD
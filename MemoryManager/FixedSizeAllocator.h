#ifndef FIXEDSIZEALLOCATOR_H_INCLUDE_GUARD
#define FIXEDSIZEALLOCATOR_H_INCLUDE_GUARD

#include <vector>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

namespace MM
{
	class FixedSizeAllocator
	{
	public:
		explicit FixedSizeAllocator(size_t blockSize = 0);
		FixedSizeAllocator(const FixedSizeAllocator&);
		FixedSizeAllocator& operator=(const FixedSizeAllocator&);
		~FixedSizeAllocator();

		void Swap(FixedSizeAllocator& a);

		// Allocate a memory block
		void* Allocate();
		// Deallocate a memory block previously allocated with Allocate()
		void Deallocate(void* p);

		// Returns the block size with which the FixedSizeAllocator was initialized
		size_t BlockSize() const;
		// Comparison operator for sorting 
		bool operator<(size_t s) const;

	private:

		#pragma region Chunk structure
		
		struct Chunk
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

		#pragma region Utilities

		Chunk* FindChunk(void* p);

		#pragma endregion

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
		mutable const FixedSizeAllocator* mPrev;
		mutable const FixedSizeAllocator* mNext;

		#pragma endregion
	};
}

#endif // FIXEDSIZEALLOCATOR_H_INCLUDE_GUARD
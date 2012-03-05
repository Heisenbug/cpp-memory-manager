#ifndef MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD

#include "MMPreprocDirectives.h"

namespace MM 
{
	#pragma region Chunk structure

	template<typename LockPolicy>
	void
	SingleObjectAlloc<LockPolicy>::SingleChunk::Init(size_t chunkSize, AllocatorInterface* owner)
	{
		mData	= new unsigned char[chunkSize];
		mSize	= chunkSize;
		mOwner	= owner;
	}

	template<typename LockPolicy>
	void*
	SingleObjectAlloc<LockPolicy>::SingleChunk::Allocate()
	{
		return mData;
	}

	template<typename LockPolicy>
	void 
	SingleObjectAlloc<LockPolicy>::SingleChunk::Release()
	{
		delete[] mData;
	}

	#pragma endregion 

	#pragma region SingleChunkAlloc

	template<typename LockPolicy>
	SingleObjectAlloc<LockPolicy>::SingleObjectAlloc() 
	{ 
		mChunks.reserve(SINGLECHUNKALLOC_DEFAULT_CHUNK_NUMBER);
	}

	template<typename LockPolicy>
	SingleObjectAlloc<LockPolicy>::~SingleObjectAlloc()
	{
		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end() ; ++it)
		{
			AllocationTable::UnregisterChunk(*it);
			it->Release();	
		}

		std::cout << "SingleObjectAlloc destroyed" << std::endl;
	}

	template<typename LockPolicy>
	void* 
	SingleObjectAlloc<LockPolicy>::Allocate(size_t size)
	{
		LockPolicy lock;
 
		SingleChunk newChunk;
		newChunk.Init(size, this);
		mChunks.push_back(newChunk);

		AllocationTable::RegisterChunk(mChunks.back());
		return newChunk.Allocate();
	}

	template<typename LockPolicy>
	void 
	SingleObjectAlloc<LockPolicy>::Deallocate(void* p)
	{
		LockPolicy lock;

		DataPointer data = reinterpret_cast<DataPointer>(p);

		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end() ; ++it)
		{
			if (it->mData == p)
			{
				AllocationTable::UnregisterChunk(*it);
				it->Release();
				it = mChunks.erase(it);
				break;
			}
		}
	}

	#pragma endregion
}

#endif // MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD
#ifndef MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD

#define DEFAULT_CHUNK_NUMBER 20

namespace MM 
{
	#pragma region Chunk structure

	template<typename LockPolicy>
	void
	SingleChunkAlloc<LockPolicy>::SingleChunk::Init(size_t chunkSize, AllocatorInterface* owner)
	{
		mData	= new unsigned char[chunkSize];
		mSize	= chunkSize;
		mOwner	= owner;
	}

	template<typename LockPolicy>
	void*
	SingleChunkAlloc<LockPolicy>::SingleChunk::Allocate()
	{
		return mData;
	}

	template<typename LockPolicy>
	void 
	SingleChunkAlloc<LockPolicy>::SingleChunk::Release()
	{
		delete[] mData;
	}

	#pragma endregion 

	template<typename LockPolicy>
	SingleChunkAlloc<LockPolicy>::SingleChunkAlloc() 
	{ 
		mChunks.reserve(DEFAULT_CHUNK_NUMBER);
	}

	template<typename LockPolicy>
	SingleChunkAlloc<LockPolicy>::~SingleChunkAlloc()
	{
		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end() ; ++it)
		{
			AllocationTable::InvalidateChunk(&*it);
			it->Release();	
		}
	}

	template<typename LockPolicy>
	void* 
	SingleChunkAlloc<LockPolicy>::Allocate(size_t size)
	{
		LockPolicy lock;
				
		if (this->mChunks.size() == mChunks.capacity())
		{
			for (Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
			{	
				AllocationTable::InvalidateChunk(&*it);
			}
						
			mChunks.reserve(mChunks.size() + DEFAULT_CHUNK_NUMBER);
 
			for (Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
			{
				AllocationTable::RegisterChunk(&*it);
			}
		}
 
		SingleChunk newChunk;
		newChunk.Init(size, this);
		mChunks.push_back(newChunk);

		AllocationTable::RegisterChunk(&mChunks.back());
		return newChunk.Allocate();
	}

	template<typename LockPolicy>
	void 
	SingleChunkAlloc<LockPolicy>::Deallocate(void* p)
	{
		LockPolicy lock;

		DataPointer data = reinterpret_cast<DataPointer>(p);

		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end() ; ++it)
		{
			if (it->mData == p)
			{
				AllocationTable::InvalidateChunk(&*it);
				it->Release();
				it = mChunks.erase(it);
				break;
			}
		}
	}
}

#endif // MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD


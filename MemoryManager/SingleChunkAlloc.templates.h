
namespace MM 
{
	//---------- SINGLE CHUNK STRUCTURE -----------------------------
	template<typename LockPolicy>
	SingleChunkAlloc<LockPolicy>::SingleChunk::SingleChunk(size_t blockSize, AllocatorInterface* owner)
	{
		mOwner = owner;
		this->size = blockSize;
		mData =static_cast<DataPointer>(malloc(blockSize));
	}

	template<typename LockPolicy>
	void SingleChunkAlloc<LockPolicy>::SingleChunk::Release()
	{
		if (mData) 
			free(mData);
	}


	//------------------ SINGLE CHUNK ALLOC -------------------------------
	template<typename LockPolicy>
	SingleChunkAlloc<LockPolicy>::SingleChunkAlloc() { }

	template<typename LockPolicy>
	SingleChunkAlloc<LockPolicy>::~SingleChunkAlloc()
	{
		for (std::vector<SingleChunk>::iterator it= this->chunks.begin(); it !=this->chunks.end() ; ++it)
		{
			AllocationTable::InvalidateChunk(&*it);
			(*it).Release();	
		}
	}

	template<typename LockPolicy>
	void* SingleChunkAlloc<LockPolicy>::Allocate(size_t size)
	{
		LockPolicy lock;
		SingleChunk newChunk(size, this);
		AllocationTable::RegisterChunk(&newChunk);

		this->chunks.push_back(newChunk);

		return newChunk.mData;
	}

	template<typename LockPolicy>
	void SingleChunkAlloc<LockPolicy>::Deallocate(void* ptr)
	{
		LockPolicy lock;
		DataPointer data = reinterpret_cast<DataPointer>(ptr);

		for (std::vector<SingleChunk>::iterator it= this->chunks.begin(); it !=this->chunks.end() ; ++it)
		{
			if ((*it).mData == ptr)
			{

				SingleChunk chunk = *it; 
				this->chunks.erase(it);
				AllocationTable::InvalidateChunk(&chunk);
				chunk.Release();

			}

		}
	}
}




//---------- SINGLE CHUNK STRUCTURE -----------------------------
template<typename LockPolicy>
void MM::SingleChunkAlloc<LockPolicy>::SingleChunk(size_t blockSize, AllocatorInterface* owner)
{
	SetOwner(owner);
	this->size = blockSize;
	mData =static_cast<DataPointer> malloc(blockSize);

}
template<typename LockPolicy>
void* MM::SingleChunkAlloc<LockPolicy>::Allocate()
{
	return mData;
}

template<typename LockPolicy>
void MM::SingleChunkAlloc<LockPolicy>::Release()
{
	if (mData) 
		free(mData);
}


//------------------ SINGLE CHUNK ALLOC -------------------------------
template<typename LockPolicy>
explicit SingleChunkAlloc<LockPolicy>::SingleChunkAlloc()
{
	this->chunks = new std::vector<SingleChunk*>();
}

template<typename LockPolicy>
SingleChunkAlloc<LockPolicy>::~SingleChunkAlloc(void)
{
	for (std::vector<SingleChunk*>::iterator it= this->chunks.begin(); it !=this->chunks.end() ; ++i)
	{
		AllocationTable::InvalidateChunk(&*it);
		(*it)->Release();	
	}
	delete this->chunks;
}

template<typename LockPolicy>
SingleChunkAlloc<LockPolicy>::void* Allocate(size_t size)
{
	LockPolicy lock;
	SingleChunk *newChunk = SingleChunk(size, this);
	AllocationTable::RegisterChunk(newChunk);

	this->chunks.push_back(newChunk);
}
template<typename LockPolicy>
SingleChunkAlloc<LockPolicy>::void Deallocate(void* ptr)
{
	LockPolicy lock;
	DataPointer data = reinterpret_cast<DataPointer>(ptr);

	for (std::vector<SingleChunk*>::iterator it= this->chunks.begin(); it !=this->chunks.end() ; ++i)
	{
		if ((*it)->mData == ptr)
		{

			ChunkInterface * chunk = *it; 
			this->chunks.erase(it);
			AllocationTable::InvalidateChunk(chunk);
			chunk->Release();

		}
			
	}
}


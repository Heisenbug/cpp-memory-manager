#ifndef MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD

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
	SingleChunkAlloc<LockPolicy>::SingleChunkAlloc() { 
		this->mChunks.reserve(DEFAULT_CHUNK_NUM);
	}

	template<typename LockPolicy>
	SingleChunkAlloc<LockPolicy>::~SingleChunkAlloc()
	{
		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end() ; ++it)
		{
			MM::SingletonHolder<AllocationTable>::Instance().InvalidateChunk(&*it);
			//AllocationTable::InvalidateChunk(&*it);
			it->Release();	
		}
	}

	template<typename LockPolicy>
        void* 
        SingleChunkAlloc<LockPolicy>::Allocate(size_t size)
        {
                LockPolicy lock;
 
                //bool hasChangedAddress = false;
                
                if (this->mChunks.size() == mChunks.capacity()){
                
					MM::AllocationTable::Dump();
                        //hasChangedAddress = true;
                        for (Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
                        {
							void *toRemove = &*it;
							MM::SingletonHolder<AllocationTable>::Instance().InvalidateChunk(&*it);
                            //AllocationTable::InvalidateChunk(&*it);
                        }
						MM::AllocationTable::Dump();
                        mChunks.reserve(mChunks.capacity() + DEFAULT_CHUNK_NUM);
 
                        for (Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
                        {
							MM::SingletonHolder<AllocationTable>::Instance().RegisterChunk(&*it);
                            //AllocationTable::RegisterChunk(&*it);
                        }
					//	MM::AllocationTable::Dump();
                }
 
                SingleChunk newChunk;
                newChunk.Init(size, this);
                mChunks.push_back(newChunk);
 
			//	MM::AllocationTable::Dump();
				MM::SingletonHolder<AllocationTable>::Instance().RegisterChunk(&mChunks.back());
                //AllocationTable::RegisterChunk(&mChunks.back());
			//	MM::AllocationTable::Dump();
                return newChunk.Allocate();
        }
	//template<typename LockPolicy>
	//void* 
	//SingleChunkAlloc<LockPolicy>::Allocate(size_t size)
	//{
	//	LockPolicy lock;

	//	bool hasChangedAddress = false;
	//	if (this->mChunks.size() == mChunks.capacity()){
	//	
	//		hasChangedAddress = true;
	//		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
	//		{
	//			MM::SingletonHolder<AllocationTable>::Instance().InvalidateChunk(&*it);
	//			//AllocationTable::InvalidateChunk(&*it);
	//		}
	//		this->mChunks.reserve(this->mChunks.capacity() + DEFAULT_CHUNK_NUM);	
	//		
	//		if (hasChangedAddress == true)
	//		{
	//		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
	//		{
	//			MM::SingletonHolder<AllocationTable>::Instance().RegisterChunk(&*it);
	//			//AllocationTable::RegisterChunk(&*it);
	//		}
	//	}
	//	}
	//	

	//	SingleChunk newChunk;
	//	newChunk.Init(size, this);
	//	mChunks.push_back(newChunk);

	//	MM::SingletonHolder<AllocationTable>::Instance().RegisterChunk(&mChunks.back());

	//	//AllocationTable::RegisterChunk(&mChunks.back());

	//	return newChunk.Allocate();
	//}

	template<typename LockPolicy>
	void 
	SingleChunkAlloc<LockPolicy>::Deallocate(void* p)
	{
		LockPolicy lock;

		DataPointer data = reinterpret_cast<DataPointer>(p);

		
		//while(mChunks.begin() != mChunks.end()){
		//	Chunks::iterator it = mChunks.begin();
		//	if (it->mData == p)
		//	{
		//		MM::SingletonHolder<AllocationTable>::Instance().InvalidateChunk(&*it);
		//		//AllocationTable::InvalidateChunk(&*it);
		//		it->Release();
		//		mChunks.erase(it);
		//		break;
		//	}
		//}
		for (Chunks::iterator it = mChunks.begin(); it != mChunks.end() ; ++it)
		{
			if (it->mData == p)
			{
				MM::SingletonHolder<AllocationTable>::Instance().InvalidateChunk(&*it);
				//AllocationTable::InvalidateChunk(&*it);
				it->Release();
				it = mChunks.erase(it);
				break;
			}
		}
	}
}

#endif // MMSINGLEOBJECTALLOC_TEMPLATES_H_INCLUDE_GUARD


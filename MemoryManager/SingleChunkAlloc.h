#pragma once

#include "MMAllocatorInterface.h"
#include "MMChunkInterface.h"
#include "MMLockPoliclies.h"
#include "MMAllocationTable.h"

#include <vector>

namespace MM{

template<typename LockPolicy = NoLock>
class SingleChunkAlloc : public AllocatorInterface
{
	

public:
	explicit SingleChunkAlloc();
	~SingleChunkAlloc(void);

	void* Allocate(size_t);
	void Deallocate(void*);
	
private:
	struct SingleChunk : public ChunkInterface
		{
			SingleChunk(size_t blockSize, AllocatorInterface* owner);
			void	Release();		
			size_t size;
		};

	
	
	std::vector<SingleChunk *> chunks;	
	
};

#include "SingleChunkAlloc.templates.h"
}
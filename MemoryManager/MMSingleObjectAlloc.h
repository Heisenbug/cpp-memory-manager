#ifndef MMSINGLEOBJECTALLOC_H_INCLUDE_GUARD
#define MMSINGLEOBJECTALLOC_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"
#include "MMChunkInterface.h"
#include "MMAllocationTable.h"

#include <vector>

namespace MM
{
	template<typename LockPolicy>
	class SingleChunkAlloc : public AllocatorInterface
	{
	private:

		#pragma region Chunk structure

		struct SingleChunk : public ChunkInterface
		{
			void Init(size_t chunkSize, AllocatorInterface* owner);
			void* Allocate();
			void Release();
		};

		#pragma endregion

	public:

		SingleChunkAlloc();
		virtual ~SingleChunkAlloc();

		void* Allocate(size_t size);
		void Deallocate(void* p);
	
	private:

		typedef std::vector<SingleChunk> Chunks;

		Chunks mChunks;
	};
}

#include "MMSingleObjectAlloc.templates.h"

#endif // MMSINGLEOBJECTALLOC_H_INCLUDE_GUARD
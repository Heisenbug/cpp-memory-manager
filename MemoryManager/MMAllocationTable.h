#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
#define MMALLOCATIONTABLE_H_INCLUDE_GUARD

#include "MMChunkInterface.h"
#include "MMAllocatorInterface.h"

#include "MMSingletonHolder.h"

#include <utility>
#include <map>

namespace MM
{
	struct ChunkIndex 
	{
		ChunkIndex(ChunkInterface* p, size_t size = 0)
			: mIndex(std::make_pair(p, size)) { }

		std::pair<ChunkInterface*, size_t> mIndex;
	};

	bool operator==(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		return i1.mIndex.
	}

	typedef std::map<ChunkIndex, AllocatorInterface*> AllocTable; 

	class AllocationTable
	{
	public:

		static void RegisterChunk(ChunkInterface* c, size_t size, AllocatorInterface* a)
		{
			SingletonHolder<AllocTable>::Instance().insert( ChunkIndex(c, size) );
		}

		// Implement a search function using std::map::find() method
	};
}

#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD
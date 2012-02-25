#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
#define MMALLOCATIONTABLE_H_INCLUDE_GUARD

#include "MMChunkInterface.h"
#include "MMAllocatorInterface.h"

#include "MMSingletonHolder.h"

#include <utility>
#include <map>
#include <cassert>

namespace MM
{
	// Chunk index is the key type used for the AllocationTable
	#pragma region ChunkIndex

	struct ChunkIndex 
	{
		ChunkIndex(ChunkInterface* p, size_t size = 0)
			: mIndex(std::make_pair(p, size)) { }

		std::pair<ChunkInterface*, size_t> mIndex;
	};

	#pragma region Comparsion operators

	bool operator==(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		// Suppose i1 = <p, s> and i2 = <q, t>

		// If s == 0 || t == 0
		if (i1.mIndex.second == 0 || i2.mIndex.second == 0)
		{
			// Case s == 0
			if (i1.mIndex.first == 0)
			{
				// Check if (p >= q && p < q + t)
				return (i1.mIndex.first >= i2.mIndex.first &&
							i1.mIndex.first < i2.mIndex.first + i2.mIndex.second);
			}
			else
			{
				// Case t == 0

				// Check if (q >= p && q < p + s)
				return (i2.mIndex.first >= i1.mIndex.first &&
							i2.mIndex.first < i1.mIndex.first + i1.mIndex.second);
			}
		}
		
		return (i1 == i2);
	}

	bool operator<(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		return i1.mIndex.first < i2.mIndex.first;
	}

	bool operator>(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		return i1.mIndex.first > i2.mIndex.first;
	}

	bool operator<=(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		return (i1 < i2 || i1 == i2);
	}

	bool operator>=(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		return (i1 > i2 || i1 == i2);
	}

	bool operator!=(const ChunkIndex& i1, const ChunkIndex& i2)
	{
		return !(i1 == i2);
	}

	#pragma endregion

	#pragma endregion 

	typedef std::map<ChunkIndex, AllocatorInterface*> AllocTable; 

	class AllocationTable
	{
	public:

		static void RegisterChunk(ChunkInterface* c, size_t size, AllocatorInterface* a)
		{
			SingletonHolder<AllocTable>::Instance().insert( ChunkIndex(c, size) );
		}

		static AllocatorInterface* FindAllocatorFor(void* p)
		{
			// TODO: This segment is NOT thread-safe! (A multiple acces can invalidate the calculus of end())
			AllocTable::iterator res = 
				SingletonHolder<AllocTable>::Instance().find( ChunkIndex(reinterpret_cast<ChunkInterface*>(p)) );

			if (res != SingletonHolder<AllocTable>::Instance().end())
			{
				return res->second;
			}

			// "You shall not pass!" (Gandalf, LOTR) XD
			assert(false);
			return 0;
		}
	};
}

#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD
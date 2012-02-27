#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
#define MMALLOCATIONTABLE_H_INCLUDE_GUARD

#include "MMChunkInterface.h"
#include "MMSingletonHolder.h"

#include <vector>
#include <algorithm>
#include <cassert>

#define DEFAULT_CHUNK_NUMBER 100

namespace MM
{
	typedef std::vector<ChunkInterface*> AllocTable;

	// Comparsion predicate
	struct ChunkComparsion
	{
		bool operator()(const AllocTable::value_type& c1, const AllocTable::value_type& c2) const
		{
			return (c1->mData < c2->mData);
		}
	};

	class AllocationTable
	{
	public:

		AllocationTable()  
		{
			mTable.reserve(DEFAULT_CHUNK_NUMBER);
		}

		static void RegisterChunk(ChunkInterface* c)
		{
			SingletonHolder<AllocationTable>::Instance().mTable.push_back(c);

			std::sort(SingletonHolder<AllocationTable>::Instance().mTable.begin(), 
				SingletonHolder<AllocationTable>::Instance().mTable.end(), ChunkComparsion());
		}

		static void InvalidateChunk(ChunkInterface* c)
		{
			AllocTable::iterator found = std::find(SingletonHolder<AllocationTable>::Instance().mTable.begin(), 
				SingletonHolder<AllocationTable>::Instance().mTable.end(), c);

			SingletonHolder<AllocationTable>::Instance().mTable.erase(found);
		}

		static AllocatorInterface* FindAllocatorFor(void* p)
		{
			// TODO: This segment is NOT thread-safe! (A multiple acces can invalidate the calculus of end())

			// TODO: Convert it into a binary search, fool!
			AllocTable::const_iterator it = SingletonHolder<AllocationTable>::Instance().mTable.begin();
			for (; it != SingletonHolder<AllocationTable>::Instance().mTable.end(); ++it)
			{
				if (!((*it)->mData < p))
					break;
			}
				
			if (it != SingletonHolder<AllocationTable>::Instance().mTable.end())
			{
				AllocatorInterface* a = (*it)->mOwner;
				return (*it)->mOwner;
			}

			// "You shall not pass!" (Gandalf, LOTR) XD
			assert(false);
			return 0;
		}

	private:

		AllocTable mTable;
	};
}

#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD





































//#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
//#define MMALLOCATIONTABLE_H_INCLUDE_GUARD
//
//#include "MMChunkInterface.h"
//#include "MMAllocatorInterface.h"
//
//#include "MMSingletonHolder.h"
//
//#include <utility>
//#include <map>
//#include <cassert>
//
//namespace MM
//{
//	// Chunk index is the key type used for the AllocationTable
//	#pragma region ChunkIndex
//
//	struct ChunkIndex 
//	{
//		ChunkIndex(ChunkInterface* p, size_t size = 0)
//			: mIndex(std::make_pair(p, size)) { }
//
//		std::pair<ChunkInterface*, size_t> mIndex;
//	};
//
//	#pragma region Comparsion operators
//
//	inline bool operator==(const ChunkIndex& i1, const ChunkIndex& i2)
//	{
//		// Suppose i1 = <p, s> and i2 = <q, t>
//
//		// If s == 0 || t == 0
//		if (i1.mIndex.second == 0 || i2.mIndex.second == 0)
//		{
//			// Case s == 0
//			if (i1.mIndex.first == 0)
//			{
//				// Check if (p >= q && p < q + t)
//				return (i1.mIndex.first >= i2.mIndex.first &&
//							i1.mIndex.first < i2.mIndex.first + i2.mIndex.second);
//			}
//			else
//			{
//				// Case t == 0
//
//				// Check if (q >= p && q < p + s)
//				return (i2.mIndex.first >= i1.mIndex.first &&
//							i2.mIndex.first < i1.mIndex.first + i1.mIndex.second);
//			}
//		}
//		
//		return (i1.mIndex == i2.mIndex);
//	}
//
//	inline bool operator<(const ChunkIndex& i1, const ChunkIndex& i2)
//	{
//		return i1.mIndex.first < i2.mIndex.first;
//	}
//
//	inline bool operator>(const ChunkIndex& i1, const ChunkIndex& i2)
//	{
//		return i1.mIndex.first > i2.mIndex.first;
//	}
//
//	inline bool operator<=(const ChunkIndex& i1, const ChunkIndex& i2)
//	{
//		return (i1 < i2 || i1 == i2);
//	}
//
//	inline bool operator>=(const ChunkIndex& i1, const ChunkIndex& i2)
//	{
//		return (i1 > i2 || i1 == i2);
//	}
//
//	inline bool operator!=(const ChunkIndex& i1, const ChunkIndex& i2)
//	{
//		return !(i1 == i2);
//	}
//
//	#pragma endregion
//
//	#pragma endregion 
//
//	typedef std::map<ChunkIndex, AllocatorInterface*> AllocTable; 
//
//	class AllocationTable
//	{
//	public:
//
//		static void RegisterChunk(ChunkInterface* c, size_t size, AllocatorInterface* a)
//		{
//			//put allocator interface inside AllocatorDescriptor
//			SingletonHolder<AllocTable>::Instance().insert( std::make_pair(ChunkIndex(c, size), a) );
//		}
//
//		static void InvalidateChunk(ChunkInterface* c, size_t size, AllocatorInterface* a)
//		{
//			SingletonHolder<AllocTable>::Instance().erase( ChunkIndex(c, size) );
//		}
//
//		static AllocatorInterface* FindAllocatorFor(void* p)
//		{
//			// TODO: This segment is NOT thread-safe! (A multiple acces can invalidate the calculus of end())
//			AllocTable::iterator res = 
//				SingletonHolder<AllocTable>::Instance().find( ChunkIndex(reinterpret_cast<ChunkInterface*>(p)) );
//
//			if (res != SingletonHolder<AllocTable>::Instance().end())
//			{
//				return res->second;
//			}
//
//			// "You shall not pass!" (Gandalf, LOTR) XD
//			assert(false);
//			return 0;
//		}
//	};
//}
//
//#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD
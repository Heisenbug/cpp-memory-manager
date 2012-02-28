#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
#define MMALLOCATIONTABLE_H_INCLUDE_GUARD

#include "MMChunkInterface.h"
#include "MMSingletonHolder.h"

#include <vector>
#include <algorithm>
#include <cassert>

#ifndef DEFAULT_TABLE_SIZE
#define DEFAULT_TABLE_SIZE 100
#endif

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
			mTable.reserve(DEFAULT_TABLE_SIZE);
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
#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
#define MMALLOCATIONTABLE_H_INCLUDE_GUARD

#include "MMChunkInterface.h"
#include "MMSingletonHolder.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "MMPreprocDirectives.h"

namespace MM
{
	class AllocationTable
	{
	private:

		typedef std::vector<ChunkInterface> AllocTable;

#ifndef MM_ENABLE_MULTITHREAD
		typedef NoLock LockPolicy;
#else
		struct AllocationTableTypeTag { };

		typedef MutexLock<AllocationTableTypeTag> LockPolicy;
#endif

		typedef SingletonHolder<AllocationTable, LockPolicy> AllocationTableSingleton;

		// Comparsion predicate
		struct ChunkComparsion
		{
			bool operator()(const AllocTable::value_type& c1, const AllocTable::value_type& c2) const
			{
				return (c1.mData < c2.mData);
			}
		};

	public:

		AllocationTable()  
		{
			LockPolicy lock;

			mTable.reserve(MM_DEFAULT_ALLOCATION_TABLE_SIZE);	
			atexit(Destroy);
		}

		static void RegisterChunk(const ChunkInterface& c)
		{
			LockPolicy lock;

			AllocationTableSingleton::Instance().mTable.push_back(c);

			std::sort(AllocationTableSingleton::Instance().mTable.begin(), 
				AllocationTableSingleton::Instance().mTable.end(), ChunkComparsion());
		}

		static void UnregisterChunk(const ChunkInterface& c)
		{
			LockPolicy lock;

			AllocTable::iterator found = std::find(AllocationTableSingleton::Instance().mTable.begin(), 
				AllocationTableSingleton::Instance().mTable.end(), c);

			AllocationTableSingleton::Instance().mTable.erase(found);
		}

		static AllocatorInterface* FindAllocatorFor(void* p)
		{
			LockPolicy lock;

			assert(!mTable.empty());

			// TODO: Convert it into a binary search, fool!
			AllocTable::const_iterator it = AllocationTableSingleton::Instance().mTable.begin();
			
			for (; it != AllocationTableSingleton::Instance().mTable.end(); ++it)
			{
				DataPointer dataAddress = it->mData;
				size_t		dataSize	= it->mSize;

				if (dataAddress <= p && p <= dataAddress + dataSize)
					break;		
			}

			if (it != AllocationTableSingleton::Instance().mTable.end())
			{
				AllocatorInterface* a = it->mOwner;
				return a;
			}

			// "You shall not pass!" (Gandalf, LOTR) XD
			assert(false);
			return 0;
		}

		static void Destroy()
		{
			// TODO: Destroy the AllocationTracker? Or not...this is the problem.

			while (!mTable.empty())
			{
				// Destroy all allocator's istances
				delete mTable.back().mOwner;		
			}

			// TODO: Self-destruct! o_O
		}

		static void Dump(std::ostream& os)
		{
			LockPolicy lock;

			os << "------------------------------------------------------------------------" << std::endl;
			os << "mTable size: " << AllocationTableSingleton::Instance().mTable.size() << std::endl;
			os << "------------------------------------------------------------------------" << std::endl;
			int i = 0;
			for (AllocTable::iterator it = AllocationTableSingleton::Instance().mTable.begin(); 
					it != AllocationTableSingleton::Instance().mTable.end(); ++it)
			{
				os << "Entry : " << i
							<< "\t" << "mData: " << reinterpret_cast<void*>(it->mData)
							<< "\t" << "mSize: " << it->mSize
							<< "\t" << "mOwner: " << reinterpret_cast<void*>(it->mOwner) << std::endl;
				++i;
			}
			os << "------------------------------------------------------------------------" << std::endl;
		}

	private:

		static AllocTable mTable;
	};
}

#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD
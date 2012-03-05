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
	class AllocTable
	{
	private:

		typedef std::vector<ChunkInterface> Table;

		// Comparsion predicate
		struct ChunkComparsion
		{
			bool operator()(const Table::value_type& c1, const Table::value_type& c2) const
			{
				return (c1.mData < c2.mData);
			}
		};

	public:

		AllocTable()  
		{
			mTable.reserve(MM_DEFAULT_ALLOCATION_TABLE_SIZE);	
		}

		~AllocTable() { }

		void RegisterChunk(const ChunkInterface& c)
		{
			mTable.push_back(c);

			std::sort(mTable.begin(), mTable.end(), ChunkComparsion());
		}

		void UnregisterChunk(const ChunkInterface& c)
		{
			Table::iterator found = std::find(mTable.begin(), mTable.end(), c);

			mTable.erase(found);
		}

		AllocatorInterface* FindAllocatorFor(void* p)
		{
			assert(!mTable.empty());

			// TODO: Convert it into a binary search, fool!
			Table::const_iterator it = mTable.begin();

			for (; it != mTable.end(); ++it)
			{
				DataPointer dataAddress = it->mData;
				size_t		dataSize	= it->mSize;

				if (dataAddress <= p && p <= dataAddress + dataSize)
					break;		
			}

			if (it != mTable.end())
			{
				AllocatorInterface* a = it->mOwner;
				return a;
			}

			// "You shall not pass!" (Gandalf, LOTR) XD
			assert(false);
			return 0;
		}

		void Dump(std::ostream& os)
		{
			os << "------------------------------------------------------------------------" << std::endl;
			os << "mTable size: " << mTable.size() << std::endl;
			os << "------------------------------------------------------------------------" << std::endl;
			int i = 0;
			for (Table::iterator it = mTable.begin(); it != mTable.end(); ++it)
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

		Table mTable;
	};

	class AllocationTable
	{
	private:

#ifndef MM_ENABLE_MULTITHREAD
		typedef NoLock LockPolicy;
#else
		struct AllocationTableTypeTag { };

		typedef MutexLock<AllocationTableTypeTag> LockPolicy;
#endif

		typedef SingletonHolder<AllocTable, LockPolicy> AllocationTableSingleton;

	public:

		static void Init()
		{
			LockPolicy lock;
			
			// Safe call to the singleton's instance, table creation
			AllocationTableSingleton::Instance();
		}

		static void RegisterChunk(const ChunkInterface& c)
		{
			LockPolicy lock;

			AllocationTableSingleton::Instance().RegisterChunk(c);
		}

		static void UnregisterChunk(const ChunkInterface& c)
		{
			LockPolicy lock;

			AllocationTableSingleton::Instance().UnregisterChunk(c);
		}

		static AllocatorInterface* FindAllocatorFor(void* p)
		{
			LockPolicy lock;

			return AllocationTableSingleton::Instance().FindAllocatorFor(p);
		}

		static void Dump(std::ostream& os)
		{
			LockPolicy lock;

			AllocationTableSingleton::Instance().Dump(os);
		}
	};
}

#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD
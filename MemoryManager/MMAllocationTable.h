#ifndef MMALLOCATIONTABLE_H_INCLUDE_GUARD
#define MMALLOCATIONTABLE_H_INCLUDE_GUARD

#include "MMChunkInterface.h"
#include "MMSingletonHolder.h"

#include <vector>
#include <algorithm>
#include <cassert>

#include <iostream>

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
			atexit(Destroy);
		}

		static void RegisterChunk(ChunkInterface* c)
		{
			SingletonHolder<AllocationTable>::Instance().mTable.push_back(c);

			std::sort(SingletonHolder<AllocationTable>::Instance().mTable.begin(), 
				SingletonHolder<AllocationTable>::Instance().mTable.end(), ChunkComparsion());
//			Dump();
		}

		static void InvalidateChunk(ChunkInterface* c)
		{
			AllocTable::iterator found = std::find(SingletonHolder<AllocationTable>::Instance().mTable.begin(), 
				SingletonHolder<AllocationTable>::Instance().mTable.end(), c);

			SingletonHolder<AllocationTable>::Instance().mTable.erase(found);
		}

		static AllocatorInterface* FindAllocatorFor(void* p)
		{
			assert(!mTable.empty());

			std::cout << "Try to search an allocator for " << p << std::endl;

			// TODO: This segment is NOT thread-safe! (A multiple acces can invalidate the calculus of end())

			// TODO: Convert it into a binary search, fool!
			AllocTable::const_iterator it = SingletonHolder<AllocationTable>::Instance().mTable.begin();
			
			for (; it != SingletonHolder<AllocationTable>::Instance().mTable.end(); ++it)
			{
				DataPointer dataAddress = (*it)->mData;
				size_t		dataSize	= (*it)->mSize;

				if (dataAddress <= p && p <= dataAddress + dataSize)
					break;		
			}

			if (it != SingletonHolder<AllocationTable>::Instance().mTable.end())
			{
				AllocatorInterface* a = (*it)->mOwner;
				return a;
			}

			//if (mTable.size() != 1)
			//{
			//	for (; it != SingletonHolder<AllocationTable>::Instance().mTable.end(); ++it)
			//	{
			//		if (!((*it)->mData < p))
			//			break;
			//	}
			//}
			//	
			//if (it != SingletonHolder<AllocationTable>::Instance().mTable.end())
			//{
			//	AllocatorInterface* a = (*it)->mOwner;
			//	return (*it)->mOwner;
			//}
			//else 
			//{

			//}

			// "You shall not pass!" (Gandalf, LOTR) XD
			//assert(false);
			return 0;
		}

		static void Destroy()
		{
			while (!mTable.empty())
			{
				mTable.back()->mOwner->~AllocatorInterface();
			}
		}

		static void Dump()
		{
			std::cout << "--------------------------------------------------------------" << std::endl;
			std::cout << "mTable size: " << mTable.size() << std::endl;
			std::cout << "--------------------------------------------------------------" << std::endl;
			int i = 0;
			for (AllocTable::iterator it = mTable.begin(); it != mTable.end(); ++it)
			{
				std::cout << "Entry : " << i << " *: "	<< reinterpret_cast<void*>((*it))
							<< "\t" << "mData: " << reinterpret_cast<void*>((*it)->mData)
							<< "\t" << "mSize: " << (*it)->mSize
							<< "\t" << "mOwner: " << reinterpret_cast<void*>((*it)->mOwner) << std::endl;
				++i;
			}
		}

	private:

		static AllocTable mTable;
	};
}

#endif // MMALLOCATIONTABLE_H_INCLUDE_GUARD
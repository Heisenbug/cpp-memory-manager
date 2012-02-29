#ifndef MMCHUNKINTERFACE_H_INCLUDE_GUARD
#define MMCHUNKINTERFACE_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

namespace MM
{
	typedef unsigned char* DataPointer;

	struct ChunkInterface 
	{
		virtual ~ChunkInterface() { }

		DataPointer			mData;
		size_t				mSize;

		AllocatorInterface* mOwner;
	};

	inline bool operator==(const ChunkInterface& c1, const ChunkInterface& c2)
	{
		return c1.mData == c2.mData;
	}
}

#endif // MMCHUNKINTERFACE_H_INCLUDE_GUARD
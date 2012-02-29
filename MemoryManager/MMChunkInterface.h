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
}

#endif // MMCHUNKINTERFACE_H_INCLUDE_GUARD
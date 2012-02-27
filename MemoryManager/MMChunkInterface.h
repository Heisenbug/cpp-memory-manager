#ifndef MMCHUNKINTERFACE_H_INCLUDE_GUARD
#define MMCHUNKINTERFACE_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

namespace MM
{
	struct ChunkInterface 
	{
		virtual ~ChunkInterface() { }

		unsigned char*		mData;
		AllocatorInterface* mOwner;
	};
}

#endif // MMCHUNKINTERFACE_H_INCLUDE_GUARD
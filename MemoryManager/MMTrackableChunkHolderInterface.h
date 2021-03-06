#ifndef MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD
#define MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

namespace MM
{
	class TrackableChunkHolderInterface
	{
	protected:

		// Every lowlevel concrete allocator MUST implement this interface.
		// mOwner is a cross-layer reference to the AllocatorInterface.
		// Every concrete class implementing this interface MUST use the owner 
		// reference while registering allocated ChunkInterface's to the AllocationTable 
		virtual void SetOwner(AllocatorInterface* owner)
		{
			mOwner = owner;
		}

		virtual ~TrackableChunkHolderInterface() { }

		AllocatorInterface*	mOwner; // Cross-layer reference to the owner
	};
}

#endif // MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD
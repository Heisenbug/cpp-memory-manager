#ifndef MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD
#define MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD

#include "MMAllocatorInterface.h"

namespace MM
{
	class TrackableChunkHolderInterface
	{
	public:

		// Every lowlevel concrete allocator MUST implement this interface.
		// owner is a cross-layer reference to the AllocatorInterface.
		// Every concrete class implementing this interface MUST use the owner 
		// reference while registering allocated MMChunkInterface's to the MMAllocationTable 
		virtual void SetOwner(AllocatorInterface* owner) = 0;
	};
}

#endif // MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD
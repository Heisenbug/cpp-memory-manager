#ifndef MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD
#define MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD

class AllocatorInterface;
namespace MM
{
	class TrackableChunkHolderInterface
	{
	public:

		//every lowlevel concrete allocator MUST implement this interface. 
		//owner is a cross-layer reference to the AllocatorInterface.
		//every concrete class implementing this interface MUST use the owner reference while registering allocated MMChunkInterface's to the MMAllocationTable 
		virtual void setOwner(AllocatorInterface * owner) = 0;
		
	};
}

#endif // MMTRACKABLECHUNKHOLDERINTERFACE_H_INCLUDE_GUARD
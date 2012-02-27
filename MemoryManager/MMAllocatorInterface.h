#ifndef MMALLOCATORINTERFACE_H_INCLUDE_GUARD
#define MMALLOCATORINTERFACE_H_INCLUDE_GUARD

namespace MM
{
	class AllocatorInterface
	{
	public:

		//virtual void* AllocateBytes(size_t)	= 0;
		//virtual void DeallocateBytes(void*) = 0;

		virtual void* Allocate(size_t) = 0;
		virtual void Deallocate(void*) = 0;

		virtual ~AllocatorInterface() { }
	};
}

#endif // MMALLOCATORINTERFACE_H_INCLUDE_GUARD
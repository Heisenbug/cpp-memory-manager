#ifndef GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
#define GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD

#include <memory>

namespace MM
{
	class GenericObjectAllocator
	{
	public:

		static void* AllocateBytes(size_t size)
		{
			return malloc(size);
		}

		static void DeallocateBytes(void* p)
		{
			free(p);
		}
	}; 
}

#endif // GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
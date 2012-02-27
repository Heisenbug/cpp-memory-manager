#ifndef GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
#define GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD

#include <memory>

namespace MM
{
	class GenericObjectAllocator
	{
	public:

		static void* Allocate(size_t size)
		{
			return malloc(size);
		}

		static void Deallocate(void* p)
		{
			free(p);
		}
	}; 
}

#endif // GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
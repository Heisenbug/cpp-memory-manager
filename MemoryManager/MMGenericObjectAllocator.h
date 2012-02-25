#ifndef GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
#define GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD

#include <memory>

namespace MM
{
	class GenericObjectAllocator
	{
	public:

		static void* operator new(size_t size)
		{
			return malloc(size);
		}

		static void operator delete(void * p)
		{
			free (p);
		}
	}; 
}

#endif // GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
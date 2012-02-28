#ifndef GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
#define GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD

#include "MMGenericObjectAlloc.h"
#include "MMSingletonHolder.h"

#ifndef DEFAULT_GENERICALLOCATOR_CHUNK_SIZE
#define DEFAULT_GENERICALLOCATOR_CHUNK_SIZE 32768
#endif

namespace MM
{
	class GenericObjectAllocator
	{
	public:

		static void* Allocate(size_t size)
		{
			return SingletonHolder<GenericObjectAllocatorType>::Instance().Allocate(size);
		}

		static void Deallocate(void* p)
		{
			return SingletonHolder<GenericObjectAllocatorType>::Instance().Deallocate(p);
		}

		static size_t GetChunkSize()
		{
			return DEFAULT_GENERICALLOCATOR_CHUNK_SIZE;
		}

	private:

		struct GenericObjectAllocatorType : public GenericObjectAlloc
		{
			GenericObjectAllocatorType()
				: GenericObjectAlloc(DEFAULT_GENERICALLOCATOR_CHUNK_SIZE) { }
		};
	}; 
}

#endif // GENERICOBJECTALLOCATOR_H_INCLUDE_GUARD
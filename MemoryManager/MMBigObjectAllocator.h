#pragma once

#include "MMSingletonHolder.h"

namespace MM
{
	class BigObjectAllocator
	{
#define MAX_BIG_OBJECT_SIZE 4096
	public:

		static void* Allocate(size_t size)
		{
			return SingletonHolder<BigObjectAllocator>::Instance().Allocate(size);
		}

		static void Deallocate(void * p)
		{
			return SingletonHolder<BigObjectAllocator>::Instance().Deallocate(p);
		}
		static size_t GetMaxSmallObjectSize()
		{
			return MAX_BIG_OBJECT_SIZE;
		}
	private:

		
	}; 
}

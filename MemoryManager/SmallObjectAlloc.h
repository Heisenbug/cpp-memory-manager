#pragma once
#include "SmallObjectAllocator.h"
#include "SingletonHolder.h"


namespace MM
{
#define DEFAULT_CHUNK_SIZE 4096
#define MAX_SMALL_OBJECT_SIZE 256

class SmallObjectAlloc{

public:
	static void * operator new(size_t size){
		return SingletonHolder<SmallObjectAllocatorType>::Instance().Allocate(size);
	}
	static void operator delete(void * p, size_t size){
		return SingletonHolder<SmallObjectAllocatorType>::Instance().Deallocate(p,size);
	}

private:

	struct SmallObjectAllocatorType : public SmallObjectAllocator{
		SmallObjectAllocatorType()
			: SmallObjectAllocator(DEFAULT_CHUNK_SIZE,MAX_SMALL_OBJECT_SIZE){
		}
	};
}; 

}
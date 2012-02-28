#include <iostream>

#include "MemoryManager.h"

int main()
{
	void* p = MM_MALLOC<MM::MEMCATEGORY_GENERAL>(256);
	void* q = MM_MALLOC<MM::MEMCATEGORY_GENERAL>(4096);

	MM_FREE(p);
	MM_FREE(q);
	return 0;
}
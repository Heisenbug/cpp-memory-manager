#include <iostream>

#include "MemoryManager.h"

int main()
{
	void* p = MM_MALLOC<MM::MEMCATEGORY_GENERAL>(256);

	MM_FREE(p);

	return 0;
}
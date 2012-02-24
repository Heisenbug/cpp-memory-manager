#pragma once

#include <map>

class AllocationTable{

public:
	static void registerChunk(void * alloc, void * chunk, size_t size);
};

class AllocTable
{
public:


private:

	std::map<std:ptrdiff, void *> allocTable; 

}
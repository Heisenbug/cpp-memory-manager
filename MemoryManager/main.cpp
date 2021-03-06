#include "MemoryManager.h"
#include "MMStackAllocator.h"
#include "MMSTLAllocator.h"

#include "Timer.h"

#include <iostream>

struct AllocationTestStruct
{
	int		x;
	float	y;
};

void AllocationTest()
{
	Timer t;

	const size_t N = 1000;
	const size_t M = 1000;

	const size_t MAX_EXPONENT = 10;
	
	void* v[N];
	for (size_t i = 0; i < N; ++i)
	{
		v[i] = 0;
	}

	t.Start();

	for (size_t j = 0; j < M ; ++j)
	{
		const int r = rand() % N;
		//std::cout << j << ": Try to access at v[" << r << "]...";
		if (v[r])
		{
			//std::cout << "Deleting...";
			delete(v[r]);
			v[r] = 0;
		}
		else
		{
			//std::cout << "Allocating...";
			v[r] = new AllocationTestStruct;
		}
	}

	LONGLONG clock1 = t.TimeElapsedMicroSec();

	for (size_t i = 0; i < N; ++i)
	{
		v[i] = 0;
	}

	t.Start();

	for (size_t j = 0; j < M; ++j)
	{
		const int r = rand() % N;
		//std::cout << j << ": Try to access at v[" << r << "]...";
		if(v[r])
		{
			//std::cout << "Deleting." << std::endl;
			MM_FREE(v[r]);
			v[r] = 0;
		}
		else
		{
			size_t randomSize = 1;
			size_t exponent = 1 + rand() % MAX_EXPONENT; 

			randomSize <<= exponent;

			//std::cout << "Allocating (" << randomSize << ")." << std::endl;
			//v[r] = MM_MALLOC(randomSize, MM::MEMCATEGORY_GENERAL);
			
			v[r] = MM_MALLOC(sizeof(AllocationTestStruct), MM::MEMCATEGORY_GENERAL);
		}
	}

	LONGLONG clock2 = t.TimeElapsedMicroSec();

	std::cout << "first: " << clock1 << " - second: " << clock2 << std::endl;

	for (size_t i = 0; i < N; ++i)
	{
		if (v[i]) MM_FREE(v[i]);
	}
}

void AlignedAllocationTest()
{
	void* p = MM_MALLOC_ALIGN(32, MM::MEMCATEGORY_GENERAL, 16);

	MM_FREE_ALIGN(p, MM::MEMCATEGORY_GENERAL, 16);
}

int main()
{
	InitMemoryManager();

	// Insert your code here!

	return 0;
}
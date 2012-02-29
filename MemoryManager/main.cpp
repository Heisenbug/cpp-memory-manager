#include "MemoryManager.h"

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
	
	AllocationTestStruct* v[N];
	for (size_t i = 0; i < N; ++i)
	{
		v[i] = 0;
	}

	t.Start();

	//for (size_t j = 0; j < M ; ++j)
	//{
	//	const int r = rand() % N;
	//	std::cout << j << ": Try to access at v[" << r << "]...";
	//	if (v[r])
	//	{
	//		std::cout << "Deleting...";
	//		delete(v[r]);
	//		v[r] = 0;
	//	}
	//	else
	//	{
	//		std::cout << "Allocating...";
	//		v[r] = new AllocationTestStruct;
	//	}
	//	std::cout << "OK." << std::endl;
	//}

	LONGLONG clock1 = t.TimeElapsedMicroSec();

	for (size_t i = 0; i < N; ++i)
	{
		v[i] = 0;
	}

	t.Start();

	for (size_t j = 0; j < M; ++j)
	{
		const int r = rand() % N;
		std::cout << j << ": Try to access at v[" << r << "]...";
		if(v[r])
		{
			std::cout << "Deleting...";
			MM_FREE(v[r]);
			v[r] = 0;
		}
		else
		{
			std::cout << "Allocating...";
			v[r] = static_cast<AllocationTestStruct*>(MM_MALLOC<MM::MEMCATEGORY_GENERAL>(sizeof(AllocationTestStruct)));
		}
		std::cout << "OK." << std::endl;
	}

	LONGLONG clock2 = t.TimeElapsedMicroSec();

	std::cout << "first: " << clock1 << " - second: " << clock2 << std::endl;

	for (size_t i = 0; i < N; ++i)
	{
		if (v[i]) 
			MM_FREE(v[i]);

		v[i] = 0;
	}
}

int main()
{
	AllocationTest();

	return 0;
}
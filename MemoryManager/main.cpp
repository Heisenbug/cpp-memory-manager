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

	for (size_t j = 0; j < M ; ++j)
	{
		const int r = rand() % N;
		std::cout << j << ": Try to access at v[" << r << "]...";
		if (v[r])
		{
			std::cout << "Deleting...";
			delete(v[r]);
			v[r] = 0;
		}
		else
		{
			std::cout << "Allocating...";
			v[r] = new AllocationTestStruct;
		}
		std::cout << "OK." << std::endl;
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
		std::cout << j << ": Try to access at v[" << r << "]...";
		if(v[r])
		{
			std::cout << "Deleting... " << r;
			MM_FREE(v[r]);
			v[r] = 0;
		}
		else
		{
			std::cout << "Allocating..." << r;
			v[r] = static_cast<AllocationTestStruct*>(MM_MALLOC<MM::MEMCATEGORY_GENERAL>(573));
		}
		const size_t MAX_EXPONENT = 10;
		size_t randomSize = 1;
		size_t exponent  = 1 + rand() % MAX_EXPONENT;
		randomSize = 1 + (rand() * 213) % 2048 ;//<<= exponent;
		std::cout << "Allocating(" << randomSize << ")...";
		v[r] = static_cast<AllocationTestStruct*>(MM_MALLOC<MM::MEMCATEGORY_GENERAL>(802));


		std::cout << "OK." << std::endl;
	}

	LONGLONG clock2 = t.TimeElapsedMicroSec();

	std::cout << "first: " << clock1 << " - second: " << clock2 << std::endl;
}

static void test1()
{
#define NUM 20
	void * pointers[NUM];

	for (int i = 0 ; i < NUM ; ++i){
		std::cout << "i " << i <<std::endl;
		pointers[i] = MM_MALLOC<MM::MEMCATEGORY_GENERAL>( 500 );//(i * 7) % 2048 + 1);
	}

	if (true){
		std::cout << "allocation works"<<std::endl;

	}
	std::cout << "------------------------------------------------"<<std::endl;

	for (int i = NUM - 1 ; i >= 0 ; --i){
		std::cout << "i " << i <<std::endl;
		
		
		MM_FREE(pointers[i]);
	}
}
int main()
{
	AllocationTest();
//	test1();
	return 0;
}
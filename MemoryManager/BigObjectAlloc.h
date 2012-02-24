#pragma once

#include <memory>

namespace MM
{

class SmallObjectAlloc{

public:
	static void * operator new(size_t size){
		return malloc(size);
	}
	static void operator delete(void * p, size_t size){
		free (p);
	}

private:


}; 

}
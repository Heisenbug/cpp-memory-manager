#ifndef INTERFACES_H_INCLUDE_GUARD
#define INTERFACES_H_INCLUDE_GUARD

struct ChunkInterface { };

class AllocatorInterface
{
public:

	virtual void* Allocate(size_t) = 0;
	virtual void Deallocate(void*) = 0;
};

#endif // INTERFACES_H_INCLUDE_GUARD
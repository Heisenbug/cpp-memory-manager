#ifndef MMALLOCATORINTERFACE_H_INCLUDE_GUARD
#define MMALLOCATORINTERFACE_H_INCLUDE_GUARD

namespace MM
{
	class AllocatorInterface
	{
	public:

		virtual void* operator new(size_t)	= 0;
		virtual void operator delete(void*) = 0;
	};
}

#endif // MMALLOCATORINTERFACE_H_INCLUDE_GUARD
#ifndef MMALLOCATEDOBJECT_H_INCLUDE_GUARD
#define MMALLOCATEDOBJECT_H_INCLUDE_GUARD

namespace MM
{
	template <class AllocationPolicy>
	class AllocatedObject
	{
	public:
		explicit AllocatedObject() { }

		~AllocatedObject() { }

		// operator new, with debug line info
		void* operator new(size_t sz, const char* file, int line, const char* func)
		{
			return AllocationPolicy::AllocateBytes(sz, file, line, func);
		}

		void* operator new(size_t sz)
		{
			return AllocationPolicy::AllocateBytes(sz);
		}

		// Placement operator new
		void* operator new(size_t sz, void* p)
		{
			(void) sz;
			return p;
		}

		// Array operator new, with debug line info
		void* operator new[](size_t sz, const char* file, int line, const char* func)
		{
			return AllocationPolicy::AllocateBytes(sz, file, line, func);
		}

		void* operator new[] (size_t sz)
		{
			return AllocationPolicy::AllocateBytes(sz);
		}

		void operator delete(void* p)
		{
			AllocationPolicy::DeallocateBytes(p);
		}

		// Corresponding operator for placement delete
		void operator delete(void* p, void*)
		{
			AllocationPolicy::DeallocateBytes(p);
		}

		// Only called if there is an exception in corresponding 'new'
		void operator delete(void* p, const char* , int , const char*)
		{
			AllocationPolicy::DeallocateBytes(p);
		}

		void operator delete[](void* p)
		{
			AllocationPolicy::DeallocateBytes(p);
		}

		void operator delete[](void* ptr, const char* , int , const char*)
		{
			AllocationPolicy::DeallocateBytes(ptr);
		}
	};
}

#endif // MMALLOCATEDOBJECT_H_INCLUDE_GUARD
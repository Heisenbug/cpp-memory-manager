#ifndef MMSTLALLOCATOR_H_INCLUDE_GUARD
#define MMSTLALLOCATOR_H_INCLUDE_GUARD

#include <limits>

#include "MemoryManager.h"

namespace MM
{
	template<typename T, typename Category = MEMCATEGORY_GENERAL>
	class STLAllocator 
	{
	public : 

		#pragma region Types

		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;

		template<typename U>
		struct rebind 
		{
			typedef STLAllocator<U, Category> other;
		};

		#pragma endregion

		#pragma region Constructors and destructor

		inline explicit STLAllocator() { }
   
		inline STLAllocator(const STLAllocator& other) { }

		template<typename U, typename C>
		inline STLAllocator(const STLAllocator<U, C>& other) { }

		inline ~STLAllocator() { }

		#pragma endregion

		#pragma region Address

		inline pointer address(reference r) { return &r; }
		inline const_pointer address(const_reference r) { return &r; }

		#pragma endregion

		#pragma region Memory allocation/deallocation

		inline pointer allocate(size_type cnt, const void* = 0) 
		{ 
			void* p = MM_MALLOC(sizeof(value_type) * cnt, Category);
			return reinterpret_cast<pointer>(p);
		}

		inline void deallocate(pointer p, size_type) 
		{ 
			MM_FREE_CAT(p, Category); // Or, simply: MM_FREE(p);
		}

		#pragma endregion

		#pragma region Construction/destruction

		inline void construct(pointer p, const value_type& t) 
		{
			new ( (void*)p ) value_type(t);
		}

		inline void destroy(pointer p) {
			p->~T();
		}

		#pragma endregion

		#pragma region Utilities

		inline size_type max_size() const 
		{ 
			return std::numeric_limits<size_type>::max();
		}

		#pragma endregion

		#pragma region Comparsion operators

		inline bool operator==(STLAllocator const& other) { return true;  }
		inline bool operator!=(STLAllocator const& other) { return false; }

		#pragma endregion

	private:

		// Private operator=
		STLAllocator& operator=(const STLAllocator&);
	};     
}

#endif // MMSTLALLOCATOR_H_INCLUDE_GUARD
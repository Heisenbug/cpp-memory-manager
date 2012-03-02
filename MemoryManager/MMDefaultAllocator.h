#pragma once

#include <limits>
#include <bitset>
#include <cassert>
#include <cstddef>

#include "MMConfig.h"
#include "MemoryManager.h"

namespace MM{



template<typename T, Category = MEMCATEGORY_GENERAL>
class MMDefaultAllocator {
public : 
    //    typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

	template <class U>
	struct rebind {
        typedef MMDefaultAllocator<U> other;
    };


	inline explicit MMDefaultAllocator(){
	}
    inline ~MMDefaultAllocator() 
	{
	
	}
    inline explicit MMDefaultAllocator(std::allocator<value_type> const& other) {
		
		
	}
    template<typename U>
    inline explicit MMDefaultAllocator(MMDefaultAllocator<U> const& other) 
	{
	
	}

    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    inline pointer allocate(size_type cnt, 
    typename std::allocator<void>::const_pointer = 0) { 
		 return reinterpret_cast<value_type> (MM_MALLOC(sizeof(value_type * cnt), Category));
    }
    inline void deallocate(pointer p, size_type) { 

		MM_FREE(p);//mmm..it's smarter than std::allocator, only need the pointer ;)
    }

    
    inline size_type max_size() const { 
		return MM::MAX_ALLOCATION_SIZE;
	}

    //    construction/destruction
    inline void construct(pointer p, const T& t) 
	{
		*p = value_type(t);
	}
    inline void destroy(pointer p) {
		p->~T();
	}

    inline bool operator==(MMDefaultAllocator const& other) 
	{ 
		return (*this == other)?true:false;
	}
    inline bool operator!=(MMDefaultAllocator const& a) 
	{
		return !operator==(a); 
	
	}
};     

}
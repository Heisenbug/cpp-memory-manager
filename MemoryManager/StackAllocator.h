#pragma once

#include <limits>
#include <bitset>
#include <cassert>
#include <cstddef>

namespace MM{

#define STACK_ALLOCATOR_DEFAULT_SLICE_NUM 50

template<typename T, size_t SLICE_NUM = STACK_ALLOCATOR_DEFAULT_SLICE_NUM>
class StackAllocator {
public : 
    //    typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

	static const size_t bufferSize = sizeof(typename T) * SLICE_NUM;
	unsigned int buffer[bufferSize];
	size_t freeIndex;
	
	//template<typename U>
   /* struct rebind {
        typedef StackAllocator<U> other;
    };*/
	template <class U>
	struct rebind {
        typedef StackAllocator<U> other;
    };


	inline explicit StackAllocator(){
		freeIndex = 0;
	}
    inline ~StackAllocator() 
	{
		int x;
		/*for (int i = 0 ; i < freeIndex ; ++i){
			pointer toDestruct = reinterpret_cast<pointer>(&buffer[i]);
			toDestruct->~T();
		*/
	}
    inline explicit StackAllocator(std::allocator<value_type> const& other) {
		
		assert(other.bufferSize <= this->bufferSize);

		this->freeIndex = other.freeIndex;
	//	memcpy(this->buffer, other.buffer, other.freeIndex);
		
	}
    template<typename U>
    inline explicit StackAllocator(StackAllocator<U> const& other) 
	{
		this->freeIndex = other.freeIndex;//int x;//???? other type ???? what ??this->val = other.val;
	}

    //    address?????????
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //    memory allocation
    inline pointer allocate(size_type cnt, 
       typename std::allocator<void>::const_pointer = 0) { 
			
		   //assert((bufferSize - freeIndex) + (cnt * sizeof(value_type)) <= bufferSize);
		   
		   size_t firstIndex = freeIndex;
		   freeIndex += cnt;

		   return reinterpret_cast<pointer>(&buffer[firstIndex]);
		
    }
    inline void deallocate(pointer p, size_type) { 
        //do nothing...
		int x;
    }

    
    inline size_type max_size() const { 
		return this->bufferSize ;
	}

    //    construction/destruction
    inline void construct(pointer p, const T& t) 
	{
		*p = T(t); 
	}
    inline void destroy(pointer p) { 
	
		//do nothing
		p->~T();
	}

    inline bool operator==(StackAllocator const& other) 
	{ 
		return (*this == other)?true:false;
	}
    inline bool operator!=(StackAllocator const& a) 
	{
		return !operator==(a); 
	
	}
};     

}
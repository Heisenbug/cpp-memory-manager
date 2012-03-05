#ifndef MMSTACKALLOCATOR_H_INCLUDE_GUARD
#define MMSTACKALLOCATOR_H_INCLUDE_GUARD

#include "MMPreprocDirectives.h"

#include <cassert>

namespace MM
{
	const size_t BUFFER_SIZE = STACKALLOCATOR_DEFAULT_BUFFER_SIZE;

	// This is an STL allocator that allocates bytes on the stack.
	// When you use it in order to allocate a container, we suggest 
	// you to use a reserve immediately after its creation. 
	// Subsequent reallocation quickly consume the dedicated stack-space
	template<typename T>//, size_t BUFFER_SIZE = STACKALLOCATOR_DEFAULT_BUFFER_SIZE>
	class StackAllocator 
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
		typedef StackAllocator<U> other;
	};

	#pragma endregion

	#pragma region Constructors and destructor

	inline  StackAllocator()
		: mBufferSize(0), mMarker(0) {
		mMarkerPtr = NULL;
		this->bufferPtr = NULL;
	}
	inline explicit StackAllocator(unsigned char* buf, size_t size)
		: mBufferSize(size), mMarker(0) 
	{
		std::cout << "New allocator<" << typeid(T).name() << ", " << BUFFER_SIZE << ">: " << mBufferSize << " elements\n";
		mMarkerPtr = &mMarker;
		this->bufferPtr = buf;
	}

	inline StackAllocator(const StackAllocator& other)
		: mBufferSize(STACKALLOCATOR_DEFAULT_BUFFER_SIZE), mMarker(0) 
	{ 
		mMarkerPtr = other.mMarkerPtr;
		this->bufferPtr = other.bufferPtr;
	}

	template<typename U>//, size_t S>
	inline StackAllocator(const StackAllocator<U>& other) 
		: mBufferSize(STACKALLOCATOR_DEFAULT_BUFFER_SIZE), mMarker(0) 
	{
		std::cout << "New (copied) allocator<" << typeid(T).name() << ", " << BUFFER_SIZE << ">: " << mBufferSize << " elements\n";
		mMarkerPtr = other.mMarkerPtr;
		this->bufferPtr = other.bufferPtr;
	}

	inline ~StackAllocator() { }

	#pragma endregion

	#pragma region Address

	inline pointer address(reference r) 
	{ 
		return &r; 
	}

	inline const_pointer address(const_reference r) 
	{ 
		return &r; 
	}

	#pragma endregion

	#pragma region Memory allocation/deallocation

	inline pointer allocate(size_type cnt, const void* = 0) 
	{ 		
		std::cout << "<" << typeid(T).name() << ", " << BUFFER_SIZE << "> Try to allocate " << cnt << " elements [" << *mMarkerPtr << ", " << mBufferSize * sizeof(value_type) << "\n";
		assert(*mMarkerPtr + cnt * sizeof(value_type) <= mBufferSize * sizeof(value_type));
		size_t availableIndex = *mMarkerPtr;
		*mMarkerPtr += cnt * sizeof(value_type);
		std::cout << "New mMarker: " <<*mMarkerPtr << std::endl;

	//	return reinterpret_cast<pointer>(&mBuffer[availableIndex]);
		return reinterpret_cast<pointer>(&bufferPtr[availableIndex]);
	}

	inline void deallocate(pointer p, size_type) 
	{
		// Nothing to do :D
	}
	
	#pragma endregion

	#pragma region Construction/destruction

	inline void construct(pointer p, const value_type& v) 
	{
		new ( (void*)p ) T(v); 
	}

	inline void destroy(pointer p) 
	{ 
		p->~T();
	}

	#pragma endregion

	#pragma region Utilities

	inline size_type max_size() const 
	{ 
		return sizeof(value_type) * (mBufferSize - *mMarkerPtr);
	}

	inline size_type size() const
	{
		return mBufferSize;
	}

	#pragma endregion

	#pragma region Comparsion operators

	inline bool operator==(StackAllocator const& other) const { return true;  }

	inline bool operator!=(StackAllocator const& other) const { return false; }

	#pragma endregion

	size_t *mMarkerPtr;
	unsigned char* bufferPtr;
	private:

		//const size_t	mBufferSize = sizeof(value_type) * BUFFER_SIZE;
		unsigned char	mBuffer[sizeof(value_type) * BUFFER_SIZE];

		// Number of elements storable into the buffer
		size_t			mBufferSize;

		// Index to the first available block of the buffer
		size_t			mMarker;
		

		//template<typename U>
		//friend class StackAllocator<U>;
	};     
}

#endif
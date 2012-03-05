#ifndef REFLINKEDPTR_H_INCLUDE_GUARD
#define REFLINKEDPTR_H_INCLUDE_GUARD

namespace MM
{
	template<typename T>
	class RefLinkedPtr
	{
	public:

		typedef T	value_type;
		typedef T*	pointee_type;
		typedef T*	pointer;
		typedef T&	reference;

		explicit RefLinkedPtr(pointer p = 0)
			: mPointee(p)
		{
			mPrev = mNext = this;
		}

		RefLinkedPtr(const RefLinkedPtr& ptr)
		{
			Acquire(ptr);
		}

		RefLinkedPtr& operator=(const RefLinkedPtr& ptr)
		{
			if (this != &ptr)
			{
				Release();
				Acquire(ptr);
			}

			return *this;
		}

		~RefLinkedPtr()
		{
			Release();
		}

		template<typename U> friend class RefLinkedPtr;

		template<typename U> 
		RefLinkedPtr(const RefLinkedPtr<U>& ptr)
		{
			Acquire(ptr);
		}

		template<typename U> 
		RefLinkedPtr& operator=(const RefLinkedPtr<U>& ptr)
		{
			if (this != &ptr) {
				Release();
				Acquire(ptr);
			}

			return *this;
		}

		reference operator*() const 
		{
			return *mPointee;
		}

		pointer operator->() const 
		{
			return mPointee;
		}

		pointer Get() const 
		{
			return mPointee;
		}

		bool IsUnique() const
		{
			return (mPointee ? mPrev == this : true);
		}

	private:

		void Acquire(const RefLinkedPtr& ptr)
		{ 
			mPointee		= ptr.mPointee;
			mNext			= ptr.mNext;
			mNext->mPrev	= this;
			mPrev			= &ptr;
			ptr.mNext		= this;
		}

		template<typename U> 
		void Acquire(const RefLinkedPtr<U>& ptr)
		{ 
			mPointee		= ptr.mPointee;
			mNext			= ptr.mNext;
			mNext->mPrev	= this;
			mPrev			= &ptr;
			ptr.mNext		= this;
		}

		void Release()
		{ 
			if (IsUnique()) 
			{ 
				MM_DELETE_T(mPointee, T);
				//delete mPointee;
			}
			else 
			{
				mPrev->mNext = mNext
				mNext->mPrev = mPrev;
				mPrev = mNext = 0;
			}

			mPointee = 0;
		}

		pointee_type				mPointee;
		mutable const RefLinkedPtr* mPrev;
		mutable const RefLinkedPtr* mNext;
	};
}

#endif // REFLINKEDPTR_H_INCLUDE_GUARD
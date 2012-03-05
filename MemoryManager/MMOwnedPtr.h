#ifndef MMOWNEDPTR_H_INCLUDE_GUARD
#define MMOWNEDPTR_H_INCLUDE_GUARD

namespace MM
{
	template<typename T>
	class OwnedPtr
	{
	public:

		typedef T	value_type;
		typedef T*	pointee_type;
		typedef T*	pointer;
		typedef T&	reference;

		explicit OwnedPtr(pointer p = 0) 
			: mOwn(p != 0), mPointee(p) { }

		OwnedPtr(const OwnedPtr& ptr)
			: mOwn(ptr.mOwn), mPointee(ptr.Release()) { }

		OwnedPtr& operator=(const OwnedPtr& ptr)
		{
			if (this != &ptr)
			{
				if (mPointee != ptr.mPointee)
				{
					if (mOwn) delete mPointee;
					mOwn = ptr.mOwn;
				}
				else if (ptr.mOwn) mOwn = true;

				mPointee = ptr.Release();
			}

			return *this;
		}

		~OwnedPtr() 
		{
			if (mOwn) delete mPointee;
		}

		template<typename U> friend class OwnedPtr;

		template<typename U> 
		OwnedPtr(const OwnedPtr<U>& ptr)
			: mOwn(ptr.mOwn), mPointee(ptr.Release()) { }

		template<typename U> 
		OwnedPtr& operator=(const OwnedPtr<U>& ptr)
		{
			if (this != &ptr) {
				if (mPointee != ptr.mPointee) 
				{
					if (mOwn) delete mPointee;
					mOwn = ptr.mOwn;
				}
				else if (ptr.mOwn) mOwn = true;

				mPointee = ptr.Release();
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

		pointer Release() const
		{
			mOwn = false; 
			return mPointee;
		}

	private:

		mutable bool mOwn;
		pointee_type mPointee;	
	};
}

#endif // MMOWNEDPTR_H_INCLUDE_GUARD
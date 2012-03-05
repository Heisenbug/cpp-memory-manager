#ifndef MMREFCOUNTEDPTR_H_INCLUDE_GUARD
#define MMREFCOUNTEDPTR_H_INCLUDE_GUARD

namespace MM
{
	template<typename T>
	class RefCountedPtr
	{
	public:

		typedef T	value_type;
		typedef T*	pointee_type;
		typedef T*	pointer;
		typedef T&	reference;

		explicit RefCountedPtr(pointer p = 0)
			: mRCPImpl(p ? new RCPImpl(p) : 0) { }

		RefCountedPtr(const RefCountedPtr& ptr) 
		{
			Acquire(ptr.mRCPImpl);
		}

		RefCountedPtr& operator=(const RefCountedPtr& ptr)
		{
			if (this != &ptr)
			{
				Release();
				Acquire(ptr.mRCPImpl);
			}

			return *this;
		}

		~RefCountedPtr()
		{
			Release();
		}

		template<typename U> friend class RefCountedPtr;

		template<typename U> 
		RefCountedPtr(const RefCountedPtr<U>& ptr)
		{
			Acquire(ptr.mRCPImpl);
		}

		template<typename U> 
		RefCountedPtr& operator=(const RefCountedPtr<U>& ptr)
		{
			if (this != &ptr) {
				Release();
				Acquire(ptr.mRCPImpl);
			}

			return *this;
		}

		reference operator*() const 
		{
			return *mRCPImpl->mPointee;
		}

		pointer operator->() const 
		{
			return mRCPImpl->mPointee;
		}

		pointer Get() const 
		{
			return mRCPImpl ? mRCPImpl->mPointee : 0;
		}

		bool IsUnique() const 
		{
			return (mRCPImpl ? mRCPImpl->mCounter == 1 : true);
		}

	private:

		struct RCPImpl 
		{
			RCPImpl(pointer p = 0, unsigned int c = 1) 
				: mPointee(p), mCounter(c) { }

			pointee_type	mPointee;
			unsigned		mCounter;
		}* mRCPImpl;

		// Increment the counter
		void Acquire(RCPImpl* impl)
		{ 
			mRCPImpl = impl;
			if (impl) ++impl->mCounter;
		}

		// Decrement the counter, delete if it is 0
		void Release()
		{
			if (mRCPImpl) {
				if (--mRCPImpl->mCounter == 0) {
					delete mRCPImpl->mPointee;
					delete mRCPImpl;
				}

				mRCPImpl = 0;
			}
		}
	};
}

#endif // MMREFCOUNTEDPTR_H_INCLUDE_GUARD
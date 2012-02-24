#ifndef COPYONWRITEPTR_H_INCLUDE_GUARD
#define COPYONWRITEPTR_H_INCLUDE_GUARD

#include "DeepCopyPtr.h"

namespace MM
{
	template<typename T>
	class CopyOnWritePtr
	{
	public:

		typedef T				value_type;
		typedef DeepCopyPtr<T>	pointee_type;
		typedef T*				pointer;
		typedef T&				reference;

		explicit CopyOnWritePtr(pointer p = 0)
			: mPointee(p) { }

		CopyOnWritePtr(const CopyOnWritePtr& ptr)
			: mPointee(ptr.mPointee) { }

		CopyOnWritePtr& operator=(const CopyOnWritePtr& ptr)
		{
			if (this != &ptr)
			{
				mPointee = ptr.mPointee;
			}

			return *this;
		}

		template<typename U> friend class CopyOnWritePtr<U>;

		template<typename U> 
		CopyOnWritePtr(const CopyOnWritePtr<U>& ptr)
			: mPointee(ptr.mPointee) { }

		template<typename U> 
		CopyOnWritePtr& operator=(const CopyOnWritePtr<U>& ptr)
		{
			if (this != &ptr)
			{
				mPointee = ptr.mPointee;
			}
	
			return *this;
		}

		const reference operator*() const 
		{
			return *mPointee;
		}

		const pointer operator->() const 
		{
			return itsPtr.get();
		}

		const pointer Get() const 
		{
			return mPointee.Get();
		}

		reference operator*()
		{
			Copy(); 
			return *mPointee;
		}
		
		pointer operator->() 
		{
			Copy(); 
			return mPointee.Get();
		}
		
		pointer Get() 
		{
			Copy(); 
			return mPointee.Get();
		}

	private:

		void Copy()
		{
			if (!mPointee.IsUnique()) 
			{
				pointer old = mPointee.Get();
				mPointee = DeepCopyPtr<T>(new T(*old));
			}
		}

		pointee_type mPointee;
	};
}

#endif // COPYONWRITEPTR_H_INCLUDE_GUARD
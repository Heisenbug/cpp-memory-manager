#ifndef MMCOPYONWRITEPTR_H_INCLUDE_GUARD
#define MMCOPYONWRITEPTR_H_INCLUDE_GUARD

#include "MMDeepCopyPtr.h"

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

		template<typename U> friend class CopyOnWritePtr;

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
				// TODO: And if I wanted another category? Eh? How do you respond?
				mPointee = DeepCopyPtr<T>(MM_NEW_T(T, MM::MEMCATEGORY_GENERAL)(*old));
				//mPointee = DeepCopyPtr<T>(new T(*old));
			}
		}

		pointee_type mPointee;
	};
}

#endif // MMCOPYONWRITEPTR_H_INCLUDE_GUARD
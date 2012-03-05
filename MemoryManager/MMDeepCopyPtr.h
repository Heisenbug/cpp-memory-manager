#ifndef MMDEEPCOPYPTR_H_INCLUDE_GUARD
#define MMDEEPCOPYPTR_H_INCLUDE_GUARD

namespace MM
{
	template<typename T>
	class DeepCopyPtr
	{
	public:

		typedef T	value_type;
		typedef T*	pointee_type;
		typedef T*	pointer;
		typedef T&	reference;

		explicit DeepCopyPtr(pointer p = 0)
			: mPointee(p) { }

		DeepCopyPtr(const DeepCopyPtr& ptr)
		{
			Copy(ptr.Get());
		}

		DeepCopyPtr& operator=(const DeepCopyPtr& ptr)
		{
			if (this != &ptr)
			{
				MM_DELETE_T(mPointee, T);
				//delete mPointee;
				Copy(ptr);
			}

			return *this;
		}

		~DeepCopyPtr()
		{
			MM_DELETE_T(mPointee, T);
			//delete mPointee;
		}

		reference operator*()  const 
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

	private:

		void Copy(const DeepCopyPtr& ptr)
		{
			// TODO: And if I wanted another category? Eh? How do you respond?
			mPointee = ptr->mPointee ? MM_NEW_T(T, MM::MEMCATEGORY_GENERAL)(*ptr.mPointee);
			//mPointee = ptr.mPointee ? new T(*ptr.mPointee) : 0;
		}

		pointee_type	mPointee;
	};
}

#endif // MMDEEPCOPYPTR_H_INCLUDE_GUARD
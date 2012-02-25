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
				delete mPointee;
				Copy(ptr);
			}

			return *this;
		}

		~DeepCopyPtr()
		{
			delete ptr;
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
			mPointee = ptr.mPointee ? new T(*ptr.mPointee) : 0;
		}

		pointee_type	mPointee;
	};
}

#endif // MMDEEPCOPYPTR_H_INCLUDE_GUARD
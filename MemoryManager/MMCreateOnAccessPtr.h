#ifndef MMCREATEONACCESSPTR_H_INCLUDE_GUARD
#define MMCREATEONACCESSPTR_H_INCLUDE_GUARD

namespace MM
{
	template<typename T>
	class CreateOnAccessPtr
	{
	public:

		typedef T	value_type;
		typedef T*	pointee_type;
		typedef T*	pointer;
		typedef T&	reference;

		CreateOnAccessPtr()
			: mPointee(0) { }

		~CreateOnAccessPtr() 
		{
			MM_DELETE_T(mPointee, T);
			//delete mPointee;
		}

		reference operator*() 
		{
			Create(); 
			return *mPointee;
		}

		pointer operator->() 
		{
			Create(); 
			return mPointee;
		}

		pointer Get() 
		{
			Create(); 
			return mPointee;
		}

	private:

		void Create() 
		{
			if (!mPointee) 
			{
				// TODO: And if I wanted another category? Eh? How do you respond?
				mPointee = MM_NEW_T(T, MM::MEMCATEGORY_GENERAL);
				//mPointee = new T;
			}
		}

		CreateOnAccessPtr(const CreateOnAccessPtr&);
		CreateOnAccessPtr& operator=(const CreateOnAccessPtr&);

		pointee_type mPointee;
	};
}

#endif // MMCREATEONACCESSPTR_H_INCLUDE_GUARD
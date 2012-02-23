template<typename T>
class DefaultStoragePolicy
{
public:

	DefaultStoragePolicy() 
		: mPointer(0) { }
	DefaultStoragePolicy(const value_type& p)
		: mPointer(p) { }
	
	reference operator->() const
	{

	}

private:

	typedef T*		value_type;
	typedef T*		pointer;
	typedef T&		reference;

	value_type mPointer;
};


template<
	typename T,
	template <class> class Ownership = RefCounted,
	typename ConversionPolicy = DisallowConversion,
	template <class> class CheckingPolicy = AssertCheck;
	template <class> class StoragePolicy = DefaultStoragePolicy
>
class SmartPointer;




template<typename T>
class SmartPointer
{
public:
	-
	explicit SmartPointer(T* pointer) 
		: mPointer(pointer) { }
	SmartPointer& operator=(const SmartPointer& sp);
	~SmartPointer();

	T& operator*() const 
	{
		// ...
		return *mPointer;
	}

	T* operator->() const 
	{
		// ...
		return mPointer;
	}

private:

	T* mPointer;
};
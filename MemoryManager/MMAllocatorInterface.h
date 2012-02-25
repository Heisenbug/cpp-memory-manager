#ifndef MMALLOCATORINTERFACE_H_INCLUDE_GUARD
#define MMALLOCATORINTERFACE_H_INCLUDE_GUARD

namespace MM
{
	class AllocatorInterface
	{
	public:
		//http://msdn.microsoft.com/it-it/library/zwc6783f.aspx
	//	virtual void* operator new(size_t)	= 0;
		void* operator new(size_t);
	//	virtual void operator delete(void*) = 0;
		void operator delete(void*);
	};
}

#endif // MMALLOCATORINTERFACE_H_INCLUDE_GUARD
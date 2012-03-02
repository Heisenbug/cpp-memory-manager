#ifndef MMLOCKPOLICIES_H_INCLUDE_GUARD
#define MMLOCKPOLICIES_H_INCLUDE_GUARD

namespace MM
{
	class NoLock
	{
	public:
		NoLock() 
		{
			// Nothing to do
		}

		~NoLock() 
		{
			// Nothing to do
		}
	};

	template<typename Host>
	class MutexLock
	{
	public:
		MutexLock() 
		{
			// TODO: Enter critical section
		}

		~MutexLock()
		{
			// TODO: Leave critial section 
		}
	};
}

#endif // MMLOCKPOLICIES_H_INCLUDE_GUARD
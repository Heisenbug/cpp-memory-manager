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

	class MutexLock
	{
	public:
		MutexLock() 
		{
			// TODO: Acquire mutex
		}

		~MutexLock()
		{
			// TODO: Release mutex
		}
	};
}

#endif // MMLOCKPOLICIES_H_INCLUDE_GUARD
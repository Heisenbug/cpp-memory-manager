#ifndef MMSINGLETONHOLDER_H_INCLUDE_GUARD
#define MMSINGLETONHOLDER_H_INCLUDE_GUARD

#include "MMLockPoliclies.h"

namespace MM
{
	template<typename T, typename LockPolicy = NoLock>
	class SingletonHolder
	{
	public:

		static T& Instance()
		{
			if (!mInstance)
			{
				LockPolicy lock;

				if (!mInstance)
				{
					mInstance = new T();
					atexit(DeleteInstance);
				}
			}

			return *mInstance;
		}

	private:

		static void DeleteInstance()
		{
			delete mInstance;
		} 

		static T* mInstance;
	};

	template<typename T, typename LockPolicy>
	T* SingletonHolder<T, LockPolicy>::mInstance = 0;
}

#endif // MMSINGLETONHOLDER_H_INCLUDE_GUARD


#pragma once


#include <Windows.h>

class NoLock{

public:
	NoLock(){
			
	}
	
	~NoLock(){
	
	}

private:

	
};

class Lock{

public:
	Lock(){
		//TODO: acquire mutex
	}
	
	~Lock(){
		//TODO: release mutex
	}

private:

	
};

template<typename T, typename LockPolicy = NoLock>
class SingletonHolder{

public:

	static T& Instance(){
		if (!instance){
			LockPolicy lock;
			if (!instance){
				instance = new T();
				atexit(DeleteInstance);
			}
		}
		return *instance;
	}
private:

	static void DeleteInstance(){
		delete instance;
	} 
	static T* instance;
};


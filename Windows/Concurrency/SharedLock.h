//==============
// SharedLock.h
//==============

#pragma once


//=======
// Using
//=======

#include "Mutex.h"


//===========
// Namespace
//===========

namespace Concurrency {


//=============
// Shared-Lock
//=============

class SharedLock: private std::unique_lock<std::mutex>
{
public:
	// Con-/Destructors
	inline SharedLock(Mutex& Mutex): unique_lock(Mutex) {}

	// Common
	inline VOID Lock() { lock(); }
	inline VOID Release() { release(); }
	inline VOID Unlock() { unlock(); }
};

}

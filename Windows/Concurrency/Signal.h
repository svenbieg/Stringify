//==========
// Signal.h
//==========

#pragma once


//=======
// Using
//=======

#include <condition_variable>
#include "ScopedLock.h"


//===========
// Namespace
//===========

namespace Concurrency {


//========
// Signal
//========

class Signal: private std::condition_variable
{
public:
	// Con-/Destructors
	inline Signal() {}

	// Common
	inline VOID Cancel() { throw NotImplementedException(); }
	inline VOID Trigger() { notify_all(); }
	inline BOOL Wait()
		{
		Mutex mutex;
		ScopedLock lock(mutex);
		wait(lock);
		return true;
		}
	inline BOOL Wait(ScopedLock& Lock) { wait(Lock); return true; }
	inline BOOL Wait(ScopedLock& Lock, UINT Timeout)
		{
		auto status=wait_for(Lock, std::chrono::milliseconds(Timeout));
		return (status==std::cv_status::no_timeout);
		}
};

}

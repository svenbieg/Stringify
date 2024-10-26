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
	inline VOID Broadcast() { notify_all(); }
	inline VOID Wait(ScopedLock& Lock) { wait(Lock); }
	inline BOOL Wait(ScopedLock& Lock, UINT Timeout)
		{
		auto status=wait_for(Lock, std::chrono::milliseconds(Timeout));
		return (status==std::cv_status::no_timeout);
		}
};

}

//=================
// CriticalMutex.h
//=================

#pragma once


//=======
// Using
//=======

#include "Mutex.h"


//===========
// Namespace
//===========

namespace Concurrency {


//================
// Critical-Mutex
//================

class CriticalMutex: public Mutex
{
public:
	// Friends
	friend ScopedLock;

	// Con-/Destructors
	CriticalMutex()=default;
	CriticalMutex(CriticalMutex const&)=delete;

	// Common
	inline VOID Lock()override { lock(); }
	inline VOID Lock(AccessMode Access)override { lock(); }
	inline BOOL TryLock()override { return try_lock(); }
	inline BOOL TryLock(AccessMode Access)override { return try_lock(); }
	inline VOID Unlock()override { unlock(); }
	inline VOID Unlock(AccessMode Access)override { unlock(); }
};

}
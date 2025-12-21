//==============
// ScopedLock.h
//==============

#pragma once


//=======
// Using
//=======

#include "Concurrency/Mutex.h"


//===========
// Namespace
//===========

namespace Concurrency {


//======================
// Forward-Declarations
//======================

class Signal;


//=============
// Scoped-Lock
//=============

class ScopedLock: protected std::unique_lock<std::mutex>
{
public:
	// Friends
	friend Signal;

	// Common
	inline VOID Lock() { lock(); }
	inline VOID Release() { release(); }
	inline BOOL TryLock() { return try_lock(); }
	inline VOID Unlock() { unlock(); }

protected:
	// Common
	ScopedLock(Mutex& Mutex): unique_lock(Mutex) {}
};

}
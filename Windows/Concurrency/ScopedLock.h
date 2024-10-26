//==============
// ScopedLock.h
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


//======================
// Forward-Declarations
//======================

class Signal;


//=============
// Scoped-Lock
//=============

class ScopedLock: private std::unique_lock<std::mutex>
{
public:
	// Friends
	friend Signal;

	// Con-/Destructors
	inline ScopedLock(Mutex& Mutex): unique_lock(Mutex) {}

	// Common
	inline VOID Lock() { lock(); }
	inline VOID Release() { release(); }
	inline VOID Unlock() { unlock(); }
};

}

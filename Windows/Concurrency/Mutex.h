//=========
// Mutex.h
//=========

#pragma once


//=======
// Using
//=======

#include <mutex>
#include "TypeHelper.h"


//===========
// Namespace
//===========

namespace Concurrency {


//======================
// Forward-Declarations
//======================

class ScopedLock;


//=============
// Access-Mode
//=============

enum class AccessMode
{
ReadOnly
};


//=======
// Mutex
//=======

class Mutex: protected std::mutex
{
public:
	// Friends
	friend ScopedLock;

	// Con-/Destructors
	Mutex() {}

	// Common
	inline VOID Lock() { lock(); }
	inline VOID Lock(AccessMode Access) { lock(); }
	inline BOOL TryLock() { return try_lock(); }
	inline BOOL TryLock(AccessMode Access) { return try_lock(); }
	inline VOID Unlock() { unlock(); }
	inline VOID Unlock(AccessMode Access) { unlock(); }
};

}
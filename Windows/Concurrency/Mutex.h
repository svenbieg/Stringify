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
	Mutex()=default;
	Mutex(Mutex const&)=delete;

	// Common
	virtual inline VOID Lock() { lock(); }
	virtual inline VOID Lock(AccessMode Access) { lock(); }
	virtual inline BOOL TryLock() { return try_lock(); }
	virtual inline BOOL TryLock(AccessMode Access) { return try_lock(); }
	virtual inline VOID Unlock() { unlock(); }
	virtual inline VOID Unlock(AccessMode Access) { unlock(); }
};

}
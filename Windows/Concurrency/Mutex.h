//=========
// Mutex.h
//=========

#pragma once


//=======
// Using
//=======

#include <mutex>


//===========
// Namespace
//===========

namespace Concurrency {


//======================
// Forward-Declarations
//======================

class ScopedLock;


//=======
// Mutex
//=======

class Mutex: protected std::mutex
{
public:
	// Friends
	friend ScopedLock;

	// Con-/Destructors
	inline Mutex() {}

	// Common
	virtual inline VOID Lock() { lock(); }
	virtual inline BOOL TryLock() { return try_lock(); }
	virtual inline VOID Unlock() { unlock(); }
};

}
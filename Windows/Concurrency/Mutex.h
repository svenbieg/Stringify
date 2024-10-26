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
class SharedLock;


//=======
// Mutex
//=======

class Mutex: protected std::mutex
{
public:
	// Friends
	friend ScopedLock;
	friend SharedLock;

	// Con-/Destructors
	inline Mutex() {}

	// Common
	virtual inline VOID Lock() { lock(); }
	virtual inline BOOL TryLock() { return try_lock(); }
	virtual inline VOID Unlock() { unlock(); }
};

}

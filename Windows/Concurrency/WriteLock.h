//=============
// WriteLock.h
//=============

#pragma once


//=======
// Using
//=======

#include "Concurrency/ScopedLock.h"


//===========
// Namespace
//===========

namespace Concurrency {


//============
// Write-Lock
//============

class WriteLock: public ScopedLock
{
public:
	// Con-/Destructors
	WriteLock(Mutex& Mutex): ScopedLock(Mutex) {}
};

}
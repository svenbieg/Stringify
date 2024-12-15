//==============
// SharedLock.h
//==============

#pragma once


//=======
// Using
//=======

#include "ScopedLock.h"


//===========
// Namespace
//===========

namespace Concurrency {


//=============
// Shared-Lock
//=============

class SharedLock: public ScopedLock
{
public:
	// Con-/Destructors
	inline SharedLock(Mutex& Mutex): ScopedLock(Mutex) {}
};

}

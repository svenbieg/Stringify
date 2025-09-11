//============
// ReadLock.h
//============

#pragma once


//=======
// Using
//=======

#include "ScopedLock.h"


//===========
// Namespace
//===========

namespace Concurrency {


//===========
// Read-Lock
//===========

class ReadLock: public ScopedLock
{
public:
	// Con-/Destructors
	ReadLock(Mutex& Mutex): ScopedLock(Mutex) {}
};

}
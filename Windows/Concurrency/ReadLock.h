//============
// ReadLock.h
//============

#pragma once


//=======
// Using
//=======

#include "Concurrency/ScopedLock.h"


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
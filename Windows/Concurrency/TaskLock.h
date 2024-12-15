//============
// TaskLock.h
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


//======================
// Forward-Declarations
//======================

class Signal;


//===========
// Task-Lock
//===========

class TaskLock: public ScopedLock
{
public:
	// Friends
	friend Signal;

	// Con-/Destructors
	inline TaskLock(Mutex& Mutex): ScopedLock(Mutex) {}
};

}

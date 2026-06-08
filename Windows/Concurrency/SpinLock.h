//============
// SpinLock.h
//============

#pragma once


//=======
// Using
//=======

#include "Concurrency/CriticalSection.h"
#include "Concurrency/ScopedLock.h"


//===========
// Namespace
//===========

namespace Concurrency {


//===========
// Spin-Lock
//===========

class SpinLock: public ScopedLock
{
public:
	// Con-/Destructors
	SpinLock(CriticalSection& CriticalSection): ScopedLock(CriticalSection) {}
};

}
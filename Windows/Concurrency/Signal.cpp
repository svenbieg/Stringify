//============
// Signal.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Signal.h"
#include "Task.h"


//===========
// Namespace
//===========

namespace Concurrency {


//========
// Common
//========

BOOL Signal::Wait()
{
Task::ThrowIfMain();
Mutex mutex;
ScopedLock lock(mutex);
wait(lock);
return true;
}

BOOL Signal::Wait(ScopedLock& Lock)
{
Task::ThrowIfMain();
wait(Lock);
return true;
}

BOOL Signal::Wait(ScopedLock& Lock, UINT Timeout)
{
Task::ThrowIfMain();
auto status=wait_for(Lock, std::chrono::milliseconds(Timeout));
return (status==std::cv_status::no_timeout);
}


//================
// Common Private
//================

BOOL Signal::WaitInternal()
{
Mutex mutex;
ScopedLock lock(mutex);
wait(lock);
return true;
}

}
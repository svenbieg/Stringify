//============
// Signal.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Signal.h"


//===========
// Namespace
//===========

namespace Concurrency {


//========
// Common
//========

BOOL Signal::Wait()
{
MainTask::ThrowIf();
Mutex mutex;
ScopedLock lock(mutex);
wait(lock);
return true;
}

BOOL Signal::Wait(ScopedLock& Lock)
{
MainTask::ThrowIf();
wait(Lock);
return true;
}

BOOL Signal::Wait(ScopedLock& Lock, UINT Timeout)
{
MainTask::ThrowIf();
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
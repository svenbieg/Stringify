//============
// Signal.cpp
//============

#include "Concurrency/Signal.h"


//=======
// Using
//=======

#include "Concurrency/Task.h"


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
WriteLock lock(mutex);
wait(lock);
return true;
}

BOOL Signal::Wait(ScopedLock& lock)
{
Task::ThrowIfMain();
wait(lock);
return true;
}

BOOL Signal::Wait(ScopedLock& lock, UINT timeout)
{
Task::ThrowIfMain();
auto status=wait_for(lock, std::chrono::milliseconds(timeout));
return (status==std::cv_status::no_timeout);
}


//================
// Common Private
//================

BOOL Signal::WaitInternal(ScopedLock& lock)
{
wait(lock);
return true;
}

}
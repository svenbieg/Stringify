//==============
// TaskHelper.h
//==============

#pragma once


//=======
// Using
//=======

#include <functional>
#include "Concurrency/ScopedLock.h"
#include "Concurrency/SharedLock.h"
#include "Concurrency/Signal.h"
#include "Concurrency/Task.h"


//========
// Common
//========

template <class _owner_t, class... _args_t> inline Handle<Concurrency::Task> CreateTask(_owner_t* Owner, VOID (_owner_t::*Procedure)(_args_t...), _args_t... Arguments)
{
return new Concurrency::Task([Owner, Procedure, Arguments...](){ (Owner->*Procedure)(Arguments...); });
}

Concurrency::Task* GetCurrentTask();

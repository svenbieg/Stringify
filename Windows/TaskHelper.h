//==============
// TaskHelper.h
//==============

#pragma once


//=======
// Using
//=======

#include <functional>
#include "TaskClass.h"


//========
// Common
//========

template <class _owner_t, class... _args_t> inline Handle<Task> CreateTask(_owner_t* Owner, VOID (_owner_t::*Procedure)(_args_t...), _args_t... Arguments)
{
return new Task([Owner, Procedure, Arguments...](){ (Owner->*Procedure)(Arguments...); });
}

Task* GetCurrentTask();

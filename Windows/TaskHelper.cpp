//================
// TaskHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "TaskHelper.h"

using namespace Concurrency;


//========
// Common
//========

Task* GetCurrentTask()
{
DWORD id=GetCurrentThreadId();
return Task::GetTask(id);
}

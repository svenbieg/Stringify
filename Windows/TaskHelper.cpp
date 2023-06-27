//================
// TaskHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "TaskHelper.h"


//========
// Common
//========

Task* GetCurrentTask()
{
DWORD id=GetCurrentThreadId();
return Task::GetTask(id);
}

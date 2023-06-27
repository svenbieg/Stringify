//===============
// TaskClass.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "TaskClass.h"

using namespace Core;


//==================
// Con-/Destructors
//==================

Task::Task(std::function<VOID()> proc):
Cancelled(false),
cProcedure(proc),
hThread(NULL),
uId(0)
{
hThread=CreateThread(nullptr, 0, TaskProc, this, CREATE_SUSPENDED, nullptr);
if(hThread==INVALID_HANDLE_VALUE)
	hThread=NULL;
if(!hThread)
	throw E_INVALIDARG;
uId=GetThreadId(hThread);
cTasks.add(uId, this);
ResumeThread(hThread);
}

Task::~Task()
{
Cancel();
}


//========
// Common
//========

VOID Task::Cancel()
{
ScopedLock lock(cMutex);
if(hThread==NULL)
	return;
Cancelled=true;
lock.Unlock();
Wait();
}

Task* Task::GetTask(DWORD id)
{
Task* task=nullptr;
cTasks.try_get(id, &task);
return task;
}

VOID Task::Wait()
{
ScopedLock lock(cMutex);
if(hThread==NULL)
	return;
auto task=GetCurrentTask();
if(!task)
	Application::Current->HandleDispatched();
cFinished.Wait(lock);
}


//================
// Common Private
//================

VOID Task::DoTask()
{
try
	{
	cProcedure();
	}
catch(std::exception&)
	{
	}
cTasks.remove(uId);
ScopedLock lock(cMutex);
cProcedure=nullptr;
CloseHandle(hThread);
hThread=NULL;
uId=0;
cFinished.Broadcast();
}

DWORD WINAPI Task::TaskProc(VOID* param)
{
auto task=(Task*)param;
task->DoTask();
return 0;
}

Clusters::shared_map<DWORD, Task*> Task::cTasks;

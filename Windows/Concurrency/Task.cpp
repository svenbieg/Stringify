//==========
// Task.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Task.h"

using namespace Core;


//===========
// Namespace
//===========

namespace Concurrency {


//==================
// Con-/Destructors
//==================

Task::Task(std::function<VOID()> proc):
Cancelled(false),
s_Id(0),
m_Procedure(proc),
m_Thread(NULL)
{
m_Thread=CreateThread(nullptr, 0, TaskProc, this, CREATE_SUSPENDED, nullptr);
if(m_Thread==INVALID_HANDLE_VALUE)
	m_Thread=NULL;
if(!m_Thread)
	throw E_INVALIDARG;
s_Id=GetThreadId(m_Thread);
m_Tasks.add(s_Id, this);
m_This=this;
ResumeThread(m_Thread);
}

Task::~Task()
{
if(m_Thread)
	{
	m_Tasks.remove(s_Id);
	CloseHandle(m_Thread);
	}
}


//========
// Common
//========

VOID Task::Cancel()
{
ScopedLock lock(m_Mutex);
if(m_Thread==NULL)
	return;
Cancelled=true;
m_Done.Wait(lock);
}

Handle<Task> Task::GetTask(DWORD id)
{
Task* task=nullptr;
m_Tasks.try_get(id, &task);
return task;
}

VOID Task::Wait()
{
ScopedLock lock(m_Mutex);
if(m_Thread==NULL)
	return;
m_Done.Wait(lock);
}


//================
// Common Private
//================

VOID Task::DoTask()
{
m_This=nullptr;
try
	{
	m_Procedure();
	}
catch(std::exception&)
	{
	}
m_Done.Trigger();
}

DWORD WINAPI Task::TaskProc(VOID* param)
{
Handle<Task> task=(Task*)param;
task->DoTask();
return 0;
}

Clusters::shared_map<DWORD, Task*> Task::m_Tasks;

}

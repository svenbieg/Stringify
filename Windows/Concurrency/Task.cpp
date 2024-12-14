//==========
// Task.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Task.h"

using namespace Collections;


//===========
// Namespace
//===========

namespace Concurrency {


//==================
// Con-/Destructors
//==================

Task::~Task()
{
if(m_Thread!=NULL)
	CloseHandle(m_Thread);
s_Tasks.remove(m_Id);
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

Handle<Task> Task::Get()
{
DWORD id=GetCurrentThreadId();
return s_Tasks.get(id);
}

VOID Task::Resume()
{
m_This=this;
ResumeThread(m_Thread);
}

VOID Task::Wait()
{
ScopedLock lock(m_Mutex);
if(m_Thread==NULL)
	return;
m_Done.Wait(lock);
}


//============================
// Con-/Destructors Protected
//============================

Task::Task():
Cancelled(false),
m_Id(0),
m_Status(Status::Pending),
m_Then(nullptr),
m_Thread(NULL)
{
m_Thread=CreateThread(nullptr, 0, TaskProc, this, CREATE_SUSPENDED, &m_Id);
if(m_Thread==INVALID_HANDLE_VALUE)
	m_Thread=NULL;
if(!m_Thread)
	throw InvalidArgumentException();
s_Tasks.add(m_Id, this);
}


//================
// Common Private
//================

DWORD WINAPI Task::TaskProc(VOID* param)
{
Handle<Task> task=(Task*)param;
task->m_This=nullptr;
Status status=Status::Success;
try
	{
	task->Run();
	}
catch(Exception& e)
	{
	status=e.GetStatus();
	}
task->m_Status=status;
task->m_Done.Trigger();
if(task->m_Then)
	{
	MainTask::Dispatch(task->m_Then);
	task->m_Then=nullptr;
	}
return 0;
}

shared_map<DWORD, Task*> Task::s_Tasks;

}

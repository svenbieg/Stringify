//=====================
// DispatchedQueue.cpp
//=====================

#include "Concurrency/DispatchedQueue.h"


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

VOID DispatchedQueue::Append(DispatchedHandler* handler)
{
WriteLock lock(s_Mutex);
if(!s_First)
	{
	s_First=handler;
	s_Last=handler;
	}
else
	{
	s_Last->m_Next=handler;
	s_Last=handler;
	}
PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
}

VOID DispatchedQueue::Initialize()
{
s_ThreadId=GetCurrentThreadId();
}

VOID DispatchedQueue::Run()
{
WriteLock lock(s_Mutex);
while(s_First)
	{
	auto handler=s_First;
	s_First=nullptr;
	s_Last=nullptr;
	lock.Unlock();
	while(handler)
		{
		handler->Run();
		auto next=handler->m_Next;
		delete handler;
		handler=next;
		}
	lock.Lock();
	}
}


//================
// Common Private
//================

DispatchedHandler* DispatchedQueue::s_First=nullptr;
DispatchedHandler* DispatchedQueue::s_Last=nullptr;
Mutex DispatchedQueue::s_Mutex;
DWORD DispatchedQueue::s_ThreadId=0;

}
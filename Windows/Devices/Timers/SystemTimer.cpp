//=================
// SystemTimer.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "SystemTimer.h"

using namespace Concurrency;


//===========
// Namespace
//===========

namespace Devices {
	namespace Timers {


//==================
// Con-/Destructors
//==================

SystemTimer::SystemTimer()
{
m_Task=CreateTask(this, &SystemTimer::TaskProc);
}

SystemTimer::~SystemTimer()
{
m_Task->Cancel();
}


//========
// Common
//========

Handle<SystemTimer> SystemTimer::Open()
{
if(!s_Current)
	s_Current=new SystemTimer();
return s_Current;
}

//================
// Common Private
//================

VOID SystemTimer::TaskProc()
{
auto task=GetCurrentTask();
while(!task->Cancelled)
	{
	Tick(this);
	Sleep(10);
	}
}

Handle<SystemTimer> SystemTimer::s_Current;

}}
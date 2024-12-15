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
m_Task=Scheduler::CreateTask(this, &SystemTimer::TaskProc);
}

SystemTimer::~SystemTimer()
{
m_Task->Cancel();
}


//========
// Common
//========

SIZE_T SystemTimer::GetTickCount()
{
return (SIZE_T)::GetTickCount64();
}

SIZE_T SystemTimer::Microseconds()
{
LARGE_INTEGER ticks;
QueryPerformanceFrequency(&ticks);
ticks.QuadPart/=1000;
LARGE_INTEGER time;
QueryPerformanceCounter(&time);
return time.QuadPart/ticks.QuadPart;
}

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
auto task=Task::Get();
while(!task->Cancelled)
	{
	Tick(this);
	Sleep(10);
	}
}

Handle<SystemTimer> SystemTimer::s_Current;

}}
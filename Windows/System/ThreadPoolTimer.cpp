//=====================
// ThreadPoolTimer.cpp
//=====================

#include "pch.h"


//=======
// Using
//=======

#include "ThreadPoolTimer.h"


//===========
// Namespace
//===========

namespace System {


//==================
// Con-/Destructors
//==================

ThreadPoolTimer::ThreadPoolTimer():
pTimer(nullptr)
{}

ThreadPoolTimer::~ThreadPoolTimer()
{
Stop();
}


//========
// Common
//========

VOID ThreadPoolTimer::StartOnce(UINT ms)
{
Stop();
if(!pTimer)
	pTimer=CreateThreadpoolTimer(TimerProc, this, nullptr);
ULARGE_INTEGER time;
time.QuadPart=-((UINT64)ms*10'000);
FILETIME t;
t.dwLowDateTime=time.LowPart;
t.dwHighDateTime=time.HighPart;
SetThreadpoolTimer(pTimer, &t, 0, 0);
}

VOID ThreadPoolTimer::StartPeriodic(UINT ms)
{
Stop();
if(!pTimer)
	pTimer=CreateThreadpoolTimer(TimerProc, this, nullptr);
ULARGE_INTEGER time;
time.QuadPart=-((UINT64)ms*10'000);
FILETIME t;
t.dwLowDateTime=time.LowPart;
t.dwHighDateTime=time.HighPart;
SetThreadpoolTimer(pTimer, &t, ms, 0);
}

VOID ThreadPoolTimer::Stop()
{
if(!pTimer)
	return;
SetThreadpoolTimer(pTimer, nullptr, 0, 0);
WaitForThreadpoolTimerCallbacks(pTimer, false);
CloseThreadpoolTimer(pTimer);
pTimer=nullptr;
}


//================
// Common Private
//================

VOID CALLBACK ThreadPoolTimer::TimerProc(PTP_CALLBACK_INSTANCE inst, VOID* param, PTP_TIMER tp_timer)
{
auto timer=(ThreadPoolTimer*)param;
timer->Triggered(timer);
}

}
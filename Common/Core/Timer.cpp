//===========
// Timer.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Devices/Clock.h"
#include "Timer.h"

using namespace Devices;


//===========
// Namespace
//===========

namespace Core {


//==================
// Con-/Destructors
//==================

Timer::Timer():
iInterval(0),
uLastTime(0)
{}

Timer::~Timer()
{
Stop();
}


//========
// Common
//========

VOID Timer::Reset()
{
uLastTime=GetTickCount64();
}

VOID Timer::StartOnce(UINT ms)
{
if(iInterval!=0)
	Stop();
if(ms==0)
	return;
iInterval=ms;
uLastTime=GetTickCount64();
auto clock=Clock::Get();
clock->Tick.Add(this, &Timer::OnClockTick);
}

VOID Timer::StartPeriodic(UINT ms)
{
if(iInterval!=0)
	Stop();
if(ms==0)
	return;
iInterval=-(INT)ms;
uLastTime=GetTickCount64();
auto clock=Clock::Get();
clock->Tick.Add(this, &Timer::OnClockTick);
}

VOID Timer::Stop()
{
if(iInterval==0)
	return;
auto clock=Clock::Get();
clock->Tick.Remove(this);
iInterval=0;
uLastTime=0;
}


//================
// Common Private
//=================

VOID Timer::DoTrigger()
{
Triggered(this);
}

VOID Timer::OnClockTick()
{
UINT64 time=GetTickCount64();
UINT dt=iInterval>0? iInterval: -iInterval;
if(time<uLastTime+dt)
	return;
uLastTime=time;
auto app=Application::Current;
if(app)
	app->Dispatch(this, &Timer::DoTrigger);
if(iInterval>0)
	Stop();
}

}
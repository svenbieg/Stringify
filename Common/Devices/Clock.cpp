//===========
// Clock.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Devices/ClockHelper.h"
#include "Clock.h"

using namespace Core;
using namespace Culture;
using namespace Physics;


//===========
// Namespace
//===========

namespace Devices {


//============
// Year-Table
//============

BYTE DaysPerMonth[4][12]={
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }};


//========
// Common
//========

Handle<Clock> Clock::Get()
{
if(!hCurrent)
	hCurrent=new Clock();
return hCurrent;
}

INT Clock::GetDayOfYear()const
{
if(cTimePoint.Year==0)
	return 0;
UINT uyear=cTimePoint.Year;
WORD uyear4=uyear%4;
INT iday=0;
for(INT i=0; i<cTimePoint.Month; i++)
	iday+=DaysPerMonth[uyear4][i];
iday+=cTimePoint.DayOfMonth+1;
return iday;
}

TIMEPOINT Clock::Now()
{
TIMEPOINT tp;
if(!ClockGetTime(tp))
	{
	UINT64 ticks=GetTickCount64();
	TimePointFromTickCount(&tp, ticks);
	}
return tp;
}

BOOL Clock::Update(TIMEPOINT* tp)
{
if(!tp)
	return false;
if(cTimePoint.Year==0)
	return false;
UINT64 ticks=TimePointToTickCount(*tp);
if(ticks==0)
	return false;
UINT64 ticks_now=GetTickCount64();
UINT delta=(UINT)((ticks_now-ticks)/1000);
return ClockGetTime(*tp, delta);
}


//==========================
// Con-/Destructors Private
//==========================

Clock::Clock():
cTimePoint({ 0, 0, 0, 0, 0, 0, 0 }),
uTicks(0)
{
hTimer=new Timer();
hTimer->Triggered.Add(this, &Clock::OnTimerTriggered);
hTimer->StartPeriodic(100);
}


//================
// Common Private
//================

VOID Clock::DoSecond()
{
TIMEPOINT tp_new;
if(!ClockGetTime(tp_new))
	return;
TIMEPOINT tp_old(cTimePoint);
cTimePoint=tp_new;
if(tp_old.Year==0)
	TimeSet(this);
if(tp_old.Second!=tp_new.Second)
	Second(this);
if(tp_old.Minute!=tp_new.Minute)
	Minute(this);
if(tp_old.Hour!=tp_new.Hour)
	Hour(this);
if(tp_old.DayOfMonth!=tp_new.DayOfMonth)
	Day(this);
if(tp_old.Month!=tp_new.Month)
	Month(this);
if(tp_old.Year!=tp_new.Year)
	Year(this);
}

VOID Clock::OnTimerTriggered()
{
Tick(this);
uTicks++;
if(uTicks%10)
	return;
uTicks=0;
auto app=Application::Current;
if(app)
	app->Dispatch(this, &Clock::DoSecond);
}

Handle<Clock> Clock::hCurrent;

}
//=================
// ClockHelper.cpp
//=================

#include "Timing/ClockHelper.h"


//=======
// Using
//=======

#include <time.h>

using namespace Timing;


//===========
// Namespace
//===========

namespace Devices {


//========
// Common
//========

BOOL ClockGetTime(TIME_POINT& tp, UINT delta)
{
time_t tnow;
time(&tnow);
tnow-=delta;
tm tmnow;
localtime_s(&tmnow, &tnow);
if(tmnow.tm_year+1900<2000)
	return false;
tp.Second=tmnow.tm_sec;
tp.Minute=tmnow.tm_min;
tp.Hour=tmnow.tm_hour;
tp.DayOfWeek=tmnow.tm_wday;
tp.DayOfMonth=tmnow.tm_mday;
tp.Month=tmnow.tm_mon;
tp.Year=tmnow.tm_year;
return true;
}

}
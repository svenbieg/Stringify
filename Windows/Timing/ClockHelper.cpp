//=================
// ClockHelper.cpp
//=================

#include "ClockHelper.h"


//=======
// Using
//=======

#include <time.h>


//===========
// Namespace
//===========

namespace Timing {


//========
// Common
//========

BOOL ClockHelper::GetTime(TIME_POINT* tp)
{
time_t tnow;
time(&tnow);
tm tmnow;
localtime_s(&tmnow, &tnow);
if(tmnow.tm_year+1900<2000)
	return false;
tp->Second=tmnow.tm_sec;
tp->Minute=tmnow.tm_min;
tp->Hour=tmnow.tm_hour;
tp->DayOfWeek=tmnow.tm_wday;
tp->DayOfMonth=tmnow.tm_mday;
tp->Month=tmnow.tm_mon;
tp->Year=tmnow.tm_year;
return true;
}

}
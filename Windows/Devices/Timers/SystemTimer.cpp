//=================
// SystemTimer.cpp
//=================

#include "SystemTimer.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Timers {


//========
// Common
//========

UINT64 SystemTimer::GetTickCount()
{
return ::GetTickCount64();
}

UINT64 SystemTimer::Microseconds()
{
LARGE_INTEGER ticks;
QueryPerformanceFrequency(&ticks);
ticks.QuadPart/=1000;
LARGE_INTEGER time;
QueryPerformanceCounter(&time);
return time.QuadPart/ticks.QuadPart;
}

}}
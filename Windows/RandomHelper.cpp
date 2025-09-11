//==================
// RandomHelper.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include <random>
#include "Devices/Timers/SystemTimer.h"
#include "RandomHelper.h"
#include "StringBuilder.h"

using namespace Devices::Timers;


//========
// Common
//========

constexpr CHAR g_RandomChars[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

Handle<String> RandomHelper::GetString(UINT len)
{
StringBuilder builder(len);
std::mt19937 rng(SystemTimer::GetTickCount());
for(UINT u=0; u<len; u++)
	{
	UINT rnd=rng()%TypeHelper::ArraySize(g_RandomChars);
	builder.Append(CharHelper::ToChar(g_RandomChars[rnd]));
	}
return builder.ToString();
}

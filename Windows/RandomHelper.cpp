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

using namespace Devices::Timers;


//========
// Common
//========

constexpr CHAR g_RandomChars[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

Handle<String> RandomHelper::GetString(UINT len)
{
auto str=String::Create(len+1, nullptr);
auto ptr=const_cast<LPTSTR>(str->Begin());
std::mt19937 rng(SystemTimer::GetTickCount());
for(UINT u=0; u<len; u++)
	{
	UINT rnd=rng()%TypeHelper::ArraySize(g_RandomChars);
	ptr[u]=CharHelper::ToChar(g_RandomChars[rnd]);
	}
ptr[len]=0;
return str;
}

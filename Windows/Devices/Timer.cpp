//===========
// Timer.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Timer.h"


//===========
// Namespace
//===========

namespace Devices {


//==================
// Con-/Destructors
//==================

Timer::Timer():
hWindow(NULL),
m_Interval(0)
{
HINSTANCE hinst=GetModuleHandleA(nullptr);
WNDCLASSA wc;
MemoryHelper::Fill(&wc, sizeof(WNDCLASSA), 0);
wc.hInstance=hinst;
wc.lpfnWndProc=WndProc;
wc.lpszClassName="Timer";
RegisterClassA(&wc);
hWindow=CreateWindowExA(0, "Timer", nullptr, 0, 0, 0, 0, 0, NULL, NULL, hinst, this);
if(hWindow==INVALID_HANDLE_VALUE)
	hWindow=NULL;
}

Timer::~Timer()
{
Stop();
if(hWindow)
	DestroyWindow(hWindow);
}


//========
// Common
//========

VOID Timer::StartOnce(UINT ms)
{
if(!hWindow)
	return;
Stop();
m_Interval=ms;
SetTimer(hWindow, 0, ms, nullptr);
}

VOID Timer::StartPeriodic(UINT ms)
{
if(!hWindow)
	return;
Stop();
m_Interval=-(INT)ms;
SetTimer(hWindow, 0, ms, nullptr);
}

VOID Timer::Stop()
{
if(!hWindow)
	return;
KillTimer(hWindow, 0);
m_Interval=0;
}


//================
// Common Private
//================

LRESULT CALLBACK Timer::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
auto timer=(Timer*)GetPropA(hwnd, "Timer");
if(msg==WM_CREATE)
	{
	CREATESTRUCT* cs=(CREATESTRUCT*)lparam;
	timer=(Timer*)cs->lpCreateParams;
	SetPropA(hwnd, "Timer", timer);
	return 0;
	}
if(timer)
	{
	if(msg==WM_TIMER)
		{
		timer->Triggered(timer);
		if(timer->m_Interval>0)
			timer->Stop();
		return 0;
		}
	}
return DefWindowProc(hwnd, msg, wparam, lparam);
}

}
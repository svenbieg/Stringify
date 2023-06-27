//===================
// ThreadPoolTimer.h
//===================

#pragma once


//===========
// Namespace
//===========

namespace System {


//===================
// Thread-Pool-Timer
//===================

class ThreadPoolTimer: public Object
{
public:
	// Con-/Destructors
	ThreadPoolTimer();
	~ThreadPoolTimer();

	// Common
	BOOL IsStarted()const { return pTimer!=nullptr; }
	VOID StartOnce(UINT MilliSeconds);
	VOID StartPeriodic(UINT MilliSeconds);
	VOID Stop();
	Event<ThreadPoolTimer> Triggered;

private:
	// Common
	static VOID CALLBACK TimerProc(PTP_CALLBACK_INSTANCE Instance, VOID* Param, PTP_TIMER Timer);
	PTP_TIMER pTimer;
};

}
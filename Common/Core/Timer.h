//=========
// Timer.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Core {


//=======
// Timer
//=======

class Timer: public Object
{
public:
	// Con-/Destructors
	Timer();
	~Timer();

	// Common
	BOOL IsStarted()const { return iInterval!=0; }
	VOID Reset();
	VOID StartOnce(UINT MilliSeconds);
	VOID StartPeriodic(UINT MilliSeconds);
	VOID Stop();
	Event<Timer> Triggered;

private:
	// Common
	VOID DoTrigger();
	VOID OnClockTick();
	INT iInterval;
	UINT64 uLastTime;
};

}
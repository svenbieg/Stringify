//=========
// Timer.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Devices {


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
	VOID StartOnce(UINT MilliSeconds);
	VOID StartPeriodic(UINT MilliSeconds);
	VOID Stop();
	Event<Timer> Triggered;

private:
	// Common
	static LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	HWND hWindow;
	INT iInterval;
};

}
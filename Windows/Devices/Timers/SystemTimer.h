//===============
// SystemTimer.h
//===============

#pragma once


//===========
// Namespace
//===========

namespace Devices {
	namespace Timers {


//==============
// System-Timer
//==============

class SystemTimer: public Object
{
public:
	// Con-/Destructors
	SystemTimer();
	~SystemTimer();

	// Common
	static SIZE_T GetTickCount();
	static SIZE_T Microseconds();
	static Handle<SystemTimer> Open();
	Event<SystemTimer> Tick;

private:
	// Common
	VOID TaskProc();
	Handle<Concurrency::Task> m_Task;
	static Handle<SystemTimer> s_Current;
};

}}
//===================
// DispatchedQueue.h
//===================

#pragma once


//=======
// Using
//=======

#include "Concurrency/Signal.h"
#include "Desktop/Application.h"
#include "DispatchedHandler.h"


//===========
// Namespace
//===========

namespace Concurrency {


//==================
// Dispatched-Queue
//==================

class DispatchedQueue
{
public:
	// Friends
	friend Desktop::Application;

	// Common
	static VOID Append(DispatchedHandler* Handler);
	static VOID Exit();

private:
	// Common
	static VOID Run();
	static DispatchedHandler* s_First;
	static DispatchedHandler* s_Last;
	static Mutex s_Mutex;
};

}
//===============
// SystemTimer.h
//===============

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Timers {


//==============
// System-Timer
//==============

class SystemTimer
{
public:
	// Common
	static UINT64 GetTickCount();
	static UINT64 Microseconds();
};

}}
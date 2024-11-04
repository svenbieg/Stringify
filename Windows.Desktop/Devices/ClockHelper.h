//===============
// ClockHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include "Timing/TimePoint.h"


//===========
// Namespace
//===========

namespace Devices {


//========
// Common
//========

BOOL ClockGetTime(Timing::TIMEPOINT& TimePoint, UINT Delta=0);

}
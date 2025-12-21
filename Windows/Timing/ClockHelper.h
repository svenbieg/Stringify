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

namespace Timing {


//==============
// Clock-Helper
//==============

class ClockHelper
{
public:
	static BOOL ClockGetTime(Timing::TIMEPOINT& TimePoint, UINT Delta=0);
};

}
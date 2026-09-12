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
	static BOOL GetTime(Timing::TIME_POINT* TimePoint);
};

}
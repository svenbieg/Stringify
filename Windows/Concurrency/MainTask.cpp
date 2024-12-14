//==============
// MainTask.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "MainTask.h"


//===========
// Namespace
//===========

namespace Concurrency {


//========
// Common
//========

VOID MainTask::Initialize()
{
s_ThreadId=GetCurrentThreadId();
}


//================
// Common Private
//================

DWORD MainTask::s_ThreadId=0;

}
//=======
// pch.h
//=======

#pragma once


//========
// System
//========

#define _WINDOWS
#define _WINSOCKAPI_
#define SECURITY_WIN32

#include <Windows.h>

#undef MessageBox


//======================
// Forward-Declarations
//======================

#include "ErrorHelper.h"
#include "MemoryHelper.h"
#include "ResourceHelper.h"
#include "TypeHelper.h"

#define WM_DISPATCH (WM_USER+1)


//=======
// Using
//=======

#include "Common/Platform.h"


//========
// Common
//========

#include "ComPointer.h"
#include "Convertible.h"
#include "TaskHelper.h"
#include "TimeHelper.h"

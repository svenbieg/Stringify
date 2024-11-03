//============
// Platform.h
//============

#pragma once


//========
// System
//========

#define _WINDOWS
#define _WINSOCKAPI_
#define SECURITY_WIN32

#include <Windows.h>

#undef CopyMemory
#undef FillMemory
#undef MessageBox
#undef MoveMemory
#undef ZeroMemory

#undef VOID
#undef INT_MAX
#undef INT_MIN
#undef UINT_MAX

#include "MemoryHelper.h"


//======================
// Forward-Declarations
//======================

constexpr SIZE_T PAGE_SIZE=4096;

typedef UINT64 FILE_SIZE;

constexpr FILE_SIZE FILE_SIZE_MAX=0xFFFFFFFFFFFFFFFF;

#define WM_DISPATCH (WM_USER+1)

#define ICO_APP 1


//=======
// Using
//=======

#include "Default/Platform.h"


//========
// Common
//========

#include "ComPointer.h"
#include "Convertible.h"
#include "ErrorHelper.h"
#include "ResourceHelper.h"
#include "TaskHelper.h"
#include "TimeHelper.h"

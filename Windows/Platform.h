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

#undef INT_MAX
#undef INT_MIN
#undef UINT_MAX
#undef VOID

#undef CopyMemory
#undef FillMemory
#undef MessageBox
#undef MoveMemory
#undef ZeroMemory


//======================
// Forward-Declarations
//======================

#define __long long
#define __longlong long long

typedef unsigned __longlong int FILE_SIZE;

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

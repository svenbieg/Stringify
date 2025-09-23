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

#undef TEXT

#undef DrawText
#undef InterlockedDecrement
#undef InterlockedIncrement

#undef MessageBox
#ifdef _UNICODE
#define MessageBoxT MessageBoxW
#else
#define MessageBoxT MessageBoxA
#endif


//======================
// Forward-Declarations
//======================

#define __LONG long
#define __LONG_LONG long long

typedef unsigned __LONG_LONG int FILE_SIZE;

constexpr FILE_SIZE FILE_SIZE_MAX=0xFFFFFFFFFFFFFFFF;
constexpr UINT PAGE_SIZE=4096;

#define WM_DISPATCH (WM_USER+1)

#define ICO_APP 1


//=======
// Using
//=======

#include "Default/Platform.h"


//========
// Common
//========

#include "ErrorHelper.h"
